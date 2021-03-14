#include <stdio.h>
#include <string.h>
#include "parameters.h"

extern int Embed[39][4];
extern int Weight_Matrix[4][32];
extern int Recurrent_Matrix[32][32];
extern int Bias[32];
extern int Dense[32];
extern int Dense_Bias;
extern int char_list[256];


int test_rnn_input(char * test)
{
    init_char_list();
    
    int input_sequence[32];
    int padding = 32 - ((int) strlen(test));
    
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
