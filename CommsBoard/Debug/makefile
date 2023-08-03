################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/subdir.mk
-include Middlewares/Third_Party/FatFs/src/option/subdir.mk
-include Middlewares/Third_Party/FatFs/src/subdir.mk
-include FATFS/Target/subdir.mk
-include FATFS/App/subdir.mk
-include Drivers/STM32L4xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/shared_utils/shared/bsp/UART/subdir.mk
-include Core/Src/shared_utils/shared/bsp/LEDs/subdir.mk
-include Core/Src/shared_utils/shared/bsp/DS28CM00ID/subdir.mk
-include Core/Src/shared_utils/shared/bsp/CANOpenNode/subdir.mk
-include Core/Src/shared_utils/shared/Logging/subdir.mk
-include Core/Src/shared_utils/shared/CANopenNode/storage/subdir.mk
-include Core/Src/shared_utils/shared/CANopenNode/extra/subdir.mk
-include Core/Src/shared_utils/shared/CANopenNode/309/subdir.mk
-include Core/Src/shared_utils/shared/CANopenNode/305/subdir.mk
-include Core/Src/shared_utils/shared/CANopenNode/304/subdir.mk
-include Core/Src/shared_utils/shared/CANopenNode/303/subdir.mk
-include Core/Src/shared_utils/shared/CANopenNode/301/subdir.mk
-include Core/Src/shared_utils/shared/CANopenNode/subdir.mk
-include Core/Src/device_specific/zlib/subdir.mk
-include Core/Src/device_specific/protob/subdir.mk
-include Core/Src/device_specific/Tasks/subdir.mk
-include Core/Src/device_specific/JSON/Parser/subdir.mk
-include Core/Src/device_specific/JSON/Generator/subdir.mk
-include Core/Src/device_specific/HTTP/mbed-http/http_parser/subdir.mk
-include Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Com/Src/subdir.mk
-include Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Interface/Cellular_Ctrl/Src/subdir.mk
-include Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Trace/Src/subdir.mk
-include Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Runtime_Library/Src/subdir.mk
-include Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Rtosal/Src/subdir.mk
-include Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/PPPosif/Src/subdir.mk
-include Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Ipc/Src/subdir.mk
-include Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Error/Src/subdir.mk
-include Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Data_Cache/Src/subdir.mk
-include Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/Cellular_Service/Src/subdir.mk
-include Core/Src/device_specific/Cellular/Middlewares/ST/STM32_Cellular/Core/AT_Core/Src/subdir.mk
-include Core/Src/device_specific/Cellular/Drivers/BSP/AT_modem_bg96/Src/subdir.mk
-include Core/Src/device_specific/Cellular/subdir.mk
-include Core/Src/device_specific/CANOpenNode/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := CommsBoard
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
CommsBoard.elf \

MAP_FILES += \
CommsBoard.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
CommsBoard.list \

OBJCOPY_BIN += \
CommsBoard.bin \


# All Target
all: main-build

# Main-build Target
main-build: CommsBoard.elf secondary-outputs

# Tool invocations
CommsBoard.elf CommsBoard.map: $(OBJS) $(USER_OBJS) C:\Users\Chris\STM32CubeIDE\workspace_1.10.1\communication-board\CommsBoard\STM32L4R9VGTX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-g++ -o "CommsBoard.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\Chris\STM32CubeIDE\workspace_1.10.1\communication-board\CommsBoard\STM32L4R9VGTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="CommsBoard.map" -Wl,--gc-sections -static  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

CommsBoard.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "CommsBoard.list"
	@echo 'Finished building: $@'
	@echo ' '

CommsBoard.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "CommsBoard.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) CommsBoard.bin CommsBoard.elf CommsBoard.list CommsBoard.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
