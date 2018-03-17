#pragma once

namespace fos {
namespace internal
{
	/**
	 * This class holds all the extra data saved by the parts of the os
	 * that are not the kernel.
	 */
	class task_extra_data
	{
		std::function<void()> entry;
	};
}
}
