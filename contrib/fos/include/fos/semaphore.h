#pragma once

#include <stdexcept>
#include "fos/mutex.h"

namespace fos
{
	/**
	 * Semaphore is used to signal the availability of resources.
	 */
	class semaphore
	{
	public:

		/**
		 * Initializes a new instance of a semaphore.
		 *
		 * The amount of resources will start at 0.
		 */
		semaphore() : semaphore(0) { }

		/**
		 * Initializes a new instance of a semaphore.
		 *
		 * @param count The amount of resources to start with.
		 */
		semaphore(unsigned int count) : _count(count) { }

		/**
		 * Takes a resource.
		 *
		 * When the amount of resources reaches 0, This function will be blocking
		 * until a new resource will be provided.
		 */
		inline void take()
		{
			if (_count == 0)
			{
				// No more resources to take, wait until a new resource will arrive
				_mutex.lock();
			}

			_count--;
		}

		/**
		 * Gives a new resource.
		 *
		 * If the amount of resources was 0, any tasks waiting for resources will be released.
		 */
		inline void give()
		{
			if (_count == 0)
			{
				// Need to wake up all the waiting tasks
				_mutex.unlock();
			}

			_count++;
		}

		/**
		 * Returns a amount of resources available in the semaphore
		 */
		inline unsigned int count() const
		{
			return _count;
		}

	private:
		unsigned int _count;
		fos::mutex _mutex;
	};
}
