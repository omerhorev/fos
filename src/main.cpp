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

	os.add_task(new uint8_t[400], 400, [&]()
	{
		// Idle task is needed because otherwise we sometimes have nothing to run.
		for (;;)
			;
	});

	os.add_task(new uint8_t[2000], 2000, [&]()
	{
		while(true)
		{
			m.take();

			trace_printf("a second passed\n");
		}
	});

	os.add_task(new uint8_t[2000], 2000, [&]()
	{
		for (size_t i = 0; i < 5; i++)
		{
			// TODO: Firgure out why our clock suddenly doubled itself
			os.sleep(2000);
			m.give();
		}

		return 0;
	});

	os.run();
}


