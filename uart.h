
// used to choose the UART controller for each module
#define WIFI       1
#define BLUETOOTH  0

void enable_uart_read_irq(int module);
void disable_uart_read_irq(int module);
int  can_transmit(int module);
void send_command(int module, char * cmd);
int  can_receive(int module);
void  receive_single_data( int module ,char * buffer, int print);
void check_status(int module);
void reset_wifi(void);
void reset_bluetooth(void);