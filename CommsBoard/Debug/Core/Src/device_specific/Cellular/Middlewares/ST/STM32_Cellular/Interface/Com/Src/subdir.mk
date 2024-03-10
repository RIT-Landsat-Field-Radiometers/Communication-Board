################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/com_core.c \
../Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/com_icc.c \
../Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/com_sockets.c \
../Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/com_sockets_err_compat.c \
../Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/com_sockets_ip_modem.c \
../Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/com_sockets_statistic.c \
../Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/com_utils.c 

C_DEPS += \
./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/com_core.d \
./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/com_icc.d \
./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/com_sockets.d \
./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/com_sockets_err_compat.d \
./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/com_sockets_ip_modem.d \
./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/com_sockets_statistic.d \
./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/com_utils.d 

OBJS += \
./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/com_core.o \
./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/com_icc.o \
./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/com_sockets.o \
./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/com_sockets_err_compat.o \
./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/com_sockets_ip_modem.o \
./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/com_sockets_statistic.o \
./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/com_utils.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/%.o Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/%.su: ../Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/%.c Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32L4R9xx -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"D:/ProgramsOnHDD/DIRS/TestDirectory/communication-board/CommsBoard-backupUpdate/Core/Src/shared_utils/shared" -I"D:/ProgramsOnHDD/DIRS/TestDirectory/communication-board/CommsBoard-backupUpdate/Core/Src/shared_utils/shared/CANopenNode" -I"D:/ProgramsOnHDD/DIRS/TestDirectory/communication-board/CommsBoard-backupUpdate/Core/Src/device_specific" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-device_specific-2f-Cellular-2f-Middlewares-2f-ST-2f-STM32_Cellular-2f-Interface-2f-Com-2f-Src

clean-Core-2f-Src-2f-device_specific-2f-Cellular-2f-Middlewares-2f-ST-2f-STM32_Cellular-2f-Interface-2f-Com-2f-Src:
	-$(RM) ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/com_core.d ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/com_core.o ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/com_core.su ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/com_icc.d ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/com_icc.o ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/com_icc.su ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/com_sockets.d ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/com_sockets.o ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/com_sockets.su ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/com_sockets_err_compat.d ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/com_sockets_err_compat.o ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/com_sockets_err_compat.su ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/com_sockets_ip_modem.d ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/com_sockets_ip_modem.o ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/com_sockets_ip_modem.su ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/com_sockets_statistic.d ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/com_sockets_statistic.o ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/com_sockets_statistic.su ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/com_utils.d ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/com_utils.o ./Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/com_utils.su

.PHONY: clean-Core-2f-Src-2f-device_specific-2f-Cellular-2f-Middlewares-2f-ST-2f-STM32_Cellular-2f-Interface-2f-Com-2f-Src

