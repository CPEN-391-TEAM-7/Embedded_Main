#include <stdio.h>
#include <string.h>

int char_list[256];

void init_char_list(){
    
    for(int i =0; i< 256; i++) {
        char_list[i] = -1;
    }
    
char_list[ (int) ' '] = 0;
char_list[ (int) 'a'] = 1;
char_list[ (int) 'b'] = 2;
char_list[ (int) 'c'] = 3;
char_list[ (int) 'd'] = 4;
char_list[ (int) 'e'] = 5;
char_list[ (int) 'f'] = 6;
char_list[ (int) 'g'] = 7;
char_list[ (int) 'h'] = 8;
char_list[ (int) 'i'] = 9;
char_list[ (int) 'j'] = 10;
char_list[ (int) 'k'] = 11;
char_list[ (int) 'l'] = 12;
char_list[ (int) 'm'] = 13;
char_list[ (int) 'n'] = 14;
char_list[ (int) 'o'] = 15;
char_list[ (int) 'p'] = 16;
char_list[ (int) 'q'] = 17;
char_list[ (int) 'r'] = 18;
char_list[ (int) 's'] = 19;
char_list[ (int) 't'] = 20;
char_list[ (int) 'u'] = 21;
char_list[ (int) 'v'] = 22;
char_list[ (int) 'w'] = 23;
char_list[ (int) 'x'] = 24;
char_list[ (int) 'y'] = 25;
char_list[ (int) 'z'] = 26;
char_list[ (int) '0'] = 27;
char_list[ (int) '1'] = 28;
char_list[ (int) '2'] = 29;
char_list[ (int) '3'] = 30;
char_list[ (int) '4'] = 31;
char_list[ (int) '5'] = 32;
char_list[ (int) '6'] = 33;
char_list[ (int) '7'] = 34;
char_list[ (int) '8'] = 35;
char_list[ (int) '9'] = 36;
char_list[ (int) '.'] = 37;
char_list[ (int) '-'] = 38;
 
}

int main()
{
    
    init_char_list();

    char test[] = "test.ca";
    
    int input_sequence[32];
    int padding = 32 - ((int) strlen(test));
    
    int i = 0;
    
    while(i<padding) {
        input_sequence[i] = 0;
        i++;
    }
    
    while (i<32) {
        int v = (int) test[i-padding];
        input_sequence[i] = char_list[ (int) test[i-padding] ];
        i++;
    }
    
    for (int x =0; x<32;x++) {
        printf("%d ",input_sequence[x]);
    }

    return 0;
}
