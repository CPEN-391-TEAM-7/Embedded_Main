#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "wifi_password.h"
#include "registers.h"
#include "uart.h"
#include "interrupts.h"

#define IPD_PREFIX_SIZE 5

/* ******************************************************************************** */
// GLOBAL VARIABLES
/* ******************************************************************************** */

//global domain counters for hex display
int good_domains = 0;
int bad_domains  = 0;

// global buffers reachable from ISRs
char wifi_buffer[1000];
char   bt_buffer[1000];

// global line counters
volatile int wifi_lines = 0;
volatile int   bt_lines = 0;

// global enable controlled by bluetooth
volatile int enable_rnn = 1;

// global send signal controlled by wifi
volatile int send_now = 0;
volatile int echo_off = 0;

/* ******************************************************************************** */


int get_result(long len) { // temporarily all domains with odd number of characters are malware
    return (len % 2);
}

long parse_ipd( char * msg, char * domain) {

    msg = msg + IPD_PREFIX_SIZE;                      // Remove string prefix '+IPD' which is 5 characters
    char * rest;
    long ret = strtoul(msg, &rest, 10); // Read size of domain name into long, 10 refers to base 10
    rest = rest + 1;                    // remove colon before domain name
    strcpy(domain,rest);				// copy extracted domain to allocated char array
    return ret;
}

void send_result(int result, long len, char * domain) {
    
    char cmd[100];								// allocate space for AT command
    //sprintf(cmd,"AT+CIPSENDEX=%lu,\"34.216.108.218\",8082\r\n",len+1);		// create AT command (ACTUAL BACKEND)
	sprintf(cmd,"AT+CIPSENDEX=%lu,\"192.168.1.78\",8082\r\n",len);		// create AT command
    char data[100];								// allocate space for data to be sent
    sprintf(data,"%s%d",domain,result);			// create sending data
	send_command(WIFI, cmd);
	int timeout = 100000;
	while(!send_now && timeout > 0) {
		timeout--;
	}
	send_command(WIFI, data);
	send_now = 0;

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

void handle_wifi_buffer() {

	char buff_copy[1000];               // make same size as receiving buffer, just in case
	buff_copy[0] = 0;
	disable_uart_read_irq(WIFI);		// disable interrupt before copying buffer so data is not modified during copy
	strcpy(buff_copy, wifi_buffer);  	// copy up to null terminator
	echo_off = 1;
	wifi_buffer[0] = 0;					// clear wifi buffer
	wifi_lines--;
	enable_uart_read_irq(WIFI); 		// renable interrupt once data is copied

	if(strstr(buff_copy,"IPD")) {
		char domain[100];       					// allocate space for extracted domain
		long len = parse_ipd(buff_copy, domain);	// get domain and data length
		int result = get_result(len);
		send_result(result,len,domain);
		update_counters(result);
	} 

}

void handle_bt_buffer() {

	printf("%s",bt_buffer);
	bt_buffer[0] = 0;
    bt_lines     = 0;
}

int main() {

	disable_uart_read_irq(WIFI);	  // turn off wifi receiving interrupt
	disable_uart_read_irq(BLUETOOTH); // turn off bluetooth receiving interrupt

	// clear buffers
	wifi_buffer[0] = 0;
	bt_buffer[0]   = 0;

	printf("initializing WiFi\n");
	reset_wifi();
	sleep(1000);

	printf("initializing Bluetooth\n");
	reset_bluetooth();
	sleep(1000);

    send_command(BLUETOOTH, "AT+UART=38400,0,0\r\n"); // set correct UART settings
    receive_single_data(BLUETOOTH, bt_buffer,1);

    send_command(BLUETOOTH, "AT+INIT\r\n"); // initialize bluetooth 
    receive_single_data(BLUETOOTH, bt_buffer,1);

	// WiFi password in separate non-committed file
	send_command(WIFI, WIFI_CONNECT_WITH_PASSWORD);
	receive_single_data(WIFI, wifi_buffer,1);

	sleep(10000); // wait atleast 10 sec for connection to establish

	// display board IP address
	send_command(WIFI, "AT+CIFSR\r\n"); 
	receive_single_data(WIFI, wifi_buffer,1);

	//start receiving at this port
	send_command(WIFI, "AT+CIPSTART=\"UDP\",\"0.0.0.0\",41234,41234,0\r\n"); 
	receive_single_data(WIFI, wifi_buffer,1);

	// turn off command echoing
	send_command(WIFI, "ATE0\r\n"); 
	receive_single_data(WIFI, wifi_buffer,1);

	printf("Enabling interrupts\n");
	sleep(5000); // wait 5 seconds for all polling commands to settle

	// clear buffers
	wifi_buffer[0] = 0;
	bt_buffer[0]   = 0;

	// Initialzing interrupts
	disable_A9_interrupts ();	// disable interrupts in the A9 processor
	set_A9_IRQ_stack ();	    // initialize the stack pointer for IRQ mode
	config_GIC ();				// configure the general interrupt controller
	enable_uart_read_irq(WIFI);	// turn on wifi receiving interrupt
	enable_uart_read_irq(BLUETOOTH); // turn on bluetooth receiving interrupt

	enable_A9_interrupts ();	// enable interrupts in the A9 processor

	printf("SYSTEM READY\n");

    while(1) {
		if (wifi_lines > 0 && enable_rnn) handle_wifi_buffer();
		if (bt_lines   > 0) handle_bt_buffer();
	}

}
