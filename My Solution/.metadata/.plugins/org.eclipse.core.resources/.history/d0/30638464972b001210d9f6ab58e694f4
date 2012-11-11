################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../UMAC/uMac.c" \

C_SRCS += \
../UMAC/uMac.c \

OBJS += \
./UMAC/uMac_c.obj \

OBJS_QUOTED += \
"./UMAC/uMac_c.obj" \

C_DEPS += \
./UMAC/uMac_c.d \

C_DEPS_QUOTED += \
"./UMAC/uMac_c.d" \

OBJS_OS_FORMAT += \
./UMAC/uMac_c.obj \


# Each subdirectory must supply rules for building sources it contributes
UMAC/uMac_c.obj: ../UMAC/uMac.c pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"UMAC/uMac.args" -ObjN="UMAC/uMac_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

UMAC/%.d: ../UMAC/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


