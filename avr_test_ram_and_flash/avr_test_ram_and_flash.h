#include <stdint.h>

//#define RAM_INJECT_FAULT 0x0000  // uncomment to inject a fault as explained in the top of the page

//#define TEST_RAM 1 // uncomment to test the RAM (0 for r0 part other value for r1 part of MATS++)
//#define TEST_FLASH 1 // uncomment to test the FLASH (value doesn't matter)
//#define RESET_CHECKSUM 1


int8_t testFlash();
int8_t testRam();

void* testError(void* arg);

#ifdef TEST_RAM	// uncomment TEST_RAM to test the ram (0 for r0 part other value for r1 part of MATS++)
	void __attribute__((constructor)) TestRAM() {  // this method is not portable it may not work on all compilers !!! it serves to call this function before the execution of the main function

		if(testRam()){
            testError();
        }
		
	} 
#endif

#ifdef TEST_FLASH	// uncomment TEST_FASH to test the flash (value doesn't matter)
	void __attribute__((constructor)) TestFLASH(){ // this method is not portable it may not work on all compilers !!! it serves to call this function before the execution of the main function
	
		//this function is declared before the RAM tester because if the FLASH memory is corrupted the RAM tester might have the wrong code or corrupted code and it may give false positives or negatives
		// because of that this tester doesn't use RAM, all variables are stored on registers, and in doing so the integrity of the RAM doesn't matter (it kind of does because the return address is stored on the RAM's stack, so maybe this needs to be changed (store the return address on registers and do a jump maybe))
#ifdef  RESET_CHECKSUM

        eeprom_write_word(0x0000, 0x0000);

#endif
#ifndef RESET_CHECKSUM		
        if(!eeprom_read_word(0x0000))
            eeprom_write_word(0x0000, generateChecksum());

		if(testFlash())
			testError();
#endif
	}
#endif

