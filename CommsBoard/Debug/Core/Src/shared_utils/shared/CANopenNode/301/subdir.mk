################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/shared_utils/shared/CANopenNode/301/CO_Emergency.c \
../Core/Src/shared_utils/shared/CANopenNode/301/CO_HBconsumer.c \
../Core/Src/shared_utils/shared/CANopenNode/301/CO_NMT_Heartbeat.c \
../Core/Src/shared_utils/shared/CANopenNode/301/CO_ODinterface.c \
../Core/Src/shared_utils/shared/CANopenNode/301/CO_PDO.c \
../Core/Src/shared_utils/shared/CANopenNode/301/CO_SDOclient.c \
../Core/Src/shared_utils/shared/CANopenNode/301/CO_SDOserver.c \
../Core/Src/shared_utils/shared/CANopenNode/301/CO_SYNC.c \
../Core/Src/shared_utils/shared/CANopenNode/301/CO_TIME.c \
../Core/Src/shared_utils/shared/CANopenNode/301/CO_fifo.c \
../Core/Src/shared_utils/shared/CANopenNode/301/crc16-ccitt.c 

C_DEPS += \
./Core/Src/shared_utils/shared/CANopenNode/301/CO_Emergency.d \
./Core/Src/shared_utils/shared/CANopenNode/301/CO_HBconsumer.d \
./Core/Src/shared_utils/shared/CANopenNode/301/CO_NMT_Heartbeat.d \
./Core/Src/shared_utils/shared/CANopenNode/301/CO_ODinterface.d \
./Core/Src/shared_utils/shared/CANopenNode/301/CO_PDO.d \
./Core/Src/shared_utils/shared/CANopenNode/301/CO_SDOclient.d \
./Core/Src/shared_utils/shared/CANopenNode/301/CO_SDOserver.d \
./Core/Src/shared_utils/shared/CANopenNode/301/CO_SYNC.d \
./Core/Src/shared_utils/shared/CANopenNode/301/CO_TIME.d \
./Core/Src/shared_utils/shared/CANopenNode/301/CO_fifo.d \
./Core/Src/shared_utils/shared/CANopenNode/301/crc16-ccitt.d 

OBJS += \
./Core/Src/shared_utils/shared/CANopenNode/301/CO_Emergency.o \
./Core/Src/shared_utils/shared/CANopenNode/301/CO_HBconsumer.o \
./Core/Src/shared_utils/shared/CANopenNode/301/CO_NMT_Heartbeat.o \
./Core/Src/shared_utils/shared/CANopenNode/301/CO_ODinterface.o \
./Core/Src/shared_utils/shared/CANopenNode/301/CO_PDO.o \
./Core/Src/shared_utils/shared/CANopenNode/301/CO_SDOclient.o \
./Core/Src/shared_utils/shared/CANopenNode/301/CO_SDOserver.o \
./Core/Src/shared_utils/shared/CANopenNode/301/CO_SYNC.o \
./Core/Src/shared_utils/shared/CANopenNode/301/CO_TIME.o \
./Core/Src/shared_utils/shared/CANopenNode/301/CO_fifo.o \
./Core/Src/shared_utils/shared/CANopenNode/301/crc16-ccitt.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/shared_utils/shared/CANopenNode/301/%.o Core/Src/shared_utils/shared/CANopenNode/301/%.su: ../Core/Src/shared_utils/shared/CANopenNode/301/%.c Core/Src/shared_utils/shared/CANopenNode/301/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32L4R9xx -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"D:/ProgramsOnHDD/DIRS/TestDirectory/communication-board/CommsBoard-backupUpdate/Core/Src/shared_utils/shared" -I"D:/ProgramsOnHDD/DIRS/TestDirectory/communication-board/CommsBoard-backupUpdate/Core/Src/shared_utils/shared/CANopenNode" -I"D:/ProgramsOnHDD/DIRS/TestDirectory/communication-board/CommsBoard-backupUpdate/Core/Src/device_specific" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-shared_utils-2f-shared-2f-CANopenNode-2f-301

clean-Core-2f-Src-2f-shared_utils-2f-shared-2f-CANopenNode-2f-301:
	-$(RM) ./Core/Src/shared_utils/shared/CANopenNode/301/CO_Emergency.d ./Core/Src/shared_utils/shared/CANopenNode/301/CO_Emergency.o ./Core/Src/shared_utils/shared/CANopenNode/301/CO_Emergency.su ./Core/Src/shared_utils/shared/CANopenNode/301/CO_HBconsumer.d ./Core/Src/shared_utils/shared/CANopenNode/301/CO_HBconsumer.o ./Core/Src/shared_utils/shared/CANopenNode/301/CO_HBconsumer.su ./Core/Src/shared_utils/shared/CANopenNode/301/CO_NMT_Heartbeat.d ./Core/Src/shared_utils/shared/CANopenNode/301/CO_NMT_Heartbeat.o ./Core/Src/shared_utils/shared/CANopenNode/301/CO_NMT_Heartbeat.su ./Core/Src/shared_utils/shared/CANopenNode/301/CO_ODinterface.d ./Core/Src/shared_utils/shared/CANopenNode/301/CO_ODinterface.o ./Core/Src/shared_utils/shared/CANopenNode/301/CO_ODinterface.su ./Core/Src/shared_utils/shared/CANopenNode/301/CO_PDO.d ./Core/Src/shared_utils/shared/CANopenNode/301/CO_PDO.o ./Core/Src/shared_utils/shared/CANopenNode/301/CO_PDO.su ./Core/Src/shared_utils/shared/CANopenNode/301/CO_SDOclient.d ./Core/Src/shared_utils/shared/CANopenNode/301/CO_SDOclient.o ./Core/Src/shared_utils/shared/CANopenNode/301/CO_SDOclient.su ./Core/Src/shared_utils/shared/CANopenNode/301/CO_SDOserver.d ./Core/Src/shared_utils/shared/CANopenNode/301/CO_SDOserver.o ./Core/Src/shared_utils/shared/CANopenNode/301/CO_SDOserver.su ./Core/Src/shared_utils/shared/CANopenNode/301/CO_SYNC.d ./Core/Src/shared_utils/shared/CANopenNode/301/CO_SYNC.o ./Core/Src/shared_utils/shared/CANopenNode/301/CO_SYNC.su ./Core/Src/shared_utils/shared/CANopenNode/301/CO_TIME.d ./Core/Src/shared_utils/shared/CANopenNode/301/CO_TIME.o ./Core/Src/shared_utils/shared/CANopenNode/301/CO_TIME.su ./Core/Src/shared_utils/shared/CANopenNode/301/CO_fifo.d ./Core/Src/shared_utils/shared/CANopenNode/301/CO_fifo.o ./Core/Src/shared_utils/shared/CANopenNode/301/CO_fifo.su ./Core/Src/shared_utils/shared/CANopenNode/301/crc16-ccitt.d ./Core/Src/shared_utils/shared/CANopenNode/301/crc16-ccitt.o ./Core/Src/shared_utils/shared/CANopenNode/301/crc16-ccitt.su

.PHONY: clean-Core-2f-Src-2f-shared_utils-2f-shared-2f-CANopenNode-2f-301

