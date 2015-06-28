#ifndef MMAPGPIO_H
#define MMAPGPIO_H
//
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//
/***********************************************************************
* Author: Nicolas Coustaud
*
* The Gpio Class is able to control the direction and state of all GPIO pins
* on the RPI's 40-pin header using the 'mmaping into /dev/mem' approach. 
* The advantage of this approach is
* that GPIO toggling speeds of uptp 21MHz can be achieved whereas the toggling gpio
* Program for Raspberry PI 2
*
* *********************************************************************/
class Gpio{

public:
	Gpio();// default constructor. Mmaps into /dev/mem'
	~Gpio();// removes mapping between process memory
	// & physical memory
	void writeGPIOReg(unsigned int reg, unsigned int val);
	void readGPIOReg(unsigned int reg, unsigned int &val);
	// The two methods above are able to read from and write to
	// the GPIO registers listed on lines 54-59 
	// sets  GPIO direction
	void setPinDirOutput(unsigned int pinnum);
	void setPinDirInput(unsigned int pinnum);
	//reads the value of a pin
	unsigned int readPin(unsigned int pinnum);
	//writes the state of output pins to either high or low.
	void writePinState(unsigned int pinnum, const unsigned int &pinstate);
    //
	void writePinHigh(unsigned int pinnum);
	void writePinLow(unsigned int pinnum);
private:
	//gpio registers
	static const unsigned int GPFSET0 = 7;
	static const unsigned int GPFCLR0 = 10;
	static const unsigned int GPFLEV0 = 13;
	static const unsigned int GPFSEL0 = 0;
	static const unsigned int GPFSEL1 = 1;
	static const unsigned int GPFSEL2 = 2;
	static const unsigned int GPFSEL3 = 3;
	// two possible states for output pins
	static const unsigned int LOW = 0;
	static const unsigned int HIGH = 1;
//
	static const uint32_t GPIO_BASE = 0x3f200000;// gpio registers base address PI2
	static const uint32_t GPIO_LEN = 0xB4;// need only map B4 registers
	volatile uint32_t *m_gpio;
};
#endif
