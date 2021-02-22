#include <stdio.h>
#include "uart.h"
#include "interrupts.h"
#include "registers.h"

/*
        "RXDATA:  *(wifi_uart));
        "TXDATA:  *(wifi_uart+1));
        "STATUS:  *(wifi_uart+2));
        "CONTROL: *(wifi_uart+3));
        "DIVISOR: *(wifi_uart+4));
*/

// enable interrupts for either module
// write 1 to bit 7 of the control register
void enable_uart_read_irq(int module) {
    
    if(module == WIFI) {
        *(wifi_uart + 2) = 0;
        *(wifi_uart + 3) = 0x1 << 7;
    } else {
        *(bt_uart + 2) = 0;
        *(bt_uart + 3) = 0x1 << 7;
    }
}

// disable interrupts for either module
// write 0 to bit 7 of the control register
void disable_uart_read_irq(int module) {
    
    if(module == WIFI) *(wifi_uart + 3) = 0;
    else *(bt_uart + 3) = 0;
    
}

// check if transmit ready bit is high in UART controller
int can_transmit(int module) {
	if (module == WIFI) return *(wifi_uart+2) & 1 << 6;
	else                return *(  bt_uart+2) & 1 << 6;
}

// sends chars to UART controller as long as it can transmit
void send_command(int module, char * cmd) {
	char * i = cmd;
    while( *i != '\0') {
    	while(!can_transmit(module));
    	if (module == WIFI) *(wifi_uart+1) = *i;
    	else                *(  bt_uart+1) = *i;
		i++;
    }
}

// check if receive status bit is 1 in UART controller
int can_receive(int module) {
	if (module == WIFI) return *(wifi_uart+2) & 1 << 7;
	else                return *(  bt_uart+2) & 1 << 7;
}


// receive uart data by using polling instead of interrupts.
// useful during setup phase
int receive_single_data( int module ,char * buffer, int print) {

	int ptr = 0;
	int counter = 0;

	while (counter < 100000) {
		if (can_receive(module)) {
	    	if (module == WIFI) buffer[ptr] = *(wifi_uart);
	    	else                buffer[ptr] = *(bt_uart);
	    	counter = 0;
	    	ptr++;
		}
		counter++;
	}

	if (print) {
		if (ptr > 0) {
			int i = 0;
			while (i < ptr) {
				printf("%c",buffer[i]);
				i++;
			}
		}
	}

	return ptr;
}

void reset_wifi(){

	// reset value should start at 0
	// set it manually to 0 just to be sure
	*(wifi_reset) = 0;
	sleep(1000);
    *(wifi_reset) = 1;

    // reset status register
	*(wifi_uart+2) = 0;
}

void reset_bluetooth(){
	*(bt_uart+2) = 0;
}

// check all status register bits in UART core
void check_status(int module) {

	int status;

	if (module == WIFI) status = *(wifi_uart+2);
	else                status = *(bt_uart+2);

	printf("status = %x\n",status);
	printf("parityerror %x\n",(status & (1 << 0)));
	printf("frame error %x\n",(status & (1 << 1)));
	printf("brk  detect %x\n",(status & (1 << 2)));
	printf("rx overrun  %x\n",(status & (1 << 3)));
	printf("tx overrun  %x\n",(status & (1 << 4)));
	printf("tx empty    %x\n",(status & (1 << 5)));
	printf("tx ready    %x\n",(status & (1 << 6)));
	printf("rx ready    %x\n",(status & (1 << 7)));
	printf("ERROR       %x\n",(status & (1 << 8)));
	printf("dclr to snd %x\n",(status & (1 << 10)));
	printf("clr to snd  %x\n",(status & (1 << 11)));
	printf("eop         %x\n",(status & (1 << 12)));
	printf("\n\n");
}