#pragma once

#include <functional>

#include "fos/options.h"
#include "fos/fos-general.h"
#include "fos/internal/internal-structures.h"
#include "fos/internal/timewheel.h"

namespace fos
{
	class os
	{
	public:

		/**
		 * Returns a instance of the OS
		 */
		static os& instance()
		{
			static os instance;
			return instance;
		}

		/**
		 * Start running the OS.
		 * if successful, this function will not return.
		 */
		void run();

		/**
		 * Adds a new task to the OS
		 *
		 * @param stack      A buffer that will be used as the stack
		 * @param stack_size The size of the buffer allocated
		 * @param entry      The method that will be called when running the task.
		 */
		bool add_task(void* stack, size_t stack_size, std::function<void()> entry);

		/**
		 * Get the current systicks of the system.
		 */
		tick_t get_systicks() const;

		/**
		 * Returns the task id of the currently running task
		 */
		taskid_t get_current_task_id() const;

		/**
		 * Sleep for a certain amount of ticks
		 */
		void sleep(tick_t ticks);

	private:

		os();

		void systick_hook(tick_t tick);

		taskid_t get_next_task();

		static void systick_hook_kernel(unsigned int);

	private:

		fos::internal::task_extra_data _tasks_extra_data[FOS_KERNEL_MAX_TASKS];

		fos::internal::timewheel _timewheel;

		friend unsigned int override_get_next_task(void);
	};
}
