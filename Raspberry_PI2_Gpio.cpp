#include "Raspberry_PI2_Gpio.h"
//
/*******************************************************************
* Default constructor....
* Simply calls mapRegAddri() function to map the physical addresses
* of the GPIO registers into local process memory
*
* Parameters - None
* Return Value - None
*******************************************************************/
Gpio::Gpio(){
	int mem_fd = 0;
	m_gpio = (uint32_t *) MAP_FAILED;
	/* open /dev/mem.....need to run program as root i.e. use sudo or su */
	if ((mem_fd = open("/dev/mem", O_RDWR | O_SYNC)) < 0) {
		perror("can't open /dev/mem");
		exit(1);
	}
	/* mmap IO return address and copy to gpio pointer address
	 * (unint32_t *) requested because returned type of mmap is void
	 * and gpio type is unint32_t, otherwise copy of address is not possible
	 * because the to pointers have different type*/
	m_gpio  = (uint32_t *)mmap(
		NULL,             //Any adddress in our space will do
		GPIO_LEN,       //Map length
		PROT_READ | PROT_WRITE | PROT_EXEC,// Enable reading & writting to mapped memory
		MAP_SHARED | MAP_LOCKED,       //Shared with other processes
		mem_fd,           //File to map
		GPIO_BASE         //Offset to base address
		);
	if (m_gpio == MAP_FAILED) {
		perror("mmap error");
		close(mem_fd);
		exit(1);
	}
	if (close(mem_fd) < 0){ //No need to keep mem_fd open after mmap
		//i.e. we can close /dev/mem
		perror("couldn't close /dev/mem file descriptor");
		exit(1);
	}
}
/*******************************************************************
* Destructor - unmaps GPIO registers (physical  memory)  from
* process memoy
*
* Parameters - None
* Return Value - None
******************************************************************/
Gpio::~Gpio(){
	//unmap GPIO registers (physicalmemory)  from process memoy
	if (munmap((void*)m_gpio, GPIO_LEN) < 0){
		perror("munmap (gpio) failed");
		exit(1);
	}
}
/*******************************************************************
* writeGPIOReg() - Writes a 32-bit value to one of the GPIO
* addresses listed on lines 4-9. This function is not required for
* basic GPIO usage (low level access function) but is made available
* anyways.
*
* Parameters reg - Register address to write to.....see lines 4-9
*            val - unsigned 32-bit value to write to the reg
* Return Value - none
* ****************************************************************/
void Gpio::writeGPIOReg(unsigned int reg, unsigned int val){
	m_gpio[reg] = val;
}
/*******************************************************************
* readGPIOReg() - reads a 32-bit value from one of the GPIO
* addresses listed on lines 4-9. This function is not required for
* basic GPIO usage (low level access function) but is made available
* anyways.
*
* Parameters reg - Register address to read from.....see lines 4-9
*            val - Value of reg is written to val and passed back to
*                  calling function/method by reference
* Return Value - none
* ****************************************************************/
void Gpio::readGPIOReg(unsigned int reg, unsigned int &val){
	val = m_gpio[reg];
}
/*******************************************************************
* setPinDir() - sets the direction of a pin to either input or
* output
*
* Parameters - pinnum - GPIO pin number as per the RPI's  BCM2835's
*                       standard definition
*              dir - pin direction can be mmapGpio::INPUT for input
*                    or mmapGpio::OUTPUT for output
* Return Value -None
* *****************************************************************/
void Gpio::setPinDirOutput(unsigned int pinnum){
	m_gpio[pinnum / 10] &= ~(7 << (((pinnum) % 10) * 3));
	m_gpio[pinnum / 10] |= (1 << (((pinnum) % 10) * 3));
}
/******************************************************************/
void Gpio::setPinDirInput(unsigned int pinnum){
	m_gpio[pinnum / 10] &= ~(7 << (((pinnum) % 10) * 3));
}
/*******************************************************************
* readPin() - reads the state of a GPIO pin and returns its value
*
* Parameter - pinnum - the pin number of the GPIO to read
*
* return Value - pin value. Either 1 (mmapGpio::HIGH) if pin state
*                is high or 0 (mmapGpio::LOW) if pin is low
* ****************************************************************/
unsigned int Gpio::readPin(unsigned int pinnum){
	unsigned int retVal = 0;
	if ((m_gpio[GPFLEV0] & (1 << pinnum)) != 0)
		retVal = 1;
	return retVal;
}
/*******************************************************************
* writePinState() - sets (to 1) or clears (to 0) the state of an
* output GPIO. This function has no effect on input GPIOs.
* For faster output GPIO pin setting/clearing..use inline functions
* 'writePinHigh()' & 'writePinLow()' defined in the header file
*
* Parameters - pinnum - GPIO number as per RPI and BCM2835
*                       standard definition
*              pinstate - value to write to output pin...either
*              Gpio::HIGH for setting or Gpio::LOW for
*              clearing
* Return Value - None
* ****************************************************************/
void Gpio::writePinState(unsigned int pinnum, const unsigned int &pinstate){
	if (pinstate == HIGH)
		m_gpio[GPFSET0] = (1 << pinnum);
	else
		m_gpio[GPFCLR0] = (1 << pinnum);
}
/*********************************************************************************/
void Gpio::writePinHigh(unsigned int pinnum){
	m_gpio[GPFSET0] = (1 << pinnum);
}
/*********************************************************************************/
void Gpio::writePinLow(unsigned int pinnum){
	m_gpio[GPFCLR0] = (1 << pinnum);
}
