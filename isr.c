#include <string.h>
#include "registers.h"
#include "uart.h"


// Interrupt service routine for RFS modules
// occurs when data is ready to be received.
// requires global buffers to transfer received characters

// bluetooth globals
extern int enable_rnn;

// wifi globals
extern char wifi_buffer[1000];
extern int  wifi_lines;
extern int  send_now;
extern int  handle_enable;

void bt_read_ready_ISR(void) {

    while (can_receive(BLUETOOTH)) { 

        char data = *(bt_uart);

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

char local_buffer[1000];

void wifi_read_ready_ISR(void) {

    while (can_receive(WIFI)) {

        char data = *(wifi_uart);

        if(!enable_rnn) {
            local_buffer[0] = 0;
            *(wifi_uart + 2 ) = 0;
            return;
        }

        if(data == '>') send_now = 1;         // send signal that ESP8266 is ready to receive outbound data
        
        if( data != '\n' && data != '\r') strncat(local_buffer,&data,1);    // buffer data if all conditions met

        if (data == '\n') {

            if(strstr(local_buffer,"IPD")) {        // only IP data is allowed to be sent to handler
                handle_enable = 0;                  // prevent handler from running while data is being added
                strncat(local_buffer,&data,1);      // add newline before sending IPD
                strcat(wifi_buffer,local_buffer);   // send entire IPD to global wifi buffer
                wifi_lines++;                       // increase number of valid lines
                handle_enable = 1;                  // allow data handling
                local_buffer[0] = 0;                // clear local buffer for new line
            } else {
                local_buffer[0] = 0;                // clear on non-IPD data
            }

        }
    }

    // signal to UART controller that interrupt is over
    *(wifi_uart + 2 ) = 0;
}
