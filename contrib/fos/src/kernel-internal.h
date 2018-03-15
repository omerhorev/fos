#pragma once

#include <stdint.h>
#include <stddef.h>
#include "fos/kernel-data.h"

#ifdef __cplusplus
extern "C" {
#endif

enum kernel_state
{
	kerenl_state_uninitialized,
	kerenl_state_initialized,
	kerenl_state_running
};

enum fos_kernel_task_status
{
	fos_kernel_task_status_uninitialzied,
	fos_kernel_task_status_ready,
	fos_kernel_task_status_running,
	fos_kernel_task_status_done
};

/**
 * task object - Task control block
 */
struct fos_tcb
{
	regval_t sp;
	void* stack;
	size_t stack_size;
	unsigned int id;
	enum fos_kernel_task_status status;
};


/**
 * Called from the port when the task has ended.
 * The method removes the task from the task list and resumes other tasks.
 */
void fos_kernel_end_of_task(void);

/**
 * Call the scheduler from the PendSV interrupt.
 * This method changes the g_fos_cur_task var.
 */
void fos_kernel_schedule(void);

#ifdef __cplusplus
}
#endif
