
#include "registers.h"
#include "uart.h"


// Interrupt service routine for bluetooth module
// occurs when BT data is ready to be received.
// requires global buffers to transfer received characters
extern char bt_buffer[1000];
extern int  bt_lines;

void bt_read_ready_ISR(void) {

    while (can_receive(BLUETOOTH)) { 

        char data = *(bt_uart);
        strncat(bt_buffer,&data,1);
        if(data == '\n') bt_lines++;

    }

    // signal to UART controller that interrupt is over
    *(bt_uart + 2 ) = 0;
}