#include <stdio.h>
#include <string.h>

#include "interrupts.h"
#include "registers.h" 
#include "isr.h"


/* ********************************************************************************
 * This file contains code adapted from the intel documentation for using the ARM GIC
 * Reference: ftp://ftp.intel.com/pub/fpgaup/pub/Intel_Material/17.1/Tutorials/Using_GIC.pdf
 * See all additional references on Github
 ********************************************************************************/


#define BLUETOOTH_IRQ4 76
#define WIFI_IRQ3      75

void __attribute__ ((interrupt)) __cs3_isr_irq (void) {

    int interrupt_id = *(ICCIAR);

    if (interrupt_id == BLUETOOTH_IRQ4) bt_read_ready_ISR();
    else if (interrupt_id == WIFI_IRQ3) wifi_read_ready_ISR();
	else
		while (1);			

	*(ICCEOIR) = interrupt_id;

	return;
} 

// halt on all other exceptions
void __attribute__ ((interrupt)) __cs3_reset (void) {
    while(1);
}

void __attribute__ ((interrupt)) __cs3_isr_undef (void) {
    while(1);
}

void __attribute__ ((interrupt)) __cs3_isr_swi (void) {
    while(1);
}

void __attribute__ ((interrupt)) __cs3_isr_pabort (void) {
    while(1);
}

void __attribute__ ((interrupt)) __cs3_isr_dabort (void) {
    while(1);
}

void __attribute__ ((interrupt)) __cs3_isr_fiq (void) {
    while(1);
}

void disable_A9_interrupts(void) {
    int status = 0b11010011;
    asm("msr cpsr, %[ps]" : : [ps] "r"(status));
}

void set_A9_IRQ_stack(void) {
    int stack, mode;
    stack = 0xFFFFFFFF - 7;
    mode = 0b11010010;
    asm("msr cpsr, %[ps]" : : [ps] "r"(mode));
    asm("mov sp, %[ps]" : : [ps] "r"(stack));
    mode = 0b11010011;
    asm("msr cpsr, %[ps]" : : [ps] "r"(mode));
}

void enable_A9_interrupts(void) {
    int status = 0b01010011;
    asm("msr cpsr, %[ps]" : : [ps] "r"(status));
}

void config_GIC(void) {
    // configure the interrupt for IRQ level 4
    // which is the Bluetooth module on CPU 1
    config_interrupt(BLUETOOTH_IRQ4,1);
    config_interrupt(WIFI_IRQ3,1);

    *(ICCPMR) = 0xFFFF; // Enable interrupts of all priorities
    *(ICCICR) = 1;      // Enable signaling of interrupts
    *(ICDDCR) = 1;      // Configure the Distributor Control Register to send pending interrupts to CPUs
}

void config_interrupt(int N,int CPU_target) {
    int reg_offset, index, value, address;

    /* Configure the Interrupt Set-Enable Registers (ICDISERn).
     * reg_offset = (integer_div(N / 32)*4
     * value = 1 << (N mod 32) */
    reg_offset = (N >> 3) & 0xFFFFFFFC;
    index      = N & 0x1F;
    value      = 0x1 << index;
    address    = 0xFFFED100 + reg_offset;
    *(int*) address |= value;

    /* Configure the Interrupt Processor Targets Register (ICDIPTRn)
     * reg_offset = integer_div(N / 4)*4
     * index = N mod 4 */

    reg_offset = (N & 0xFFFFFFFC);
    index      = N & 0x3;
    address    = 0xFFFED800 + reg_offset + index;
    *(char*)address = (char)CPU_target;
}


// wait for number of ms, uses hardware timer
void sleep(int millis) {

	int ticks    = millis*200000; // 200E6 ticks is one second
	*(timer)     = ticks;
	*(timer + 2) = 1; // prevent timer from restarting automatically
	while (*(timer + 3) == 0)
		;
	*(timer + 3) = 1; // reset timer flag
}