################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Application/Source/main.c" \

C_SRCS += \
../Application/Source/main.c \

OBJS += \
./Application/Source/main_c.obj \

OBJS_QUOTED += \
"./Application/Source/main_c.obj" \

C_DEPS += \
./Application/Source/main_c.d \

C_DEPS_QUOTED += \
"./Application/Source/main_c.d" \

OBJS_OS_FORMAT += \
./Application/Source/main_c.obj \


# Each subdirectory must supply rules for building sources it contributes
Application/Source/main_c.obj: ../Application/Source/main.c pre-build
	@echo 'Building file: $<'
	@echo 'Executing target #36 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Application/Source/main.args" -ObjN="Application/Source/main_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Application/Source/%.d: ../Application/Source/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


