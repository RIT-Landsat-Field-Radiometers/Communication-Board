################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/device_specific/Tasks/CANOpenTask.cpp \
../Core/Src/device_specific/Tasks/CellularTask.cpp \
../Core/Src/device_specific/Tasks/DataTask.cpp \
../Core/Src/device_specific/Tasks/FileSystemTask.cpp \
../Core/Src/device_specific/Tasks/MonitorTask.cpp \
../Core/Src/device_specific/Tasks/RTCTask.cpp 

OBJS += \
./Core/Src/device_specific/Tasks/CANOpenTask.o \
./Core/Src/device_specific/Tasks/CellularTask.o \
./Core/Src/device_specific/Tasks/DataTask.o \
./Core/Src/device_specific/Tasks/FileSystemTask.o \
./Core/Src/device_specific/Tasks/MonitorTask.o \
./Core/Src/device_specific/Tasks/RTCTask.o 

CPP_DEPS += \
./Core/Src/device_specific/Tasks/CANOpenTask.d \
./Core/Src/device_specific/Tasks/CellularTask.d \
./Core/Src/device_specific/Tasks/DataTask.d \
./Core/Src/device_specific/Tasks/FileSystemTask.d \
./Core/Src/device_specific/Tasks/MonitorTask.d \
./Core/Src/device_specific/Tasks/RTCTask.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/device_specific/Tasks/%.o Core/Src/device_specific/Tasks/%.su Core/Src/device_specific/Tasks/%.cyclo: ../Core/Src/device_specific/Tasks/%.cpp Core/Src/device_specific/Tasks/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DSTM32L4R9xx -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Chris/STM32CubeIDE/workspace_1.10.1/communication-board/CommsBoard/Core/Src/shared_utils/shared" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.10.1/communication-board/CommsBoard/Core/Src/shared_utils/shared/CANopenNode" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.10.1/communication-board/CommsBoard/Core/Src/device_specific" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall --specs=nosys.specs -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-device_specific-2f-Tasks

clean-Core-2f-Src-2f-device_specific-2f-Tasks:
	-$(RM) ./Core/Src/device_specific/Tasks/CANOpenTask.cyclo ./Core/Src/device_specific/Tasks/CANOpenTask.d ./Core/Src/device_specific/Tasks/CANOpenTask.o ./Core/Src/device_specific/Tasks/CANOpenTask.su ./Core/Src/device_specific/Tasks/CellularTask.cyclo ./Core/Src/device_specific/Tasks/CellularTask.d ./Core/Src/device_specific/Tasks/CellularTask.o ./Core/Src/device_specific/Tasks/CellularTask.su ./Core/Src/device_specific/Tasks/DataTask.cyclo ./Core/Src/device_specific/Tasks/DataTask.d ./Core/Src/device_specific/Tasks/DataTask.o ./Core/Src/device_specific/Tasks/DataTask.su ./Core/Src/device_specific/Tasks/FileSystemTask.cyclo ./Core/Src/device_specific/Tasks/FileSystemTask.d ./Core/Src/device_specific/Tasks/FileSystemTask.o ./Core/Src/device_specific/Tasks/FileSystemTask.su ./Core/Src/device_specific/Tasks/MonitorTask.cyclo ./Core/Src/device_specific/Tasks/MonitorTask.d ./Core/Src/device_specific/Tasks/MonitorTask.o ./Core/Src/device_specific/Tasks/MonitorTask.su ./Core/Src/device_specific/Tasks/RTCTask.cyclo ./Core/Src/device_specific/Tasks/RTCTask.d ./Core/Src/device_specific/Tasks/RTCTask.o ./Core/Src/device_specific/Tasks/RTCTask.su

.PHONY: clean-Core-2f-Src-2f-device_specific-2f-Tasks

