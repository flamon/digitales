################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../SMAC/Source/SMAC.c" \

C_SRCS += \
../SMAC/Source/SMAC.c \

OBJS += \
./SMAC/Source/SMAC_c.obj \

OBJS_QUOTED += \
"./SMAC/Source/SMAC_c.obj" \

C_DEPS += \
./SMAC/Source/SMAC_c.d \

C_DEPS_QUOTED += \
"./SMAC/Source/SMAC_c.d" \

OBJS_OS_FORMAT += \
./SMAC/Source/SMAC_c.obj \


# Each subdirectory must supply rules for building sources it contributes
SMAC/Source/SMAC_c.obj: ../SMAC/Source/SMAC.c pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"SMAC/Source/SMAC.args" -ObjN="SMAC/Source/SMAC_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

SMAC/Source/%.d: ../SMAC/Source/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


