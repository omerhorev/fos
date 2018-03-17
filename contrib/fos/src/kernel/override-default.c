#include "kernel/kernel.h"
#include "kernel/kernel-internal.h"
#include "kernel/kernel-data.h"

#include "fos/options.h"

__attribute__((weak)) unsigned int fos_override_get_next_task(void)
{
	unsigned int new_task_id = fos_kernel_get_current_task_id();

	do
	{
		new_task_id = (new_task_id + 1) % FOS_KERNEL_MAX_TASKS;
	} while(fos_kernel_get_tcb(new_task_id)->status != fos_kernel_task_status_ready);

	return new_task_id;
}

