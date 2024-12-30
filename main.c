/*
 * File:   main.c
 * Author: Toshiharu MUKAI
 *
 * Input and output through USB  (serial port 921600/8/N/1/NoFlowCont)
 * Created on 2015/09/25, 11:55
 */

#include <stdio.h>
#include <string.h>
#include "xc.h"
#include "musdelay.h"
#include "usbutils.h"
#include "flags.h"

//-----------------------------------------------------------------
//  CONFIGURATION FUSES (copied to HEX file)
//-----------------------------------------------------------------
#pragma config FCKSMEN=CSW_FSCM_OFF, FOSFPR=FRC_PLL16
#pragma config WDT=WDT_OFF
#pragma config BOREN=PBOR_OFF, BODENV=BORV_27, FPWRT=PWRT_16, MCLRE=MCLR_EN
#pragma config GCP=GSS_OFF

//-----------------------------------------------------------------
//  CONSTANTS 
//-----------------------------------------------------------------
#define LED1			LATCbits.LATC13		// LED1 (red) on C13
#define LED2			LATCbits.LATC14		// LED2 (blue) on C14
#define LED1_DIR		TRISCbits.TRISC13	
#define LED2_DIR		TRISCbits.TRISC14	
#define INPUT 1
#define OUTPUT 0
#define BAUD 921600		// UART Baud Rate
#define BUFFERSIZE 5	// ASCII buffers for serial dataX 
#define BUZZER_PIN LATBbits.LATB2
#define DOT_DURATION 50
#define DASH_DURATION 150
#define SPACE_DURATION 50
#define LETTER_DURATION 150
#define WORD_DURATION 350

// モールス信号辞書
const char* morse[] = {
    ".-",   "-...", "-.-.", "-..",  ".",    "..-.", "--.",  "....", "..",   ".---", // A-J
    "-.-",  ".-..", "--",   "-.",   "---",  ".--.", "--.-", ".-.",  "...", "-",    // K-T
    "..-",  "...-", ".--",  "-..-", "-.--", "--.."              // U-Z
};

//-----------------------------------------------------------------
// Initialise I/O ports (1=input, 0=output)
//-----------------------------------------------------------------
void SetupPorts(void)
{
   	ADPCFG = 0xffff; // set all to Digital input

	PORTB = 0;
	TRISB = 0b1111111111111111;   // set PORTB input
	PORTC = 0;
	TRISC = 0b1111111111111111;   // set PORTC input
	PORTD = 0; 
	TRISD = 0b1111111111111111;;  // Set PORTD input
	PORTF = 0;
	TRISF = 0b1111111111111111;   // set PORTF input
	PORTG = 0;
	TRISG = 0b1111111111111111;   // set PORTG input
}

void string_to_morse(char c[]) {
    int i = 0;
    for (i=0; i<strlen(c); i++){
        if (c[i] >= 'a' && c[i] <= 'z') {
            playMorse(morse[c[i] - 'a']);
        } else if (c[i] >= 'A' && c[i] <= 'Z') {
            playMorse(morse[c[i] - 'A']);
        } else if (c[i] == " ") {
            DelayNmSec(WORD_DURATION);
        }
    }
}

// モールス信号生成
void playMorse(const char* code) {
    while (*code) {
        if (*code == '.') {
            beep(DOT_DURATION);
        } else if (*code == '-') {
            beep(DASH_DURATION);
        }
        DelayNmSec(SPACE_DURATION);
        code++;
    }
    DelayNmSec(LETTER_DURATION);
}

// ブザー制御
void beep(int duration) {
    BUZZER_PIN = 1;
    DelayNmSec(duration);
    BUZZER_PIN = 0;
//    DelayNmSec(SPACE_DURATION);
}
 
  
//-----------------------------------------------------------------
// MAIN
//-----------------------------------------------------------------
 int main(void) {

	int n, echo_on=1;
	char cbuf[USB_IN_BUFFER_SIZE];
    char morse[USB_IN_BUFFER_SIZE];

	SetupPorts();
    TRISBbits.TRISB0 = 0; // RB0を出力に設定
    TRISBbits.TRISB2 = 0; // RB2を出力に設定（ブザー）
    BUZZER_PIN = 0;       // ブザー初期状態はOFF// Initialize PORTS
 	OpenUART(FCY,BAUD);				// Initialize UART1
	LED1_DIR = OUTPUT;
	LED2_DIR = OUTPUT;

	printf(CLEAR);
	printf("dsPIC board USB test program\r\n");

	while (1){
		LED1 = LED2;				// flash LEDs
		LED2 = !LED1;
		printf("Press [a]\r\n\r\n");
		while (usb_getc() != 'a') ;

		printf("What is your input?: ");
		n = usb_input(cbuf, echo_on);
		printf("Your input was '%s' and %d letters\r\n\r\n", cbuf, n);
        string_to_morse(cbuf);
	}
	
    return 0;
}

// END OF FILE
