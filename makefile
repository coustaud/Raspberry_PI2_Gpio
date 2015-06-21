all: main 

main: main.o gpio.o
	g++ Raspberry_PI2_Gpio.o Raspberry_PI2_Gpio_main.o -o Raspberry_PI2_Gpio_main
	
main.o: Raspberry_PI2_Gpio_main.cpp
	g++ -c Raspberry_PI2_Gpio_main.cpp

gpio.o: Raspberry_PI2_Gpio.cpp
	g++ -c Raspberry_PI2_Gpio.cpp
