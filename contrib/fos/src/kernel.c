#include "fos/kernel.h"
#include "fos/kernel-data.h"
#include "kernel-internal.h"

#include <stdbool.h>
#include <stddef.h>

size_t g_fos_task_count;
struct fos_tcb g_fos_tasks[FOS_KERNEL_MAX_TASKS];

bool fos_kernel_init()
{
	g_fos_task_count = 0;

	return true;
}

/**
 * Runs the kernel
 */
bool fos_kernel_run()
{
	g_fos_tasks[0].entry(g_fos_tasks[0].context);

	return true;
}

bool fos_kernel_add_task(fos_task_entry_t entry, void* context)
{
	if (g_fos_task_count >= FOS_KERNEL_MAX_TASKS)
	{
		return false;
	}

	g_fos_tasks[g_fos_task_count].entry = entry;
	g_fos_tasks[g_fos_task_count].context = context;
	g_fos_task_count++;

	return true;
}
