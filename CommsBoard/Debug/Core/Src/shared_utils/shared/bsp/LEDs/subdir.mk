################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/shared_utils/shared/bsp/LEDs/LEDManager.cpp 

OBJS += \
./Core/Src/shared_utils/shared/bsp/LEDs/LEDManager.o 

CPP_DEPS += \
./Core/Src/shared_utils/shared/bsp/LEDs/LEDManager.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/shared_utils/shared/bsp/LEDs/%.o Core/Src/shared_utils/shared/bsp/LEDs/%.su Core/Src/shared_utils/shared/bsp/LEDs/%.cyclo: ../Core/Src/shared_utils/shared/bsp/LEDs/%.cpp Core/Src/shared_utils/shared/bsp/LEDs/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DSTM32L4R9xx -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Chris/STM32CubeIDE/workspace_1.10.1/communication-board/CommsBoard/Core/Src/shared_utils/shared" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.10.1/communication-board/CommsBoard/Core/Src/shared_utils/shared/CANopenNode" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.10.1/communication-board/CommsBoard/Core/Src/device_specific" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall --specs=nosys.specs -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-shared_utils-2f-shared-2f-bsp-2f-LEDs

clean-Core-2f-Src-2f-shared_utils-2f-shared-2f-bsp-2f-LEDs:
	-$(RM) ./Core/Src/shared_utils/shared/bsp/LEDs/LEDManager.cyclo ./Core/Src/shared_utils/shared/bsp/LEDs/LEDManager.d ./Core/Src/shared_utils/shared/bsp/LEDs/LEDManager.o ./Core/Src/shared_utils/shared/bsp/LEDs/LEDManager.su

.PHONY: clean-Core-2f-Src-2f-shared_utils-2f-shared-2f-bsp-2f-LEDs

