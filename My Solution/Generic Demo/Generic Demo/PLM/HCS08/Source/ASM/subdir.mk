################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../PLM/HCS08/Source/ASM/ASM.c" \

C_SRCS += \
../PLM/HCS08/Source/ASM/ASM.c \

OBJS += \
./PLM/HCS08/Source/ASM/ASM_c.obj \

OBJS_QUOTED += \
"./PLM/HCS08/Source/ASM/ASM_c.obj" \

C_DEPS += \
./PLM/HCS08/Source/ASM/ASM_c.d \

C_DEPS_QUOTED += \
"./PLM/HCS08/Source/ASM/ASM_c.d" \

OBJS_OS_FORMAT += \
./PLM/HCS08/Source/ASM/ASM_c.obj \


# Each subdirectory must supply rules for building sources it contributes
PLM/HCS08/Source/ASM/ASM_c.obj: ../PLM/HCS08/Source/ASM/ASM.c pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #33 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"PLM/HCS08/Source/ASM/ASM.args" -ObjN="PLM/HCS08/Source/ASM/ASM_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

PLM/HCS08/Source/ASM/%.d: ../PLM/HCS08/Source/ASM/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


