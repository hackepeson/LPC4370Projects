################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/board.c \
../src/board_sysinit.c 

C_DEPS += \
./src/board.d \
./src/board_sysinit.d 

OBJS += \
./src/board.o \
./src/board_sysinit.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_LPCOPEN -DCORE_M4 -I"C:\workarea\GITHUB_REPOS\LPC4370Projects\lpc_board_nxp_lpclink2_4370\inc" -I"C:\workarea\GITHUB_REPOS\LPC4370Projects\lpc_chip_43xx\inc" -I"C:\workarea\GITHUB_REPOS\LPC4370Projects\lpc_chip_43xx\inc\config_43xx" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fsingle-precision-constant -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/board.d ./src/board.o ./src/board_sysinit.d ./src/board_sysinit.o

.PHONY: clean-src

