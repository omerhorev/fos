################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../contrib/fos/src/kernel.c \
../contrib/fos/src/port_c.c 

S_UPPER_SRCS += \
../contrib/fos/src/port_asm.S 

OBJS += \
./contrib/fos/src/kernel.o \
./contrib/fos/src/port_asm.o \
./contrib/fos/src/port_c.o 

S_UPPER_DEPS += \
./contrib/fos/src/port_asm.d 

C_DEPS += \
./contrib/fos/src/kernel.d \
./contrib/fos/src/port_c.d 


# Each subdirectory must supply rules for building sources it contributes
contrib/fos/src/%.o: ../contrib/fos/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DOS_USE_SEMIHOSTING -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f1-stdperiph" -I../contrib/fos/include -std=gnu99 -Wmissing-prototypes -Wstrict-prototypes -Wbad-function-cast -fshort-wchar -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

contrib/fos/src/%.o: ../contrib/fos/src/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-move-loop-invariants -Wall -Wextra  -g3 -x assembler-with-cpp -DDEBUG -DUSE_FULL_ASSERT -DOS_USE_SEMIHOSTING -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f1-stdperiph" -I../contrib/fos/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


