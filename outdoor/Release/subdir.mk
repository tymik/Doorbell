################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../crc8.c \
../ds18x20.c \
../onewire.c \
../outdoor.c \
../rf12.c 

OBJS += \
./crc8.o \
./ds18x20.o \
./onewire.o \
./outdoor.o \
./rf12.o 

C_DEPS += \
./crc8.d \
./ds18x20.d \
./onewire.d \
./outdoor.d \
./rf12.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega8 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


