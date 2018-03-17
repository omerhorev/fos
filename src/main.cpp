#include <stdint.h>
#include <stdio.h>

#include "cmsis_device.h"
#include "trace.h"
#include "fos/fos.h"
#include "fos/mutex.h"
#include "fos/semaphore.h"

using namespace fos;

int main()
{
	SystemInit();

	os& os = os::instance();

	semaphore m;

	os.add_task(new uint8_t[1000], 1000, []()
	{
		// Idle task is needed because otherwise we sometimes have nothing to run.
		for (;;)
			;
	});

	os.add_task(new uint8_t[4000], 4000, [&m]()
	{
		for(;;)
		{
			m.take();

			trace_printf("a second passed\n");
		}
	});

	os.add_task(new uint8_t[4000], 4000, [&m]()
	{
		for (;;)
		{
			// TODO: Figure out why our clock suddenly doubled itself
			os::instance().sleep(2000);
			m.give();
		}
	});

	os.run();
}


