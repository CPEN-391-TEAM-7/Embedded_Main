#include <stdio.h>

#define leds       (volatile int *) 0xFF200020
#define wifi_uart  (volatile int *) 0xFF200060
#define wifi_reset (volatile int *) 0xFF200080
#define bt_uart    (volatile int *) 0xFF200100
#define timer      (volatile int *) 0xFFFEC600

#define WIFI       1
#define BLUETOOTH  0

// wait for number of ms, uses hardware timer
void sleep(int millis) {

	int ticks    = millis*200000; // 200E6 ticks is one second
	*(timer)     = ticks;
	*(timer + 2) = 1; // prevent timer from restarting automatically
	while (*(timer + 3) == 0)
		;
	*(timer + 3) = 1; // reset timer flag

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

int can_receive(int module) {
	if (module == WIFI) return *(wifi_uart+2) & 1 << 7;
	else                return *(  bt_uart+2) & 1 << 7;
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

int can_transmit(int module) {
	if (module == WIFI) return *(wifi_uart+2) & 1 << 6;
	else                return *(  bt_uart+2) & 1 << 6;
}

void send_command(int module, char * cmd) {
    for (char * i = cmd; *i != '\0'; i++) {
    	while(!can_transmit(module));
    	if (module == WIFI) *(wifi_uart+1) = *i;
    	else                *(  bt_uart+1) = *i;
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

	printf("initializing WiFi\n");
	reset_wifi();
	sleep(1000);

	printf("initializing Bluetooth\n");
	reset_bluetooth();
	sleep(1000);

	// WiFi testing command
     char * cmd1 = "AT+CWLAP\r\n";

	// Bluetooth testing command
	 char * cmd2 = "AT+STATE?\r\n";

    char wifi_buffer[1000];
    char   bt_buffer[1000];

    int counter = 0;

    int wifi_ptr = 0;
    int bt_ptr   = 0;

    int action_counter = 0;

    //send_command(WIFI,cmd1 );
    send_command(BLUETOOTH, cmd2);

    while(1) {

    	if (can_receive(BLUETOOTH)) {
    	    bt_buffer[bt_ptr] = *(bt_uart);
    	    counter = 0;
    	    bt_ptr++;
    	}

    	if (can_receive(WIFI)) {
    		wifi_buffer[wifi_ptr] = *(wifi_uart);
    		counter = 0;
    		wifi_ptr++;
    	}

    	counter++;
    	if (counter == 1000000) {

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
    			printf("Bluetooth message: ");
    			for (int i =0; i< bt_ptr; i++) {
    				printf("%c",bt_buffer[i]);
    			}
    			bt_ptr = 0;
    		}

    		counter = 0;
    	}
    }
}
