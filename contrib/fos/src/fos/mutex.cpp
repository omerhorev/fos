#include <vector>

#include "fos/fos.h"
#include "fos/mutex.h"
#include "kernel/kernel.h"
#include "kernel/kernel-internal.h"


using namespace fos;

void mutex::lock()
{
	while(true)
	{
		if (!_is_locked)
		{
			take_control();

			break;
		}
		else
		{
			// The mutex is locked, we need to mark that the task needs to sleep
			// and add ourself to the waiting vector

			taskid_t id = os::instance().get_current_task_id();

			fos_tcb* tcb = fos_kernel_get_tcb(id);

			if (tcb == NULL)
			{
				return;
			}

			_waiting_tasks.push(id);
			tcb->status = fos_kernel_task_status_waiting;

			while (_is_locked)
			{
				;;
			}
		}
	}
}

void mutex::unlock()
{
	_is_locked = false;

	while (!_waiting_tasks.empty())
	{
		fos_tcb* tcb = fos_kernel_get_tcb(_waiting_tasks.front());

		if (tcb == NULL)
		{
			return;
		}

		tcb->status = fos_kernel_task_status_ready;
		_waiting_tasks.pop();
	}
}

void mutex::take_control()
{
	_owner = os::instance().get_current_task_id();
	_is_locked = true;
}
