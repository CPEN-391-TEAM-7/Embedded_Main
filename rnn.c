#include <stdio.h>
#include <string.h>
#include "parameters.h"
#include "registers.h"
#include "rnn.h"


int rnn_can_load() {
    return *(rnn + load_read);
}

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
void load_dense_weight_matrix_FPGA() {
    int i = 0;
    while(i < 32) {
        while(!rnn_can_load())
            ;
        *(rnn + d_write) = (i << 16) + Dense[i];
        i++;
    }
}
void load_dense_bias_FPGA() {
    while(!rnn_can_load())
        ;
    *(rnn + db_write) = Dense_Bias;
}

void rnn_apply_dense() {
    while(!rnn_can_load())
        ;
    *(rnn + rnn_finish) = 1;
}

void rnn_start_sequence() {
    while(!rnn_can_load())
        ;
    *rnn = 1;
}

// load all params into memory
void initialize_rnn_params(){
    init_char_list();
    init_embed();
    init_weight_matrix();
    init_recurrent_matrix();
    init_rnn_bias();
    init_dense(); 
}

void load_params_into_FPGA(){

    load_rnn_weight_matrix_FPGA();
    load_rnn_recurrent_matrix_FPGA();
    load_rnn_bias_vector_FPGA();
    load_dense_weight_matrix_FPGA();
    load_dense_bias_FPGA();
}

void load_input_into_FPGA(int emb) {

    int i = 0;
    while(i < 4){

        while(!rnn_can_load())
            ;

        int val =  (i << 16) + Embed[emb][i];

        printf("%d\n",val);

        *(rnn + i_write) = val;
        i++;
    }
}

int test_rnn_input(char * test)
{
    
    int input_sequence[32];
    int len = ((int) strlen(test));
    if (len > 32) return 1;
    int padding = 32 - len;
    
    int i = 0;
    
    while(i<padding) {
        input_sequence[i] = 0;
        i++;
    }
    
    while (i<32) {
        input_sequence[i] = char_list[ (int) test[i-padding] ];
        i++;
    }
    
    int x = 0;
     while(x<32) {
        printf("%d ",input_sequence[x]);
        x++;
    }

    printf("\n");

    return 0;
}
