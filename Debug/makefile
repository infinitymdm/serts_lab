################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := clean

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include RTE/USB/subdir.mk
-include RTE/File_System/subdir.mk
-include RTE/Device/STM32F407VGTx/subdir.mk
-include RTE/Compiler/subdir.mk
-include RTE/CMSIS_Driver/subdir.mk
-include RTE/CMSIS/subdir.mk
-include RTE/Board_Support/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(SX_DEPS)),)
-include $(SX_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(SX_UPPER_DEPS)),)
-include $(SX_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
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
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: BaseProject_2020_08_05.axf

# Tool invocations
BaseProject_2020_08_05.axf: $(OBJS) C:\Users\infin\OneDrive\ -\ oc.edu\Documents\SERTS\Labs\Lab\ 03\Lab_03_20200901/STM32F407VGTx.sct $(USER_OBJS) $(LIBS)
	@echo 'Building target: $@'
	@echo 'Invoking: Arm Linker 5'
	armlink.exe --cpu=Cortex-M4.fp.sp --entry=Reset_Handler --scatter="C:\Users\infin\OneDrive - oc.edu\Documents\SERTS\Labs\Lab 03\Lab_03_20200901/STM32F407VGTx.sct" --info=sizes --strict -o "BaseProject_2020_08_05.axf" $(OBJS) $(USER_OBJS) "C:/Users/infin/AppData/Local/Arm/Packs/ARM/CMSIS/5.7.0/CMSIS/DSP/Lib/ARM/arm_cortexM4lf_math.lib" "C:/Users/infin/AppData/Local/Arm/Packs/ARM/CMSIS/5.7.0/CMSIS/RTOS/RTX/LIB/ARM/RTX_CM4.lib" "C:/Users/infin/AppData/Local/Arm/Packs/Keil/MDK-Middleware/7.12.0/FileSystem/Lib/ARM/FS_LFN_CM3_L.lib" "C:/Users/infin/AppData/Local/Arm/Packs/Keil/MDK-Middleware/7.12.0/USB/Lib/ARM/USB_CM3_L.lib" "C:/Users/infin/OneDrive - oc.edu/Documents/SERTS/Labs/Lab 03/Lab_03_20200901/src/libPDMFilter_CM4_Keil.lib"
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(SX_DEPS) $(ASM_UPPER_DEPS) $(EXECUTABLES) $(OBJS) $(SX_UPPER_DEPS) $(ASM_DEPS) $(S_DEPS) $(S_UPPER_DEPS) $(C_DEPS) BaseProject_2020_08_05.axf 
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
