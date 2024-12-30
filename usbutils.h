//=================================================================
//
//  usbutils.h
//
//  header file for usbutils.c
//
//  Toshiharu MUKAI
//  22 April 2006
//=================================================================
#ifndef UARTUTILS
#define UARTUTILS
//<<
#include <uart.h>

#define USB_IN_BUFFER_SIZE 	256
#define CLEAR "\033[;H\033[2J"   // escape sequence for clear

void OpenUART(long instructions_per_sec, long baud);

char usb_getc(void);
int usb_input(char buff[], char echo_on);

//>>
#endif
