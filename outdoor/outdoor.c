#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <stdlib.h>
#include <util/atomic.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>

#include "global.h"
#include "rf12.h"     

#define F_CPU 8000000UL
#include <util/delay.h>

#include "onewire.h"
#define DS18X20_MAX_RESOLUTION
#include "ds18x20.h"

void send(char *param);
void receive(void);

int main(void)
{
	DDRC = 0x00;
	PORTC = 0xff;
	DDRB |= (1<<PB1);
	// DDRB |= 1 << 1;		//PB1 as output
	// PORTB |= 1 << 1;		//PB1 diode on

	uint8_t ow_error = DS18X20_OK;
	char store[11]="";
	char store_button[2]="";

	//sei();

	rf12_init();					// ein paar Register setzen (z.B. CLK auf 10MHz)
	rf12_setfreq(RF12FREQ(433.92));	// Sende/Empfangsfrequenz auf 433,92MHz einstellen
	rf12_setbandwidth(4, 1, 4);		// 200kHz Bandbreite, -6dB Verst�rkung, DRSSI threshold: -79dBm 
	rf12_setbaud(19200);			// 19200 baud
	rf12_setpower(0, 6);			// 1mW Ausgangangsleistung, 120kHz Frequenzshift

	ow_error = DS18X20_start_meas(DS18X20_POWER_EXTERN, NULL);

	uint16_t cycle = 0;

	strcpy(store, "initialization");

	for (;;)
	{

		//strcpy(store, "00.00");

		if (PINC == 0x3D || PINC == 0x3E || PINC == 0x3C) {
			//PORTB |= (1<<PB1);
			//_delay_ms(1000);
			itoa(PINC,store_button,16);
			strncat(store," ",1);
			strncat(store,store_button,2);
		}
		if((cycle%300) == 0) {
			if(!DS18X20_conversion_in_progress()) {
				int32_t temp;
				ow_error |= DS18X20_read_maxres_single(DS18B20_FAMILY_CODE, &temp);
				ow_error |= DS18X20_start_meas(DS18X20_POWER_EXTERN, NULL);
				DS18X20_format_from_maxres(temp, store, 10);
			}
		}
		cycle = (cycle+1) % 1000;
		_delay_ms(1);
		//PORTB &= ~(1 << PB1);

		send(store);
		for (unsigned char i=0; i<10; i++)
			_delay_ms(1);

	//	receive();
	}
}

void receive(void)
{	unsigned char test[32];	
	rf12_rxdata(test,32);	
	// daten verarbeiten
}

void send(char *param)
{	unsigned char test=param;
	rf12_txdata(param,32);
}

