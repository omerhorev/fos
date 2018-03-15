#include "fos/kernel.h"
#include "fos/kernel-data.h"
#include "kernel-internal.h"
#include "port.h"
#include "cmsis_device.h"

#include <stdbool.h>
#include <stddef.h>

// ------------------------ public globals ------------------------
size_t g_fos_task_count;
struct fos_tcb g_fos_tasks[FOS_KERNEL_MAX_TASKS];
struct fos_tcb *g_fos_cur_task;

// ------------------------ private globals ------------------------
static tick_t g_fos_kernel_ticks;
static enum kernel_state g_fos_kernel_state = kerenl_state_uninitialized;

// ------------------------ private methods  -----------------------
static bool is_kernel_initialized(void);
static bool is_kernel_running(void);

// ------------------------ implementations ------------------------
bool fos_kernel_init()
{
	g_fos_kernel_ticks = 0;
	g_fos_task_count = 0;

	g_fos_kernel_state = kerenl_state_initialized;

	for (size_t i = 0; i < FOS_KERNEL_MAX_TASKS; i++)
	{
		g_fos_tasks[i].status = fos_kernel_task_status_uninitialzied;
	}

	fos_port_setup_interrupts();

	return true;
}

/**
 * Runs the kernel
 */
bool fos_kernel_run()
{
	if (!is_kernel_initialized())
	{
		return false;
	}

	g_fos_cur_task = &g_fos_tasks[0];
	g_fos_cur_task->status = fos_kernel_task_status_running;
	g_fos_kernel_state = kerenl_state_running;

	fos_port_run_first_task();

	for (;;)
		;

	return true;
}

bool fos_kernel_add_task(void* stack, size_t stack_size, fos_task_entry_t entry, const void* context)
{
	struct fos_tcb* task;
	regval_t sp;

	if (!is_kernel_initialized())
	{
		return false;
	}

	if (stack            == NULL                    ||
		stack_size       <= FOS_PORT_MIN_STACK_SIZE ||
		g_fos_task_count >= FOS_KERNEL_MAX_TASKS)
	{
		return false;
	}

	task = &g_fos_tasks[g_fos_task_count];

	// The stack goes backwards, from high to low
	sp = ((regval_t)stack) + stack_size - 1;
	sp = fos_port_init_task_stack(sp, entry, context);

	task->sp 		 = sp;
	task->stack 	 = stack;
	task->stack_size = stack_size;
	task->id 		 = g_fos_task_count;
	task->status     = fos_kernel_task_status_ready;

	g_fos_task_count++;

	return true;
}

void fos_kernel_schedule(void)
{
	unsigned int new_task_id = g_fos_cur_task->id;

	if (g_fos_cur_task->status == fos_kernel_task_status_running)
	{
		//
		// We did a context switch from a task that was running,
		// meaning that its ready to be executed again.
		//
		// The other option is that the status was "waiting", in that case,
		// the status should remain as waiting
		//
		g_fos_cur_task->status = fos_kernel_task_status_ready;
	}

	do
	{
		new_task_id = (new_task_id + 1) % FOS_KERNEL_MAX_TASKS;
	} while(g_fos_tasks[new_task_id].status != fos_kernel_task_status_ready);

	g_fos_cur_task = &g_fos_tasks[new_task_id];
	g_fos_cur_task->status = fos_kernel_task_status_running;
}

tick_t fos_kernel_get_systicks(void)
{
	return g_fos_kernel_ticks;
}

unsigned int fos_kernel_get_current_task_id(void)
{
	return g_fos_cur_task->id;
}

void fos_kernel_end_of_task(void)
{
	g_fos_cur_task->status = fos_kernel_task_status_done;

	//
	// This code will keep on executing until the next rescheduling.
	//
	for (;;)
		;
}

static inline bool is_kernel_initialized(void)
{
	return g_fos_kernel_state != kerenl_state_uninitialized;
}

static inline bool is_kernel_running(void)
{
	return g_fos_kernel_state == kerenl_state_running;
}

static inline bool is_need_to_schedule(unsigned int ticks)
{
	return (ticks % FOS_KERNEL_TICKS_PER_SCHEDULE) == 0;
}

__attribute__((isr, unused)) void SysTick_Handler(void);
__attribute__((isr, unused)) void SysTick_Handler(void)
{
	if (!is_kernel_running())
	{
		return;
	}

	g_fos_kernel_ticks++;

	if (is_need_to_schedule(g_fos_kernel_ticks))
	{
		// TODO: Fix interrupt priorities
		// NOTE: We didn't set any interrupt priorities so when we call the PendSV
		// it will be executed immediately.
		SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;

		// NOTE: because of interrupt priority, no mode code after this point is allowed.
		// the SysTick interrupt needs to be finished prior to the PendSV interrupt.
	}
}

