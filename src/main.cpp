#include <stdint.h>
#include <stdio.h>
#include "cmsis_device.h"
#include "trace.h"
#include "fos/kernel.h"

unsigned int g_ticks;

int clock(unsigned int times);
void first_task(void* context);

int main()
{
	g_ticks = 0;

	SystemInit();
	if (SysTick_Config(SystemCoreClock / 1000) != 0) // every ms
	{
		trace_err_printf("Error initializing sys-tick clock\n");
		return 1;
	}

	if (!fos_kernel_add_task(first_task, (void*)5))
	{
		trace_err_printf("Error initializing the kernel\n");
		return 1;
	}

	fos_kernel_run();

	return 0;
}

void first_task(void* context)
{
	clock((unsigned int)context);
}

int clock(unsigned int times)
{
	unsigned int sec = 0;

	while (sec < times)
	{
		if (sec != (g_ticks / 1000))
		{
			sec = (g_ticks / 1000);
			trace_printf("A second has passed [%u]\n", sec);
		}
	}

	return 0;
}

extern "C" __attribute__((isr)) void SysTick_Handler(void)
{
	g_ticks++;
}

