//=================================================================
//
//  musdelay.c
//
//  Modified from msdelay.c
//
//  This is a generic 1ms (1us) delay routine to give a 1mS to 65.5 Sec 
//  (1us to 65.5ms) delay
//  For N = 1 the delay is 1 mS, for N = 65535 the delay is 65,535 mS. 
//  Note that FCY is used in the computation.  
//
//  Toshiharu MUKAI
//  22 April 2006
//=================================================================
#include "flags.h"  // project specific flags
#include "musdelay.h"

void DelayNmSec(unsigned int N)
{
	volatile unsigned int j;
	while(N--)
	 	for(j=0;j < MILLISEC;j++);  // NB: each loop takes about 10 instr.
}

void DelayNuSec(unsigned int N)
{
	volatile unsigned int j;
	while(N--)
	 	for(j=0;j < MICROSEC; j++);  
}

// END OF FILE
