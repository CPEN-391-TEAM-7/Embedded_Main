#include <stdio.h>

#define leds       (volatile int *) 0xFF200020
#define wifi_uart  (volatile int *) 0xFF200060
#define wifi_reset (volatile int *) 0xFF200080
#define bt_uart    (volatile int *) 0xFF200100

#define WIFI       1
#define BLUETOOTH  0

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

int can_receive(int module) {
	if (module == WIFI) return *(wifi_uart+2) & 1 << 7;
	else                return *(  bt_uart+2) & 1 << 7;
}


void reset_wifi(){

	// reset value should start at 0
	// set it manually to 0 just to be sure
	*(wifi_reset) = 0;
    check_status(WIFI);
    *(wifi_reset) = 1;

    while(can_receive(WIFI)) {
    	printf("%c", *(wifi_uart));
    }


    // reset status register
	*(wifi_uart+2) = 0;
}

int can_transmit(int module) {
	if (module == WIFI) return *(wifi_uart+2) & 1 << 6;
	else                return *(  bt_uart+2) & 1 << 6;
}

void send_command(int module, char * cmd) {
    for (char * i = cmd; *i != '\0'; i++) {
    	while(!can_transmit(WIFI));
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

	// WiFi testing command
    //char * cmd = "AT+CIFSR\r\n";

	// Bluetooth testing command
	char * cmd = "AT\r\n";

    char wifi_buffer[1000];
    char   bt_buffer[1000];

	check_status(WIFI);
	check_status(BLUETOOTH);

	send_command(BLUETOOTH, cmd);

    int counter = 0;

    int wifi_ptr = 0;
    int bt_ptr   = 0;

    int action_counter = 0;


    while(1) {

    	if (can_receive(WIFI)) {
    		wifi_buffer[wifi_ptr] = *(wifi_uart);
    		counter = 0;
    		wifi_ptr++;
    	}

    	if (can_receive(BLUETOOTH)) {
    		bt_buffer[bt_ptr] = *(bt_uart);
    		counter = 0;
    		bt_ptr++;
    	}

    	counter++;
    	if (counter == 100000) {

    		// Get cool LED visuals
    		action_counter++;
    		*(leds) = action_counter;

    		// read all buffered wifi responses
    		if (wifi_ptr > 0) {
    			for (int i =0; i< wifi_ptr; i++) {
    				printf("%c",wifi_buffer[i]);
    			}
    			wifi_ptr = 0;
    		}

    		// read all buffered bluetooth responses
    		if (bt_ptr > 0) {
    			for (int i =0; i< bt_ptr; i++) {
    				printf("%c",bt_buffer[i]);
    			}
    			bt_ptr = 0;
    		}

    		counter = 0;
    	}
    }
}
