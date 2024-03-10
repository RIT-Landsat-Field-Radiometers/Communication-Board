################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/device_specific/protob/hourly.pb.c \
../Core/Src/device_specific/protob/pb_common.c \
../Core/Src/device_specific/protob/pb_decode.c \
../Core/Src/device_specific/protob/pb_encode.c 

CPP_SRCS += \
../Core/Src/device_specific/protob/CompressorWriteBuffer.cpp 

C_DEPS += \
./Core/Src/device_specific/protob/hourly.pb.d \
./Core/Src/device_specific/protob/pb_common.d \
./Core/Src/device_specific/protob/pb_decode.d \
./Core/Src/device_specific/protob/pb_encode.d 

OBJS += \
./Core/Src/device_specific/protob/CompressorWriteBuffer.o \
./Core/Src/device_specific/protob/hourly.pb.o \
./Core/Src/device_specific/protob/pb_common.o \
./Core/Src/device_specific/protob/pb_decode.o \
./Core/Src/device_specific/protob/pb_encode.o 

CPP_DEPS += \
./Core/Src/device_specific/protob/CompressorWriteBuffer.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/device_specific/protob/%.o Core/Src/device_specific/protob/%.su: ../Core/Src/device_specific/protob/%.cpp Core/Src/device_specific/protob/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DSTM32L4R9xx -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"D:/ProgramsOnHDD/DIRS/TestDirectory/communication-board/CommsBoard-backupUpdate/Core/Src/shared_utils/shared" -I"D:/ProgramsOnHDD/DIRS/TestDirectory/communication-board/CommsBoard-backupUpdate/Core/Src/shared_utils/shared/CANopenNode" -I"D:/ProgramsOnHDD/DIRS/TestDirectory/communication-board/CommsBoard-backupUpdate/Core/Src/device_specific" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall --specs=nosys.specs -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/device_specific/protob/%.o Core/Src/device_specific/protob/%.su: ../Core/Src/device_specific/protob/%.c Core/Src/device_specific/protob/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32L4R9xx -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"D:/ProgramsOnHDD/DIRS/TestDirectory/communication-board/CommsBoard-backupUpdate/Core/Src/shared_utils/shared" -I"D:/ProgramsOnHDD/DIRS/TestDirectory/communication-board/CommsBoard-backupUpdate/Core/Src/shared_utils/shared/CANopenNode" -I"D:/ProgramsOnHDD/DIRS/TestDirectory/communication-board/CommsBoard-backupUpdate/Core/Src/device_specific" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-device_specific-2f-protob

clean-Core-2f-Src-2f-device_specific-2f-protob:
	-$(RM) ./Core/Src/device_specific/protob/CompressorWriteBuffer.d ./Core/Src/device_specific/protob/CompressorWriteBuffer.o ./Core/Src/device_specific/protob/CompressorWriteBuffer.su ./Core/Src/device_specific/protob/hourly.pb.d ./Core/Src/device_specific/protob/hourly.pb.o ./Core/Src/device_specific/protob/hourly.pb.su ./Core/Src/device_specific/protob/pb_common.d ./Core/Src/device_specific/protob/pb_common.o ./Core/Src/device_specific/protob/pb_common.su ./Core/Src/device_specific/protob/pb_decode.d ./Core/Src/device_specific/protob/pb_decode.o ./Core/Src/device_specific/protob/pb_decode.su ./Core/Src/device_specific/protob/pb_encode.d ./Core/Src/device_specific/protob/pb_encode.o ./Core/Src/device_specific/protob/pb_encode.su

.PHONY: clean-Core-2f-Src-2f-device_specific-2f-protob

