#include "fos/fos.h"
#include "kernel/kernel.h"

#include "timewheel.h"

using namespace fos;

static void generic_task_entry(void*);

os::os()
{
	fos_kernel_init();
}

void os::run()
{
	fos_kernel_run();
}

bool os::add_task(void* stack, size_t stack_size, std::function<void()> entry)
{
	return fos_kernel_add_task(stack, stack_size, generic_task_entry, &entry);
}


tick_t os::get_systicks() const
{
	return fos_kernel_get_systicks();
}

taskid_t os::get_current_task_id() const
{
	return fos_kernel_get_current_task_id();
}

static void generic_task_entry(void* context)
{
	std::function<void()>& entry = *(std::function<void()>*)(context);

	entry();
}
