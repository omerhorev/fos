#pragma once

#include <queue>

#include "fos/fos-general.h"

namespace fos
{
	class mutex
	{
	public:

		mutex() : _is_locked(false) {}

		void lock() noexcept;
		void unlock() noexcept;

	private:

		void take_control();

	private:

		taskid_t _owner;
		bool _is_locked;
		std::queue<taskid_t> _waiting_tasks;
	};
}
