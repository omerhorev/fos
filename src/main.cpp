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
		while(true)
		{
			m.take();

			trace_printf("a second passed\n");
		}
	});

	os.add_task(new uint8_t[400], 400, [&]()
	{
		fos::tick_t sec = 0;

		while (sec < 5)
		{
			if (sec != (os::instance().get_systicks() / 1000))
			{
				sec = (os::instance().get_systicks() / 1000);

				m.give();
			}
		}

		return 0;
	});

	os.run();
}


