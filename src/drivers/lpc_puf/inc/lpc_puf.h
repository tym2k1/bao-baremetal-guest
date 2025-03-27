// CPU Specific register headers for `fsl_device_registers.h`
// TODO: Picked at random, might be wrong
#define CPU_LPC55S69JBD100_cm33_core0

#define CORE_CLK_FREQ CLOCK_GetFreq(kCLOCK_CoreSysClk)
/* Worst-case time in ms to fully discharge PUF SRAM */
#define PUF_DISCHARGE_TIME 400
#define PUF_INTRINSIC_KEY_SIZE 16

void lpc_puf_init(void);
