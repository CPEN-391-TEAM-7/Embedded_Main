#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "wifi_password.h"

#define leds       (volatile int *) 0xFF200020
#define wifi_uart  (volatile int *) 0xFF200060
#define wifi_reset (volatile int *) 0xFF200080
#define bt_uart    (volatile int *) 0xFF200100
#define timer      (volatile int *) 0xFFFEC600
#define hex0       (volatile int *) 0xFF200030
#define hex1       (volatile int *) 0xFF200040

#define WIFI       1
#define BLUETOOTH  0

//global domain counters
int good_domains = 0;
int bad_domains  = 0;

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

int receive_single_data( int module ,char * buffer) {

	int ptr = 0;
	int counter = 0;

	while (counter < 1000000) {
		if (can_receive(module)) {
	    	if (module == WIFI) buffer[ptr] = *(wifi_uart);
	    	else                buffer[ptr] = *(bt_uart);
	    	counter = 0;
	    	ptr++;
		}
		counter++;
	}

	if (ptr > 0) {
		for (int i =0; i< ptr; i++) {
			printf("%c",buffer[i]);
	    }
	}

	return ptr;
}

int get_result(long len) { // temporarily all domains with odd number of characters are malware
    return (len % 2);
}

long parse_ipd( char * msg, char * domain) {

    msg = msg + 5;                      // Remove string prefix '+IPD' which is 7 characters
    char * rest;
    long ret = strtoul(msg, &rest, 10); // Read size of domain name into long, 10 refers to base 10
    rest = rest + 1;                    // remove colon before domain name
    strcpy(domain,rest);				// copy extracted domain to allocated char array
    return ret;
}

void send_result(int result, long len, char * domain) {
    
	char wifi_buffer[1000];

    char cmd[100];								// allocate space for AT command
    sprintf(cmd,"AT+CIPSENDEX=%lu,\"192.168.1.78\",56789\r\n",len+1);		// create AT command
    char data[100];								// allocate space for data to be sent
    sprintf(data,"%s%d",domain,result);			// create sending data
	send_command(WIFI, cmd);
	receive_single_data(WIFI,wifi_buffer);
	send_command(WIFI, data);
	receive_single_data(WIFI,wifi_buffer);

}

void update_counters(long result) {

	if(result) {
		bad_domains++;
		*(hex0) = bad_domains;
	} else {
		good_domains++;
		*(hex1) = good_domains;
	}

}

void handle_wifi_buffer(char * buffer, int buffer_size) {

	char buff_copy[1000];                     // make same size as receiving buffer, just in case
	strncpy(buff_copy, buffer, buffer_size);  // copy only the valid characters
	buff_copy[buffer_size] = 0;               // add null terminator

	char * raw = strtok(buff_copy, "\r\n");	  // split buffer wherever there is a new line

    while (raw != NULL)
    {
		if (strstr(raw,"IPD")) {				 // make sure this is a data string

			printf("Got IPD\n");
            char domain[100];                    // allocate space for extracted domain

            long len = parse_ipd(raw, domain);   // get domain and data length
            int result = get_result(len);
            send_result(result,len,domain);
			update_counters(result);
			raw = strtok(NULL, "\r\n");
        } else {
			printf(raw);
			raw = strtok(NULL, "\r\n");
		}
    }

}

int main() {

	printf("initializing WiFi\n");
	reset_wifi();
	sleep(1000);

	printf("initializing Bluetooth\n");
	reset_bluetooth();
	sleep(1000);

    char wifi_buffer[1000];
    char   bt_buffer[1000];

    int counter = 0;

    int wifi_ptr = 0;
    int bt_ptr   = 0;

    int action_counter = 0;

    send_command(BLUETOOTH, "AT\r\n");
    receive_single_data(BLUETOOTH, bt_buffer);

    send_command(BLUETOOTH, "AT+UART=38400,0,0\r\n");
    receive_single_data(BLUETOOTH, bt_buffer);

    send_command(BLUETOOTH, "AT+VERSION?\r\n");
    receive_single_data(BLUETOOTH, bt_buffer);

    send_command(BLUETOOTH, "AT+ADDR?\r\n");
    receive_single_data(BLUETOOTH, bt_buffer);

	send_command(BLUETOOTH, "AT+MRAD?\r\n");
    receive_single_data(BLUETOOTH, bt_buffer);

	send_command(BLUETOOTH, "AT+STATE?\r\n");
    receive_single_data(BLUETOOTH, bt_buffer);

    send_command(BLUETOOTH, "AT+INIT\r\n");
    receive_single_data(BLUETOOTH, bt_buffer);

	// ENTER WIFI PASSWORD HERE
	send_command(WIFI, WIFI_CONNECT_WITH_PASSWORD);
	receive_single_data(WIFI, wifi_buffer);

	sleep(10000);

	send_command(WIFI, "AT+CIFSR\r\n");
	receive_single_data(WIFI, wifi_buffer);

	//send_command(WIFI, "AT+CIPMUX=1\r\n");
	//receive_single_data(WIFI, wifi_buffer);

	send_command(WIFI, "AT+CIPSTART=\"UDP\",\"0.0.0.0\",41234,41234,0\r\n");
	receive_single_data(WIFI, wifi_buffer);

	// Test Server address
	//send_command(WIFI, "AT+CIPSTART=1,\"UDP\",\"192.168.1.78\",56789,56789,2\r\n");

    while(1) {

    	if (can_receive(BLUETOOTH)) {
    	    bt_buffer[bt_ptr] = *(bt_uart);
    	    counter = 0;
    	    bt_ptr++;

			if(bt_ptr > 1000) {
				printf("PANIC: BLUETOOTH OVERFLOW");
				break;
			}

    	}

    	if (can_receive(WIFI)) {
    		wifi_buffer[wifi_ptr] = *(wifi_uart);
    		counter = 0;
    		wifi_ptr++;

			if(wifi_ptr > 1000) {
				printf("PANIC: WIFI OVERFLOW");
				break;
			}
    	}

    	counter++;
    	if (counter == 1000000) {

    		// Get cool LED visuals
    		action_counter++;
    		*(leds) = action_counter;

    		// handle all buffered wifi responses
    		if (wifi_ptr > 0) {
				handle_wifi_buffer(wifi_buffer,wifi_ptr);
    			wifi_ptr = 0;
    		}

    		// read all buffered bluetooth responses
    		if (bt_ptr > 0) {
    			printf("\nBluetooth message: ");
    			for (int i =0; i< bt_ptr; i++) {
    				printf("%c",bt_buffer[i]);
    			}
    			bt_ptr = 0;
    		}

    		counter = 0;
    	}
    }
}
