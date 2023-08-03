################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/device_specific/JSON/Generator/json-maker.c 

C_DEPS += \
./Core/Src/device_specific/JSON/Generator/json-maker.d 

OBJS += \
./Core/Src/device_specific/JSON/Generator/json-maker.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/device_specific/JSON/Generator/%.o Core/Src/device_specific/JSON/Generator/%.su Core/Src/device_specific/JSON/Generator/%.cyclo: ../Core/Src/device_specific/JSON/Generator/%.c Core/Src/device_specific/JSON/Generator/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32L4R9xx -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Chris/STM32CubeIDE/workspace_1.10.1/communication-board/CommsBoard/Core/Src/shared_utils/shared" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.10.1/communication-board/CommsBoard/Core/Src/shared_utils/shared/CANopenNode" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.10.1/communication-board/CommsBoard/Core/Src/device_specific" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-device_specific-2f-JSON-2f-Generator

clean-Core-2f-Src-2f-device_specific-2f-JSON-2f-Generator:
	-$(RM) ./Core/Src/device_specific/JSON/Generator/json-maker.cyclo ./Core/Src/device_specific/JSON/Generator/json-maker.d ./Core/Src/device_specific/JSON/Generator/json-maker.o ./Core/Src/device_specific/JSON/Generator/json-maker.su

.PHONY: clean-Core-2f-Src-2f-device_specific-2f-JSON-2f-Generator

