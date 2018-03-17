#pragma once

#include <cstdint>

namespace fos
{
	typedef std::uint32_t priority_t;
	typedef std::uint32_t taskid_t;
	typedef std::uint32_t tick_t;

	constexpr taskid_t INVALID_TASK = (taskid_t)0xFFFFFFFF;
}
