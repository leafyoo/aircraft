KDIR := /home/yu/yu-kernel
CC = /opt/codesourcery/arm-none-linux-gnueabi/arm-2007q3/bin/arm-none-linux-gnueabi-g++  

CFLAGS= -Wall -g 
ARCH :=arm
all:echo main

echo:
	@echo "+++++ Make All +++++"

main: UART_Devices.h UART_PWM.h
	$(CC) $(CFLAGS) pwm_server.cpp -o start_pwm 

.PHONY : echo main clc
clc:
	@echo "----- Clean -----"
	rm -f start_pwm
