################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service.c \
../Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_cmd.c \
../Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_config.c \
../Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_datacache.c \
../Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_int.c \
../Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_os.c \
../Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_power.c \
../Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_task.c \
../Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_utils.c 

C_DEPS += \
./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service.d \
./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_cmd.d \
./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_config.d \
./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_datacache.d \
./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_int.d \
./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_os.d \
./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_power.d \
./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_task.d \
./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_utils.d 

OBJS += \
./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service.o \
./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_cmd.o \
./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_config.o \
./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_datacache.o \
./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_int.o \
./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_os.o \
./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_power.o \
./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_task.o \
./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_utils.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/%.o Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/%.su Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/%.cyclo: ../Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/%.c Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32L4R9xx -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Chris/STM32CubeIDE/workspace_1.10.1/communication-board/CommsBoard/Core/Src/shared_utils/shared" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.10.1/communication-board/CommsBoard/Core/Src/shared_utils/shared/CANopenNode" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.10.1/communication-board/CommsBoard/Core/Src/device_specific" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-device_specific-2f-Cellular-2f-Middlewares-2f-ST-2f-STM32_Cellular-2f-Core-2f-Cellular_Service-2f-Src

clean-Core-2f-Src-2f-device_specific-2f-Cellular-2f-Middlewares-2f-ST-2f-STM32_Cellular-2f-Core-2f-Cellular_Service-2f-Src:
	-$(RM) ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service.cyclo ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service.d ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service.o ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service.su ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_cmd.cyclo ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_cmd.d ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_cmd.o ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_cmd.su ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_config.cyclo ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_config.d ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_config.o ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_config.su ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_datacache.cyclo ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_datacache.d ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_datacache.o ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_datacache.su ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_int.cyclo ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_int.d ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_int.o ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_int.su ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_os.cyclo ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_os.d ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_os.o ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_os.su ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_power.cyclo ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_power.d ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_power.o ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_power.su ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_task.cyclo ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_task.d ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_task.o ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_task.su ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_utils.cyclo ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_utils.d ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_utils.o ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/cellular_service_utils.su

.PHONY: clean-Core-2f-Src-2f-device_specific-2f-Cellular-2f-Middlewares-2f-ST-2f-STM32_Cellular-2f-Core-2f-Cellular_Service-2f-Src

