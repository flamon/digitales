################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../PLM/HCS08/Source/EEPROM/Eeprom4OTAP/Eeprom4OTAP.c" \

C_SRCS += \
../PLM/HCS08/Source/EEPROM/Eeprom4OTAP/Eeprom4OTAP.c \

OBJS += \
./PLM/HCS08/Source/EEPROM/Eeprom4OTAP/Eeprom4OTAP_c.obj \

OBJS_QUOTED += \
"./PLM/HCS08/Source/EEPROM/Eeprom4OTAP/Eeprom4OTAP_c.obj" \

C_DEPS += \
./PLM/HCS08/Source/EEPROM/Eeprom4OTAP/Eeprom4OTAP_c.d \

C_DEPS_QUOTED += \
"./PLM/HCS08/Source/EEPROM/Eeprom4OTAP/Eeprom4OTAP_c.d" \

OBJS_OS_FORMAT += \
./PLM/HCS08/Source/EEPROM/Eeprom4OTAP/Eeprom4OTAP_c.obj \


# Each subdirectory must supply rules for building sources it contributes
PLM/HCS08/Source/EEPROM/Eeprom4OTAP/Eeprom4OTAP_c.obj: ../PLM/HCS08/Source/EEPROM/Eeprom4OTAP/Eeprom4OTAP.c pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #21 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"PLM/HCS08/Source/EEPROM/Eeprom4OTAP/Eeprom4OTAP.args" -ObjN="PLM/HCS08/Source/EEPROM/Eeprom4OTAP/Eeprom4OTAP_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

PLM/HCS08/Source/EEPROM/Eeprom4OTAP/%.d: ../PLM/HCS08/Source/EEPROM/Eeprom4OTAP/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


