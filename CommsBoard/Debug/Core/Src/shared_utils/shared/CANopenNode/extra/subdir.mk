################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/shared_utils/shared/CANopenNode/extra/CO_trace.c 

C_DEPS += \
./Core/Src/shared_utils/shared/CANopenNode/extra/CO_trace.d 

OBJS += \
./Core/Src/shared_utils/shared/CANopenNode/extra/CO_trace.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/shared_utils/shared/CANopenNode/extra/%.o Core/Src/shared_utils/shared/CANopenNode/extra/%.su: ../Core/Src/shared_utils/shared/CANopenNode/extra/%.c Core/Src/shared_utils/shared/CANopenNode/extra/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32L4R9xx -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"D:/ProgramsOnHDD/DIRS/TestDirectory/communication-board/CommsBoard-backupUpdate/Core/Src/shared_utils/shared" -I"D:/ProgramsOnHDD/DIRS/TestDirectory/communication-board/CommsBoard-backupUpdate/Core/Src/shared_utils/shared/CANopenNode" -I"D:/ProgramsOnHDD/DIRS/TestDirectory/communication-board/CommsBoard-backupUpdate/Core/Src/device_specific" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-shared_utils-2f-shared-2f-CANopenNode-2f-extra

clean-Core-2f-Src-2f-shared_utils-2f-shared-2f-CANopenNode-2f-extra:
	-$(RM) ./Core/Src/shared_utils/shared/CANopenNode/extra/CO_trace.d ./Core/Src/shared_utils/shared/CANopenNode/extra/CO_trace.o ./Core/Src/shared_utils/shared/CANopenNode/extra/CO_trace.su

.PHONY: clean-Core-2f-Src-2f-shared_utils-2f-shared-2f-CANopenNode-2f-extra

