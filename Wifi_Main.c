#include <stdio.h>

#define wifi_uart  (volatile int *) 0xFF200060
#define wifi_reset (volatile int *) 0xFF200080

// check all status register bits in UART core
void check_status() {

	int status = *(wifi_uart+2);

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

int can_receive() {
	return *(wifi_uart+2) & 1 << 7;
}


void reset_wifi(){

	// reset value should start at 0
	// set it manually to 0 just to be sure
	*(wifi_reset) = 0;
    check_status();
    *(wifi_reset) = 1;

    while(can_receive()) {
    	printf("%c", *(wifi_uart));
    }


    // reset status register
	*(wifi_uart+2) = 0;
}

int can_transmit() {
	return *(wifi_uart+2) & 1 << 6;
}

void send_command(char * cmd) {
    for (char * i = cmd; *i != '\0'; i++) {
    	while(!can_transmit());
    	*(wifi_uart+1) = *i;
    }
}

/*
        "RXDATA:  *(wifi_uart));
        "TXDATA:  *(wifi_uart+1));
        "STATUS:  *(wifi_uart+2));
        "DIVISOR: *(wifi_uart+5));
        "CONTROL: *(wifi_uart+3));
        "DIVISOR: *(wifi_uart+4));
*/

int main() {

	reset_wifi();

    char * cmd = "AT+CIFSR\r\n";
    char buffer[1000];

	check_status();

	send_command(cmd);

    int counter = 0;
    int buffer_ptr = 0;
    while(1) {

    	if (can_receive()) {
    		buffer[buffer_ptr] = *(wifi_uart);
    		counter = 0;
    		buffer_ptr++;
    	}
    	counter++;
    	if (counter == 100000) {

    		if (buffer_ptr > 0) {

    			//printf("buffer size %d \n", buffer_ptr);
    			for (int i =0; i< buffer_ptr; i++) {
    				printf("%c",buffer[i]);
    			}

    			buffer_ptr = 0;
    		}
    		counter = 0;
    	}
    }
}
