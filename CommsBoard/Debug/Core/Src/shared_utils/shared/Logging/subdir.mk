################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/shared_utils/shared/Logging/printf.c 

CPP_SRCS += \
../Core/Src/shared_utils/shared/Logging/logging.cpp 

C_DEPS += \
./Core/Src/shared_utils/shared/Logging/printf.d 

OBJS += \
./Core/Src/shared_utils/shared/Logging/logging.o \
./Core/Src/shared_utils/shared/Logging/printf.o 

CPP_DEPS += \
./Core/Src/shared_utils/shared/Logging/logging.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/shared_utils/shared/Logging/%.o Core/Src/shared_utils/shared/Logging/%.su: ../Core/Src/shared_utils/shared/Logging/%.cpp Core/Src/shared_utils/shared/Logging/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DSTM32L4R9xx -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"D:/ProgramsOnHDD/DIRS/TestDirectory/communication-board/CommsBoard-backupUpdate/Core/Src/shared_utils/shared" -I"D:/ProgramsOnHDD/DIRS/TestDirectory/communication-board/CommsBoard-backupUpdate/Core/Src/shared_utils/shared/CANopenNode" -I"D:/ProgramsOnHDD/DIRS/TestDirectory/communication-board/CommsBoard-backupUpdate/Core/Src/device_specific" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall --specs=nosys.specs -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/shared_utils/shared/Logging/%.o Core/Src/shared_utils/shared/Logging/%.su: ../Core/Src/shared_utils/shared/Logging/%.c Core/Src/shared_utils/shared/Logging/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32L4R9xx -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"D:/ProgramsOnHDD/DIRS/TestDirectory/communication-board/CommsBoard-backupUpdate/Core/Src/shared_utils/shared" -I"D:/ProgramsOnHDD/DIRS/TestDirectory/communication-board/CommsBoard-backupUpdate/Core/Src/shared_utils/shared/CANopenNode" -I"D:/ProgramsOnHDD/DIRS/TestDirectory/communication-board/CommsBoard-backupUpdate/Core/Src/device_specific" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-shared_utils-2f-shared-2f-Logging

clean-Core-2f-Src-2f-shared_utils-2f-shared-2f-Logging:
	-$(RM) ./Core/Src/shared_utils/shared/Logging/logging.d ./Core/Src/shared_utils/shared/Logging/logging.o ./Core/Src/shared_utils/shared/Logging/logging.su ./Core/Src/shared_utils/shared/Logging/printf.d ./Core/Src/shared_utils/shared/Logging/printf.o ./Core/Src/shared_utils/shared/Logging/printf.su

.PHONY: clean-Core-2f-Src-2f-shared_utils-2f-shared-2f-Logging

