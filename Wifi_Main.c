#include <stdio.h>

#define wifi_uart (volatile int *) 0xFF200060

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

int can_transmit() {
	return *(wifi_uart+2) & 1 << 6;
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

	*(wifi_uart+2) = 0;
	check_status();

    char * cmd = "AT\r\n\0";

    for (int i =0; i<5 ;i++) {
    	while(!can_transmit());
    	*(wifi_uart+1) = cmd[i];
    	printf("Sending ' %c ' \n", cmd[i]);

    }


    check_status();

    while(1) {
    	if (can_receive()) {
    		printf("%c", *(wifi_uart));
    	}
    }

}
