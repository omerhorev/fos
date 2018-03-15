#include <stdint.h>
#include <stdio.h>
#include "cmsis_device.h"
#include "trace.h"
#include "fos/kernel.h"

unsigned int g_ticks;

int clock(unsigned int times);
void clock_task(void* context);

uint8_t task_stack[2][400];

int main()
{
	g_ticks = 0;

	SystemInit();

	fos_kernel_init();

	if (SysTick_Config(SystemCoreClock / 1000) != 0) // every ms
	{
		trace_err_printf("Error initializing systick clock\n");
		return 1;
	}

	if (!fos_kernel_add_task(task_stack[0], sizeof(task_stack[0]), clock_task, (void*)5))
	{
		trace_err_printf("Error initializing the first task\n");
		return 1;
	}

	if (!fos_kernel_add_task(task_stack[1], sizeof(task_stack[1]), clock_task, (void*)10))
	{
		trace_err_printf("Error initializing the second task\n");
		return 1;
	}

	fos_kernel_run();

	return 0;
}

void clock_task(void* context)
{
	clock((unsigned int)context);
}

int clock(unsigned int times)
{
	tick_t sec = 0;

	while (sec < times)
	{
		if (sec != (fos_kernel_get_systicks() / 1000))
		{
			sec = (fos_kernel_get_systicks() / 1000);

			trace_printf("A second has passed [%lu] [%u]\n", sec, fos_kernel_get_current_task_id());
		}
	}

	return 0;
}
