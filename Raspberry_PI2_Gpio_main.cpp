#include "Raspberry_PI2_Gpio.h"
#include "stdio.h"
//
int main(void){
	Gpio rpiGpio; // instantiate an instance of the mmapGpio class
	rpiGpio.setPinDirOutput(17); // set GPIO17 to output
	while (1) {// toggle pin as fast as possible
		rpiGpio.writePinHigh(17);
		rpiGpio.writePinLow(17);
	}
	return 0;
}
