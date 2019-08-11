/*
* TX_NRF.cpp
*
* Created: 6/20/19 8:42:37 PM
* Author : Xiu
*/

#include "RF24.h"

void send(unsigned char c)
{
	while(bit_is_clear(UCSRA,UDRE));// doi den khi bit UDRE = 1
	UDR = c;
}

RF24 myRadio (SS, CSN);


uint8_t addresses[][6] = {"0","1","2"};	// Receiver : 0, Quan, Tram
uint8_t _Receive = 0;

uint8_t _sendalarmtoone = 'A';
uint8_t _feedbackturnoffalarmtoA = '2';
uint8_t _feedbackWARNINGtoA = 'I';

uint8_t _sendalarmtotwo = 'B';
uint8_t _feedbackturnoffalarmtoB = '5';
uint8_t _feedbackWARNINGtoB = 'O';


int main(void)
{
	DDRD = 0x00;
	PORTD = 0b11110000;
	DDRC = 0xff;
	sbi(PORTC,WarningBuzzer); // OFF BUZZER
	//UBRRH = 0b00000001; // 0x01
	UBRRL = 103; // 4800 8MHZ // 9600 16MHZ
	//UCSRA = 1<<U2X;
	
	UCSRC =	(1<<URSEL)|(1 << UCSZ1) | (1 << UCSZ0); // k chon UBRRch va chon mode 8bit
	UCSRB = (1 << RXEN) | (1 << TXEN)|(1<<RXCIE); //EN rx,tx,ngat
	
	myRadio.begin();
	myRadio.setChannel(115);
	myRadio.setPALevel(RF24_PA_MAX);
	myRadio.setDataRate( RF24_250KBPS );
	myRadio.openReadingPipe(1, addresses[0]);
	myRadio.startListening();
	

	while (1)
	{
		if ( myRadio.available()) {
			while (myRadio.available())
			myRadio.read( &_Receive, sizeof(_Receive) );
			
			_delay_ms(20);
			if(_Receive == '0')	// Turn off led after receive feed back turn off alarm from A
			{sbi(PORTC,XiuLed);}
			
			else if(_Receive == '1')	// Alarm feed back to A when A want to turn off
			{
				_delay_ms(400);
				myRadio.stopListening();
				myRadio.openWritingPipe(addresses[1]);
				myRadio.write(&_feedbackturnoffalarmtoA, sizeof(_feedbackturnoffalarmtoA));
				myRadio.openReadingPipe(1, addresses[0]);
				myRadio.startListening();
				cbi(PORTC,XiuLed);
			}

			else if(_Receive == '3')// Turn off led after receive feed back turn off alarm from B
			{sbi(PORTC,TramLed);}
			
			else if(_Receive == '4')	// Alarm feed back to B when B want to turn off
			{
				_delay_ms(400);
				myRadio.stopListening();
				myRadio.openWritingPipe(addresses[2]);
				myRadio.write(&_feedbackturnoffalarmtoB, sizeof(_feedbackturnoffalarmtoB));
				myRadio.openReadingPipe(1, addresses[0]);
				myRadio.startListening();
				cbi(PORTC,TramLed);
			}
			
			else if(_Receive == 'W')	// Warning mes From A
			{
				_delay_ms(400);
				myRadio.stopListening();
				myRadio.openWritingPipe(addresses[1]);
				myRadio.write(&_feedbackWARNINGtoA, sizeof(_feedbackWARNINGtoA));
				myRadio.openReadingPipe(1, addresses[0]);
				myRadio.startListening();
				sbi(PORTC,WarningLed);	// ON WARNING LED
				cbi(PORTC,WarningBuzzer);	// ON WARNING BUZZER
			}
			
			else if(_Receive == 'M')	// Warning mes From B
			{
				_delay_ms(400);
				myRadio.stopListening();
				myRadio.openWritingPipe(addresses[2]);
				myRadio.write(&_feedbackWARNINGtoB, sizeof(_feedbackWARNINGtoB));
				myRadio.openReadingPipe(1, addresses[0]);
				myRadio.startListening();
				sbi(PORTC,WarningLed);	// ON WARNING LED
				cbi(PORTC,WarningBuzzer); // ON WARNING BUZZER
			}
			
			
			//send(_Receive);	// print to Serial

		}
		
		if(bit_is_clear(PIND,7))	// Send Alarm to Quan
		{
			while(bit_is_clear(PIND,7));
			myRadio.stopListening();
			myRadio.openWritingPipe(addresses[1]);
			myRadio.write(&_sendalarmtoone, sizeof(_sendalarmtoone));
			myRadio.openReadingPipe(1, addresses[0]);
			myRadio.startListening();
			//send('Z');
			
		}
		
		else if(bit_is_clear(PIND,6)) // Send Alarm to Tram
		{
			while(bit_is_clear(PIND,6));
			myRadio.stopListening();
			myRadio.openWritingPipe(addresses[2]);
			myRadio.write(&_sendalarmtotwo, sizeof(_sendalarmtotwo));
			myRadio.openReadingPipe(1, addresses[0]);
			myRadio.startListening();
			//send('Z');
			
		}
		
		else if(bit_is_clear(PIND,5))	// Turn off WARNING
		{
			while(bit_is_clear(PIND,5));
			cbi(PORTC,WarningLed); // OFF WARNING LED
			sbi(PORTC,WarningBuzzer); // OFF WARNING BUZZER
		}
		
	}
}
