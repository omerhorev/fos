#include <stdint.h>
#include <stdio.h>

#include "cmsis_device.h"
#include "trace.h"
#include "fos/fos.h"
#include "fos/mutex.h"
#include "fos/semaphore.h"

using namespace fos;

// TODO: Figure out the why the kernel stack got corrupted.
semaphore m;

int main()
{
	SystemInit();

	os& os = os::instance();

	os.add_task(new uint8_t[1000], 1000, []()
	{
		// Idle task is needed because otherwise we sometimes have nothing to run.
		for (;;)
			;
	}, 1);

	os.add_task(new uint8_t[1000], 1000, []()
	{
		trace_printf("Starting the system...\n");

		for (;;)
		{
			if (os::instance().get_systicks() >= 6000)
			{
				break;
			}
		}

		trace_printf("Stopping the high priority task\n");

	}, 5);

	os.add_task(new uint8_t[4000], 4000, []()
	{
		for(;;)
		{
			m.take();

			trace_printf("a second passed [%lu]\n", os::instance().get_systicks());
		}
	}, 2);

	os.add_task(new uint8_t[4000], 4000, []()
	{
		for (;;)
		{
			// TODO: Figure out why our clock suddenly doubled itself
			os::instance().sleep(2000);
			m.give();
		}
	}, 4);

	os.run();
}


