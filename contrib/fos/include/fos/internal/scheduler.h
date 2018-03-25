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

/**
 * The metadata saved for each task in the scheduler
 */
struct scheduler_tcb
{
	scheduler_tcb() : id(INVALID_TASK), priority(0), last_tick(0)
	{}

	scheduler_tcb(taskid_t id, priority_t priority) : id(id), priority(priority), last_tick(0)
	{}

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

	/**
	 * Adds a new task to the scheduler
	 *
	 * @param id       The ID of the task
	 * @param priority The priority of the task, the bigger the more important.
	 */
	bool add_task(taskid_t id, priority_t priority)
	{
		_tasks.push_back(scheduler_tcb(id, priority));
		std::sort(_tasks.begin(), _tasks.end(), [](scheduler_tcb& task1, scheduler_tcb& task2) {
			return task1.priority > task2.priority;
		});

		return true;
	}

	/**
	 * Removes a task from the scheduler
	 *
	 * @param id The ID of the task to remove
	 */
	bool remove_task(taskid_t id)
	{
		_tasks.erase(std::remove_if(_tasks.begin(), _tasks.end(), [id](scheduler_tcb& task)
	    {
			return task.id == id;
	    }), _tasks.end());

		return true;
	}

	/**
	 * Calculates the next task for the kernel
	 *
	 * @param current_ticks The current ticks in the system.
	 * @return The next task id
	 */
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
