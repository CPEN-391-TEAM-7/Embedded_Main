
// used to choose the UART controller for each module
#define WIFI       1
#define BLUETOOTH  0

int  can_transmit(int module);
void send_command(int module, char * cmd);
int  can_receive(int module);
int  receive_single_data( int module ,char * buffer, int print);
void check_status(int module);