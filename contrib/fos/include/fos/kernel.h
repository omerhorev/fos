#pragma once
/**
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "kernel-data.h"

#define FOS_KERNEL_MAX_TASKS (5)

/**
 * Initializes the kernel. Must be called first
 */
bool fos_kernel_init();

/**
 * Runs the kernel
 */
bool fos_kernel_run();

/**
 * Adds a task to the system.
 *
 * @param entry    The entry point to the task code
 * @param contenxt A context that will be passed to the method
 */
bool fos_kernel_add_task(fos_task_entry_t entry, void* context);

#ifdef __cplusplus
}
#endif
