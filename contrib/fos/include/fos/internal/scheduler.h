#include <algorithm>
#include <array>

#include "fos/options.h"
#include "fos/fos-general.h"
#include "fos/internal/internal-structures.h"

#include "kernel/kernel-internal.h"

#pragma once

namespace fos
{
namespace internal
{

struct scheduler_tcb
{
	scheduler_tcb() : id(INVALID_TASK), priority(0), last_tick(0)
	{

	}

	scheduler_tcb(taskid_t id, priority_t priority) : id(id), priority(priority), last_tick(0)
	{

	}

	taskid_t id;
	priority_t priority;
	tick_t last_tick;
};

/**
 * Represents a scheduler with all the logic needed
 */
class scheduler
{
public:

	bool add_task(taskid_t id, priority_t priority)
	{
		_tasks.push_back(scheduler_tcb(id, priority));
		std::sort(_tasks.begin(), _tasks.end(), [](scheduler_tcb& task1, scheduler_tcb& task2) {
			return task1.priority > task2.priority;
		});

		return true;
	}

	bool remove_task(taskid_t id)
	{
		_tasks.erase(std::remove_if(_tasks.begin(), _tasks.end(), [id](scheduler_tcb& task)
	    {
			return task.id == id;
	    }), _tasks.end());

		return true;
	}

	taskid_t next(tick_t current_ticks)
	{
		for (auto& task : _tasks)
		{
			if (fos_kernel_get_tcb(task.id) != nullptr &&
				fos_kernel_get_tcb(task.id)->status == fos_kernel_task_status_ready)
			{
				if (_current_task != nullptr)
				{
					_current_task->last_tick = current_ticks;
				}

				_current_task = &task;

				return _current_task->id;
			}
		}

		return INVALID_TASK;
	}

private:

	scheduler_tcb* _current_task;
	std::vector<scheduler_tcb> _tasks;
};

}
}
