#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <stdlib.h>

#include "global.h"
#include "rf12.h"     

#define F_CPU 16000000UL
#include <util/delay.h>

#include "hd44780.h"


void send(void);
void receive(void);

int main(void)
{
	DDRB |= (1<<PB1);

	rf12_init();					// ein paar Register setzen (z.B. CLK auf 10MHz)
	rf12_setfreq(RF12FREQ(433.92));	// Sende/Empfangsfrequenz auf 433,92MHz einstellen
	rf12_setbandwidth(4, 1, 4);		// 200kHz Bandbreite, -6dB Verst�rkung, DRSSI threshold: -79dBm 
	rf12_setbaud(19200);			// 19200 baud
	rf12_setpower(0, 6);			// 1mW Ausgangangsleistung, 120kHz Frequenzshift

	LCD_Initalize();

	for (;;)
	{
		//send();
		for (unsigned char i=0; i<10; i++)
			_delay_ms(10);

		receive();
	}
}

void receive(void)
{	char test[32];
	char test2[32];
	char disp[11]="";
	rf12_rxdata(test,32);
	strncpy(disp, test,8);
	if (strstr(test,"e") != 0 || strstr(test,"c") != 0) {
		PORTB |= (1<<PB1);
		//PORTB &= ~(1 << PB1);
		_delay_ms(1000);
	} else {
		PORTB &= ~(1 << PB1);
		//PORTB |= (1<<PB1);
	}
	strncat(disp,"   ",3);
	LCD_Clear();
	LCD_Home();
	LCD_GoTo(0,0);
	LCD_WriteText(disp);
	LCD_GoTo(0,1);
	LCD_WriteText("data received...");
}

void send(void)
{	unsigned char test[]="Dies ist ein 433MHz Test !!!\n   ";	
	rf12_txdata(test,32);
}

