#include <algorithm>
#include <array>

#include "fos/options.h"
#include "fos/fos-general.h"
#include "fos/internal/internal-structures.h"

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
		std::sort(_tasks.begin(), _tasks.end(), [](int a, int b) {
			return a > b;
		});
	}

	bool remove_task(taskid_t id);

	taskid_t get_next_task(tick_t current_ticks);

	bool halt_task(taskid_t id);

	bool resume_task(taskid_t id);

private:

	size_t count;
	std::vector<scheduler_tcb> _tasks;
};


}
}
