#include "port.h"
#include "kernel/kernel-internal.h"
#include "cmsis_device.h"

void fos_port_setup_interrupts(void)
{
	// TODO: Fix interrupt priorities

	// NVIC_SetPriorityGrouping(0);
	// NVIC_SetPriority(SysTick_IRQn, 1);
	// NVIC_SetPriority(PendSV_IRQn, 5);
}

regval_t fos_port_init_task_stack(regval_t sp, fos_task_entry_t entry, const void* context)
{
	// Setup the hardware stack pushed by the NVIC
	struct fos_hw_ctx *hw_ctx = (struct fos_hw_ctx *)(sp - sizeof(struct fos_hw_ctx));
	hw_ctx->pc  = (regval_t)entry;						// The stack entry point
	hw_ctx->r0  = (regval_t)context;					// The first argument is passed through r0
	hw_ctx->psr = (regval_t)FOS_PORT_DEFAULT_PSR;		// The default value for the status register
	hw_ctx->lr  = (regval_t)&fos_kernel_end_of_task;	// This method will happen at the end of the task

	sp -= sizeof(struct fos_hw_ctx);

	struct fos_sw_ctx *sw_ctx = (struct fos_sw_ctx *)(sp - sizeof(struct fos_sw_ctx));

	// TODO: This is not needed, only added for debug purposes so we can
	// see that the registers are in place after a context switch
	sw_ctx->r4 = 0x40000004;				// For debug purposes
	sw_ctx->r5 = 0x40000006;				// For debug purposes
	sw_ctx->r6 = 0x40000007;				// For debug purposes
	sw_ctx->r7 = 0x40000008;				// For debug purposes
	sw_ctx->r8 = 0x40000008;				// For debug purposes

	// Reserve place for the software stack too
	sp -= sizeof(struct fos_sw_ctx);

	return sp;
}

