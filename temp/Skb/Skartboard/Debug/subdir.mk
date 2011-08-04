################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../accel_capture.c \
../adc.c \
../gpio.c \
../gyro_spi.c \
../helpers.c \
../irled_pwm.c \
../leds.c \
../main.c \
../monitor.c \
../motor_pwm.c \
../stabilizer.c \
../time_ref.c \
../transceiver_mac.c \
../transceiver_spi.c \
../uart.c 

OBJS += \
./accel_capture.o \
./adc.o \
./gpio.o \
./gyro_spi.o \
./helpers.o \
./irled_pwm.o \
./leds.o \
./main.o \
./monitor.o \
./motor_pwm.o \
./stabilizer.o \
./time_ref.o \
./transceiver_mac.o \
./transceiver_spi.o \
./uart.o 

C_DEPS += \
./accel_capture.d \
./adc.d \
./gpio.d \
./gyro_spi.d \
./helpers.d \
./irled_pwm.d \
./leds.d \
./main.d \
./monitor.d \
./motor_pwm.d \
./stabilizer.d \
./time_ref.d \
./transceiver_mac.d \
./transceiver_spi.d \
./uart.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	avr-gcc -mmcu=atmega128 -I/usr/avr/include -O0 -g3 -Wall -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


