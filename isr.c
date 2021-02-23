#include <string.h>
#include "registers.h"
#include "uart.h"


// Interrupt service routine for RFS modules
// occurs when data is ready to be received.
// requires global buffers to transfer received characters
extern char bt_buffer[1000];
extern int  bt_lines;
extern int  enable_rnn;

extern char wifi_buffer[1000];
extern int  wifi_lines;
extern int  send_now;
extern int  echo_off;

void bt_read_ready_ISR(void) {

    while (can_receive(BLUETOOTH)) { 

        char data = *(bt_uart);
        strncat(bt_buffer,&data,1);
        if(data == '\n') bt_lines++;
        if(data == '0') {
            *(leds) = 1023; // turn on all LEDs as a warning
            enable_rnn = 0;
        }
        if(data == '1') {
            *(leds)    = 0; // turn off all LEDs
            enable_rnn = 1;
        }
    }
    // signal to UART controller that interrupt is over
    *(bt_uart + 2 ) = 0;
}

void wifi_read_ready_ISR(void) {

    while (can_receive(WIFI)) { 
        char data = *(wifi_uart);
        if(data == '+') echo_off = 0;               // start recording when a +IPD is received
        if(data == '&') wifi_lines++;               // using a special stop character for now
        if(data == '>') send_now = 1;         // send signal that ESP8266 is ready to receive outbound data
        if( data != '\n' && 
            data != '\r' &&
            data != '&'  &&  
            enable_rnn   &&
            !echo_off) {
                strncat(wifi_buffer,&data,1);    // buffer data if all conditions met
            }
    }

    // signal to UART controller that interrupt is over
    *(wifi_uart + 2 ) = 0;
}