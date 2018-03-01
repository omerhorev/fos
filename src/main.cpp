#include <stdint.h>
#include <stdio.h>
#include "cmsis_device.h"
#include "trace.h"

unsigned int g_ticks;

int clock();

int main()
{
	g_ticks = 0;

	SystemInit();
	SysTick_Config(SystemCoreClock / 1000); // every ms

	trace_printf("hello world from main\n");
	trace_printf("the clock is %lu cycles per second\n", SystemCoreClock);

	clock();

	return 0;
}

int clock()
{
	unsigned int sec = 0;

	while (sec < 5)
	{
		if (sec != (g_ticks / 1000))
		{
			sec = (g_ticks / 1000);
			trace_printf("A second has passed [%u]\n", sec);
		}
	}
}

extern "C" __attribute__((isr)) void SysTick_Handler(void)
{
	g_ticks++;
}

