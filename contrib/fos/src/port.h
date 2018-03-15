#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "fos/kernel-data.h"
#include "kernel-internal.h"

#define FOS_PORT_MIN_STACK_SIZE    ((sizeof(struct fos_hw_ctx) + sizeof(struct fos_sw_ctx)))
#define FOS_PORT_EXC_RETURN_TASK   (0xfffffffd)
#define FOS_PORT_EXC_RETURN_KERNEL (0xfffffff1)
#define FOS_PORT_DEFAULT_PSR		  (0x01000000)

/**
 * Context saved on the stack by the NVIC (hardware interrupt controller)
 */
__attribute__((packed)) struct fos_hw_ctx {
	regval_t r0;
	regval_t r1;
	regval_t r2;
	regval_t r3;
	regval_t r12;
	regval_t lr;
	regval_t pc;
	regval_t psr;
};

/**
 * Context saved on the stack by the schdualer (completes the hw_interrupt_context)
 */
__attribute__((packed)) struct fos_sw_ctx{
	regval_t r4;
	regval_t r5;
	regval_t r6;
	regval_t r7;
	regval_t r8;
	regval_t r9;
	regval_t r10;
	regval_t r11;
	regval_t lr;
};

/**
 * Setup the interrupts and priorities.
 */
void fos_port_setup_interrupts(void);

/**
 * Setup the task initial stack structure.
 *
 * @param sp 	Pointer to the start of the stack.
 * @param entry The entry point of the program
 * @param context A context that will pass to the task.
 * @return The new location of the stack pointer
 */
regval_t fos_port_init_task_stack(regval_t sp, fos_task_entry_t entry, const void* context);

/**
 * Run the first task
 */
void fos_port_run_first_task(void);

#ifdef __cplusplus
}
#endif
