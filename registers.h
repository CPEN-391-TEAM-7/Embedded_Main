// ARM GIC registers
#define ICCICR  (volatile int *) 0xFFFEC100
#define ICCPMR  (volatile int *) 0xFFFEC104
#define ICCIAR  (volatile int *) 0xFFFEC10C
#define ICCEOIR (volatile int *) 0xFFFEC110
#define ICDDCR  (volatile int *) 0xFFFED000

// Arm core private timer
#define timer      (volatile int *) 0xFFFEC600

// bluetooth module connected to UART controller
#define bt_uart    (volatile int *) 0xFF200100

// Wifi module connected to UART controller, with dedicated reset hardware
#define wifi_uart  (volatile int *) 0xFF200060
#define wifi_reset (volatile int *) 0xFF200080

// led display on DE1 SoC
#define leds       (volatile int *) 0xFF200020

// hex displays on front of DE1 SoC
#define hex0       (volatile int *) 0xFF200030
#define hex1       (volatile int *) 0xFF200040
