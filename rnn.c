#include <stdio.h>
#include <string.h>
#include "parameters.h"
#include "registers.h"
#include "rnn.h"

/**
 * check if RNN is in load state
 */
int rnn_can_load() {
    return *(rnn + load_read);
}

/**
 * load rnn weight matrix into FPGA
 */
void load_rnn_weight_matrix_FPGA() {
    int r = 0;
    while(r <4) {
        int c = 0;
        while(c < 32) {

            while(!rnn_can_load())
                ;
            *(rnn + r0_write) = (r << 24) + (c << 16) + Weight_Matrix[r][c];
            c++;
        }
        r++;
    }
}

/**
 * load rnn recurrent matrix into FPGA
 */
void load_rnn_recurrent_matrix_FPGA() {
    int r = 0;
    while(r <32) {
        int c = 0;
        while(c < 32) {

            while(!rnn_can_load())
                ;
            *(rnn + r1_write) = (r << 24) + (c << 16) +  Recurrent_Matrix[r][c];
            c++;
        }
        r++;
    }
}

/**
 * load rnn bias vector into FPGA
 */
void load_rnn_bias_vector_FPGA() {

    int i = 0;
    while(i < 32) {
        while(!rnn_can_load())
            ;
        int val =  (i << 16) + Bias[i];
        *(rnn + rb_write) = val;
        i++;
    }
}

/**
 * load dense layer matrix into FPGA
 */
void load_dense_weight_matrix_FPGA() {
    int i = 0;
    while(i < 32) {
        while(!rnn_can_load())
            ;
        *(rnn + d_write) = (i << 16) + Dense[i];
        i++;
    }
}

/**
 * load dense layer bias into FPGA
 */
void load_dense_bias_FPGA() {
    while(!rnn_can_load())
        ;
    *(rnn + db_write) = Dense_Bias;
}

/**
 * Apply dense layer to hidden state and start final calculation
 */
void rnn_apply_dense() {
    while(!rnn_can_load())
        ;
    *(rnn + rnn_finish) = 1;
}

/**
 * Start multiply after a new input character is given
 * Creates a new hidden state
 */
void rnn_start_sequence() {
    while(!rnn_can_load())
        ;
    *rnn = 1;
}

/**
 * load all params into memory
 */
void initialize_rnn_params(){
    init_char_list();
    init_embed();
    init_weight_matrix();
    init_recurrent_matrix();
    init_rnn_bias();
    init_dense(); 
}

/**
 * load all params from memory into the FPGA
 */
void load_params_into_FPGA(){

    load_rnn_weight_matrix_FPGA();
    load_rnn_recurrent_matrix_FPGA();
    load_rnn_bias_vector_FPGA();
    load_dense_weight_matrix_FPGA();
    load_dense_bias_FPGA();
}

/**
 * load all input vector into FPGA
 * @param int emb - index of embedding vector to load
 */
void load_input_into_FPGA(int emb) {

    int i = 0;
    while(i < 4){

        while(!rnn_can_load())
            ;

        int val =  (i << 16) + Embed[emb][i];
        *(rnn + i_write) = val;
        i++;
    }
}

/**
 * load all params from memory into the FPGA
 * @param char * domain - ptr to domain name string
 * @return 
 *      0 if domain is safe
 *      1 if malware detected
 *      3 if domain too long (only top level domains allowed)
 *      4 if invalid character in domain
 *      5 or 6 if RNN or ARM cpu error 
 */
int rnn_inference(char * domain)
{
    
    int input_sequence[32];
    int len = ((int) strlen(domain));
    if (len > 32) return 3; // domain too long
    int padding = 32 - len;
    
    int i = 0;
    
    // build input sequence
    // pad with 0s before actual domain chars
    // IMPORTANT: padding with 0s provides context on the domain length to the RNN
    //            long domains are more likely to be malware
    while(i<padding) {
        input_sequence[i] = 0;
        i++;
    }
    
    // add embedding vector indices to sequenc
    while (i<32) {
        int val = char_list[ (int) domain[i-padding]];
        // return 4 if -1 is receveived from char list
        if(val == -1) return 4;

        input_sequence[i] = char_list[ (int) domain[i-padding] ];
        i++;
    }
    
    // start loading vectors into FPGA
    int x = 0;
    while(x<32) {
        // load input
        load_input_into_FPGA(input_sequence[x]);
        // create hidden state
        rnn_start_sequence();
        x++;
    }

    // apply dense layer to 32nd hidden state
    rnn_apply_dense();

    // wait for multiply to finish
    while(!*(rnn + valid_read))
        ;

    // get simple binary output
    int binary_output = *(rnn + 4);

    // get actual numerical output
    int actual_output = *(rnn + result_read);

    // do simple checksum to see if actual matches binary output
    if(actual_output > 0 && binary_output == 0) return 5;
    if(actual_output < 0 && binary_output == 1) return 6;

    return binary_output;
}
