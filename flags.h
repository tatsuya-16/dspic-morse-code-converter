//=================================================================
//
//  flags.h
//
//  project specific flags
//
//  Jonathan Rossiter
//   18-apr-2005
//=================================================================
#ifndef FLAGS
#define FLAGS
//<<
//#include <p30f6012a.h>

#define dsPIC306012a
#define CLOCK_120MHZ

#define ERROR_LED LATCbits.LATC13		// LED2 (blue) connected to RA11

//#define PRINTF_TO_UART2  // use UART2 for printf output (default is UART1)


#ifdef CLOCK_120MHZ
 #define CRYSTAL 	7372800		// Crystal frequency = 7.3MHz
 #define PLL			16			// PLL multiplier
 #define CLOCK 		CRYSTAL*PLL	// Internal clock = 7.3*16 = 116.8MHz
 #define FCY 		CLOCK/4		// Instruction frequency	
 #define IPS 		FCY			// Instructions per second
 #define MILLISEC 	FCY/10000	// 1 mSec delay constant (*16 PLL only)
#endif

//>>
#endif
