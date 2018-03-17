#pragma once

#include <queue>
#include "fos/fos-general.h"

namespace fos
{
	/**
	 * mutual exclusion. Used to synchronize between to threads/tasks
	 * so only once can access a certain resources.
	 */
	class mutex
	{
	public:

		/**
		 * Initializes a new mutux object
		 */
		mutex() : _owner(fos::INVALID_TASK), _is_locked(false) {}

		/**
		 * Locks the mutex for a specific critical section.
		 * Only one task can go pass this point after calling the function.
		 */
		void lock();

		/**
		 * Unlocks the mutex. All the waiting tasks will be released and a new task
		 * will take control of the critical sectioned code.
		 */
		void unlock();

	private:

		taskid_t _owner;
		bool _is_locked;
		std::queue<taskid_t> _waiting_tasks;
	};
}
