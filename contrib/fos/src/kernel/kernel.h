#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>
#include "kernel-data.h"

#define FOS_KERNEL_TICKS_PER_SCHEDULE	(50)

extern unsigned int g_fos_kerenl_debug;

/**
 * Initializes the kernel. Must be called first
 */
bool fos_kernel_init(void);

/**
 * Runs the kernel
 */
bool fos_kernel_run(void);

/**
 * Signals the kernel that an systick has occurred and its time to reschedule
 */
void fos_kernel_systick(void);

/**
 * Adds a task to the system.
 *
 * @param entry    The entry point to the task code
 * @param context  A context that will be passed to the method
 */
unsigned int fos_kernel_add_task(void* stack, size_t stack_size, fos_task_entry_t entry, void* context);

/**
 * Returns the current systick clock passed since the kernel started
 */
tick_t fos_kernel_get_systicks(void);

/**
 * Get the current running task ID.
 */
unsigned int fos_kernel_get_current_task_id(void);

/**
 * Called from the port module when a task is reached to the end.
 *
 * @note called from the userspace using the task own stack.
 */
void fos_kernel_end_of_task(void);

/**
 * Sets the function that will be called when executing the systick
 * interrupt.
 *
 * The method will be called from interrupt context!
 */
void fos_kernel_set_systick_hook(void (*)(unsigned int tick));

/**
 * Override this method to change the implementation of the scheduler.
 */
__attribute__((weak)) unsigned int fos_kernel_schedule_get_next_task(void);

#ifdef __cplusplus
}
#endif
