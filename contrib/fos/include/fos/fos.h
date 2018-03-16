#pragma once

#include <functional>

#include "fos/options.h"
#include "fos/fos-general.h"

namespace fos
{
	/**
	 * This class holds all the extra data saved by the parts of the os
	 * that are not the kernel.
	 */
	class task_extra_data
	{
		std::function<void()> entry;
	};

	class os
	{
	public:
		static os& instance()
		{
			static os instance;
			return instance;
		}

		void run();

		bool add_task(void* stack, size_t stack_size, std::function<void()> entry);

		tick_t get_systicks() const;

		taskid_t get_current_task_id() const;


	private:

		os();

		task_extra_data _tasks_extra_data[FOS_KERNEL_MAX_TASKS];
	};
}
