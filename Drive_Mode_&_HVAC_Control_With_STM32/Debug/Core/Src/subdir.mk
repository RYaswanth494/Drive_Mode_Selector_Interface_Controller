################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Blower.c \
../Core/Src/Debug.c \
../Core/Src/I2C.c \
../Core/Src/MCU_ELECTROCATALYST_MESSAGES.c \
../Core/Src/Motor_control_Pins.c \
../Core/Src/can.c \
../Core/Src/condensor.c \
../Core/Src/main.c \
../Core/Src/process.c \
../Core/Src/stm32f1xx_hal_msp.c \
../Core/Src/stm32f1xx_it.c \
../Core/Src/sys_tick.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_clock_init.c \
../Core/Src/system_stm32f1xx.c \
../Core/Src/task_schedular.c \
../Core/Src/uart.c 

OBJS += \
./Core/Src/Blower.o \
./Core/Src/Debug.o \
./Core/Src/I2C.o \
./Core/Src/MCU_ELECTROCATALYST_MESSAGES.o \
./Core/Src/Motor_control_Pins.o \
./Core/Src/can.o \
./Core/Src/condensor.o \
./Core/Src/main.o \
./Core/Src/process.o \
./Core/Src/stm32f1xx_hal_msp.o \
./Core/Src/stm32f1xx_it.o \
./Core/Src/sys_tick.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_clock_init.o \
./Core/Src/system_stm32f1xx.o \
./Core/Src/task_schedular.o \
./Core/Src/uart.o 

C_DEPS += \
./Core/Src/Blower.d \
./Core/Src/Debug.d \
./Core/Src/I2C.d \
./Core/Src/MCU_ELECTROCATALYST_MESSAGES.d \
./Core/Src/Motor_control_Pins.d \
./Core/Src/can.d \
./Core/Src/condensor.d \
./Core/Src/main.d \
./Core/Src/process.d \
./Core/Src/stm32f1xx_hal_msp.d \
./Core/Src/stm32f1xx_it.d \
./Core/Src/sys_tick.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_clock_init.d \
./Core/Src/system_stm32f1xx.d \
./Core/Src/task_schedular.d \
./Core/Src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/Blower.cyclo ./Core/Src/Blower.d ./Core/Src/Blower.o ./Core/Src/Blower.su ./Core/Src/Debug.cyclo ./Core/Src/Debug.d ./Core/Src/Debug.o ./Core/Src/Debug.su ./Core/Src/I2C.cyclo ./Core/Src/I2C.d ./Core/Src/I2C.o ./Core/Src/I2C.su ./Core/Src/MCU_ELECTROCATALYST_MESSAGES.cyclo ./Core/Src/MCU_ELECTROCATALYST_MESSAGES.d ./Core/Src/MCU_ELECTROCATALYST_MESSAGES.o ./Core/Src/MCU_ELECTROCATALYST_MESSAGES.su ./Core/Src/Motor_control_Pins.cyclo ./Core/Src/Motor_control_Pins.d ./Core/Src/Motor_control_Pins.o ./Core/Src/Motor_control_Pins.su ./Core/Src/can.cyclo ./Core/Src/can.d ./Core/Src/can.o ./Core/Src/can.su ./Core/Src/condensor.cyclo ./Core/Src/condensor.d ./Core/Src/condensor.o ./Core/Src/condensor.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/process.cyclo ./Core/Src/process.d ./Core/Src/process.o ./Core/Src/process.su ./Core/Src/stm32f1xx_hal_msp.cyclo ./Core/Src/stm32f1xx_hal_msp.d ./Core/Src/stm32f1xx_hal_msp.o ./Core/Src/stm32f1xx_hal_msp.su ./Core/Src/stm32f1xx_it.cyclo ./Core/Src/stm32f1xx_it.d ./Core/Src/stm32f1xx_it.o ./Core/Src/stm32f1xx_it.su ./Core/Src/sys_tick.cyclo ./Core/Src/sys_tick.d ./Core/Src/sys_tick.o ./Core/Src/sys_tick.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_clock_init.cyclo ./Core/Src/system_clock_init.d ./Core/Src/system_clock_init.o ./Core/Src/system_clock_init.su ./Core/Src/system_stm32f1xx.cyclo ./Core/Src/system_stm32f1xx.d ./Core/Src/system_stm32f1xx.o ./Core/Src/system_stm32f1xx.su ./Core/Src/task_schedular.cyclo ./Core/Src/task_schedular.d ./Core/Src/task_schedular.o ./Core/Src/task_schedular.su ./Core/Src/uart.cyclo ./Core/Src/uart.d ./Core/Src/uart.o ./Core/Src/uart.su

.PHONY: clean-Core-2f-Src

