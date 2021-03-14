#include <stdio.h>
#include <string.h>
#include "parameters.h"
#include "registers.h"

extern int Embed[39][4];
extern int Weight_Matrix[4][32];
extern int Recurrent_Matrix[32][32];
extern int Bias[32];
extern int Dense[32];
extern int Dense_Bias;
extern int char_list[256];


// load all params into memory
void initialize_rnn_params(){
    init_char_list();
    init_embed();
    init_weight_matrix();
    init_recurrent_matrix();
    init_rnn_bias();
    init_dense(); 
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

    return 0;
}
