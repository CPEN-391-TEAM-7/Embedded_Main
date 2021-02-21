
/* ********************************************************************************
 * This file contains code adapted from the intel documentation for using the ARM GIC
 * Reference: ftp://ftp.intel.com/pub/fpgaup/pub/Intel_Material/17.1/Tutorials/Using_GIC.pdf
 * See all additional references on Github
 ********************************************************************************/

void disable_A9_interrupts (void);
void set_A9_IRQ_stack (void);
void config_GIC (void);
void config_bt (void);
void enable_A9_interrupts (void);