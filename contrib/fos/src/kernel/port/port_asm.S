.syntax unified
.cpu    cortex-m3
.thumb
.thumb_func
.text

/***************************************************************************
 * void fos_port_run_first_task(void);                                     *
 ***************************************************************************/
.globl fos_port_run_first_task
fos_port_run_first_task:

    svc 0
    ISB                         // Flush the pipeline so the interrupt will happen here
    bx lr                       // Exit from the interrupt

/***************************************************************************
 * __attribute__((naked, isr)) void SVC_Handler(void)                      *
 ***************************************************************************/
.thumb_func
.globl SVC_Handler
SVC_Handler:
    ldr r12, =g_fos_cur_task    //
    ldr r12, [r12]              // r12 = g_fos_cur_tsk.sp; // Note: sp is the first member of the struct
    ldr r12, [r12]              //

                                //
    ldmia r12!, {r4 - r11, lr}  // Pop all the registers (r4-r11, lr) from the stack we build
    msr psp, r12                // The rest of the registers will be popped by the hardware
                                // at the end of the interrupt
                                //

    mov r14, #0xfffffffd        //
    bx r14                      // Exit the interrupt signaling that we are exiting to userspace
                                //

/***************************************************************************
 * __attribute__((naked, isr)) void PendSV_Handler(void)                   *
 ***************************************************************************/
.thumb_func
.globl PendSV_Handler
PendSV_Handler:
                                //
    mrs   r12,  psp             // r12 = psp;
    stmdb r12!, {r4 - r11, lr}  // r12 = push_sw_frame(r12);
                                //

    ldr r0, =g_fos_cur_task     //
    ldr r0, [r0]                // g_fos_cur_task->sp = r12;
    str r12, [r0]               //

                                //
    bl fos_kernel_schedule      // fos_kernel_schedule();
                                //

    ldr r0, =g_fos_cur_task     //
    ldr r0, [r0]                // r12 = g_fos_cur_task->sp;
    ldr r12, [r0]               //

                                //
    ldmia r12!, {r4 - r11, lr}  // r12 = pop_sw_frame(r12);
    msr   psp,  r12             // psp = r12;
                                //

    mov r14, #0xfffffffd        //
    bx r14                      // return;
                                //


