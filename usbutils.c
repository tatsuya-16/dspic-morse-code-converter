//=================================================================
//
//  usbutils.c
//
//  Simple utils for USB interface using UART1
//
// Modified from uartutils.c
//
//  Toshiharu MUKAI
//  25-sept-2015
//=================================================================

#include <stdio.h>
#include "usbutils.h"
#include "flags.h"  // project specific flags

//-----------------------------------------------------------------
// Initialise UART1 with simple setting
//-----------------------------------------------------------------
void OpenUART(long instructions_per_sec, long baud)
{
 unsigned int UMODE_value = UART_EN & 
						    UART_IDLE_STOP &
						    UART_DIS_WAKE & 
						    UART_DIS_LOOPBACK &
						    UART_DIS_ABAUD & 
						    UART_NO_PAR_8BIT &
						    UART_1STOPBIT;
 unsigned int USTA_value = UART_INT_TX &
						   UART_TX_PIN_NORMAL &
						   UART_TX_DISABLE &
						   UART_INT_RX_CHAR &
						   UART_ADR_DETECT_DIS &
						   UART_RX_OVERRUN_CLEAR ;
 unsigned int UBRG_value = ((instructions_per_sec/16)/baud) - 1;
 OpenUART1(UMODE_value, USTA_value, UBRG_value);
// U2STAbits.UTXEN = 0;  // turn OFF UART2 TX Enable
 U1STAbits.UTXEN = 1;  // turn on UART1 TX Enable
}

char usb_getc(void)
{
	char c = 0;
	if (DataRdyUART1()){
		c = ReadUART1();
	}
	return c;
}

int usb_input(char buff[], char echo_on)
{
	int i=0;
	
	while (i<USB_IN_BUFFER_SIZE){
		buff[i] = (char)usb_getc();
		if (buff[i] != 0){
			if (echo_on){
				WriteUART1(buff[i]);  // echo back
			}
			if (buff[i] == 0x0d || buff[i] == 0x0a){
				buff[i] = '\0';
				break;
			}	
			i++;
		}
	}
	if (echo_on){
		printf("\r\n");
	}
	return i;   // The number of letters except '\0'
}


#ifdef PRINTF_TO_UART2
//-----------------------------------------------------------------
// Override write method to redirect printf to UART2 
// (only use when you want to printf to UART2)
//-----------------------------------------------------------------
int write (int handle, void *buffer, unsigned int len)
{		
    while (U2STAbits.UTXBF);	           // wait until transmit buffer empty
    U2TXREG =  *((unsigned char*)buffer);  // type casting and dereferencing  of the void pointer 	
    return len; 
}
#endif


