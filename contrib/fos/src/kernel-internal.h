#pragma once

#include "fos/kernel-data.h"

/**
 * task object - Task control block
 */
struct fos_tcb
{
	fos_task_entry_t entry;
	void* context;
};
