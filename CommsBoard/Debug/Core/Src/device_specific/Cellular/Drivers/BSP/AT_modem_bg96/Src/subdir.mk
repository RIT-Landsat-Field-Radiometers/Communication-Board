################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/device_specific/Cellular/Drivers/BSP/AT_modem_bg96/Src/at_custom_modem_api.c \
../Core/Src/device_specific/Cellular/Drivers/BSP/AT_modem_bg96/Src/at_custom_modem_signalling.c \
../Core/Src/device_specific/Cellular/Drivers/BSP/AT_modem_bg96/Src/at_custom_modem_socket.c \
../Core/Src/device_specific/Cellular/Drivers/BSP/AT_modem_bg96/Src/at_custom_modem_specific.c \
../Core/Src/device_specific/Cellular/Drivers/BSP/AT_modem_bg96/Src/sysctrl_specific.c 

C_DEPS += \
./Core/Src/device_specific/Cellular/Drivers/BSP/AT_modem_bg96/Src/at_custom_modem_api.d \
./Core/Src/device_specific/Cellular/Drivers/BSP/AT_modem_bg96/Src/at_custom_modem_signalling.d \
./Core/Src/device_specific/Cellular/Drivers/BSP/AT_modem_bg96/Src/at_custom_modem_socket.d \
./Core/Src/device_specific/Cellular/Drivers/BSP/AT_modem_bg96/Src/at_custom_modem_specific.d \
./Core/Src/device_specific/Cellular/Drivers/BSP/AT_modem_bg96/Src/sysctrl_specific.d 

OBJS += \
./Core/Src/device_specific/Cellular/Drivers/BSP/AT_modem_bg96/Src/at_custom_modem_api.o \
./Core/Src/device_specific/Cellular/Drivers/BSP/AT_modem_bg96/Src/at_custom_modem_signalling.o \
./Core/Src/device_specific/Cellular/Drivers/BSP/AT_modem_bg96/Src/at_custom_modem_socket.o \
./Core/Src/device_specific/Cellular/Drivers/BSP/AT_modem_bg96/Src/at_custom_modem_specific.o \
./Core/Src/device_specific/Cellular/Drivers/BSP/AT_modem_bg96/Src/sysctrl_specific.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/device_specific/Cellular/Drivers/BSP/AT_modem_bg96/Src/%.o Core/Src/device_specific/Cellular/Drivers/BSP/AT_modem_bg96/Src/%.su Core/Src/device_specific/Cellular/Drivers/BSP/AT_modem_bg96/Src/%.cyclo: ../Core/Src/device_specific/Cellular/Drivers/BSP/AT_modem_bg96/Src/%.c Core/Src/device_specific/Cellular/Drivers/BSP/AT_modem_bg96/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32L4R9xx -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Chris/STM32CubeIDE/workspace_1.10.1/communication-board/CommsBoard/Core/Src/shared_utils/shared" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.10.1/communication-board/CommsBoard/Core/Src/shared_utils/shared/CANopenNode" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.10.1/communication-board/CommsBoard/Core/Src/device_specific" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-device_specific-2f-Cellular-2f-Drivers-2f-BSP-2f-AT_modem_bg96-2f-Src

clean-Core-2f-Src-2f-device_specific-2f-Cellular-2f-Drivers-2f-BSP-2f-AT_modem_bg96-2f-Src:
	-$(RM) ./Core/Src/device_specific/Cellular/Drivers/BSP/AT_modem_bg96/Src/at_custom_modem_api.cyclo ./Core/Src/device_specific/Cellular/Drivers/BSP/AT_modem_bg96/Src/at_custom_modem_api.d ./Core/Src/device_specific/Cellular/Drivers/BSP/AT_modem_bg96/Src/at_custom_modem_api.o ./Core/Src/device_specific/Cellular/Drivers/BSP/AT_modem_bg96/Src/at_custom_modem_api.su ./Core/Src/device_specific/Cellular/Drivers/BSP/AT_modem_bg96/Src/at_custom_modem_signalling.cyclo ./Core/Src/device_specific/Cellular/Drivers/BSP/AT_modem_bg96/Src/at_custom_modem_signalling.d ./Core/Src/device_specific/Cellular/Drivers/BSP/AT_modem_bg96/Src/at_custom_modem_signalling.o ./Core/Src/device_specific/Cellular/Drivers/BSP/AT_modem_bg96/Src/at_custom_modem_signalling.su ./Core/Src/device_specific/Cellular/Drivers/BSP/AT_modem_bg96/Src/at_custom_modem_socket.cyclo ./Core/Src/device_specific/Cellular/Drivers/BSP/AT_modem_bg96/Src/at_custom_modem_socket.d ./Core/Src/device_specific/Cellular/Drivers/BSP/AT_modem_bg96/Src/at_custom_modem_socket.o ./Core/Src/device_specific/Cellular/Drivers/BSP/AT_modem_bg96/Src/at_custom_modem_socket.su ./Core/Src/device_specific/Cellular/Drivers/BSP/AT_modem_bg96/Src/at_custom_modem_specific.cyclo ./Core/Src/device_specific/Cellular/Drivers/BSP/AT_modem_bg96/Src/at_custom_modem_specific.d ./Core/Src/device_specific/Cellular/Drivers/BSP/AT_modem_bg96/Src/at_custom_modem_specific.o ./Core/Src/device_specific/Cellular/Drivers/BSP/AT_modem_bg96/Src/at_custom_modem_specific.su ./Core/Src/device_specific/Cellular/Drivers/BSP/AT_modem_bg96/Src/sysctrl_specific.cyclo ./Core/Src/device_specific/Cellular/Drivers/BSP/AT_modem_bg96/Src/sysctrl_specific.d ./Core/Src/device_specific/Cellular/Drivers/BSP/AT_modem_bg96/Src/sysctrl_specific.o ./Core/Src/device_specific/Cellular/Drivers/BSP/AT_modem_bg96/Src/sysctrl_specific.su

.PHONY: clean-Core-2f-Src-2f-device_specific-2f-Cellular-2f-Drivers-2f-BSP-2f-AT_modem_bg96-2f-Src

