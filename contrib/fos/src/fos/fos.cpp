#include "fos/fos.h"
#include "fos/mutex.h"
#include "kernel/kernel.h"
#include "kernel/kernel-internal.h"
#include "fos/internal/scheduler.h"

using namespace fos;

static void generic_task_entry(void*);

os::os()
{
	fos_kernel_init();
	fos_kernel_set_systick_hook(systick_hook_kernel);
}

void os::run()
{
	fos_kernel_run();
}

bool os::add_task(void* stack, size_t stack_size, std::function<void()> entry, priority_t priority)
{
	taskid_t id = fos_kernel_add_task(stack, stack_size, generic_task_entry, &entry);

	if (id == INVALID_TASK)
	{
		return false;
	}

	if (!_scheduler.add_task(id, priority))
	{
		// Note: Currently not supporting removing tasks
		// TODO: Support removing tasks
		return false;
	}

	return true;
}


tick_t os::get_systicks() const
{
	return fos_kernel_get_systicks();
}

taskid_t os::get_current_task_id() const
{
	return fos_kernel_get_current_task_id();
}

void os::sleep(tick_t ticks)
{
	tick_t wakeup_ticks;
	mutex wakeup_mutex;

	// TODO: Fix the integer overflow
	wakeup_ticks = get_systicks() + ticks;

	wakeup_mutex.lock();

	_timewheel.push([&](__attribute__((unused)) tick_t ticks)
	{
		// NOTE: This lambda will be called from an interrupt context
		//       but the method above is called from the task context.
		//       we are passing the vars of the method to the lambda by reference, this is
		//       ok because the stack will still be here when the lambda will be called
		//       (the method will be stuck on the mutex)
		wakeup_mutex.unlock();

	}, wakeup_ticks);

	wakeup_mutex.lock();
}


void os::systick_hook(tick_t ticks)
{
	// @note This method is executed in the interrupt context

	while(_timewheel.empty() == false &&
		  _timewheel.top().ticks <= ticks)
	{
		fos::internal::timewheel_event event = _timewheel.top();

		event.dispach();

		_timewheel.pop();
	}
}

void os::systick_hook_kernel(unsigned int ticks)
{
	os::instance().systick_hook(ticks);
}

taskid_t os::get_next_task(void)
{
	return _scheduler.next(get_systicks());
}

static void generic_task_entry(void* context)
{
	std::function<void()>& entry = *(std::function<void()>*)(context);

	entry();
}

namespace fos
{
	unsigned int override_get_next_task(void)
	{
		return (unsigned int)os::instance().get_next_task();
	}
}

/**
 * Override the kernel simple implementation of scheduling
 */
extern "C" __attribute__((used)) unsigned int fos_override_get_next_task(void)
{
	return fos::override_get_next_task();
}

