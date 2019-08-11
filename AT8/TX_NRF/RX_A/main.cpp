/*
 * A_RX.cpp
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

uint8_t addresses[][6] = {"1","0"}; // Lis:1 , Send:0
uint8_t _Send_FirstTime_1 = 'K';
uint8_t _Receive_1 = 0;

uint8_t _Send_Alarm_Off_1 = '1';
uint8_t _Feedback_1 = '0';
uint8_t _Send_W_1 = 'W';


	
int main(void)
{
	DDRD = 0x00;
	PORTD = 0b11110000;
	DDRC = 0xff;

	sbi(PORTC,3);
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
		_delay_ms(2000);
		myRadio.stopListening();
		
		//Start First
		//send('F');
		myRadio.openWritingPipe(addresses[1]);
		myRadio.write(&_Send_FirstTime_1, sizeof(_Send_FirstTime_1));
		myRadio.openReadingPipe(1, addresses[0]);
		myRadio.startListening();
		_delay_ms(2000);

	while (1)
	{

		if ( myRadio.available()) {
		while (myRadio.available())
			myRadio.read( &_Receive_1, sizeof(_Receive_1) );
		
		_delay_ms(20);
		 if(_Receive_1 == 'A')	// Receive Call Alarm from Center
		 {
			 _delay_ms(200);
			 myRadio.stopListening();
				//send('L');
			 myRadio.openWritingPipe(addresses[1]);
			 myRadio.write(&_Feedback_1, sizeof(_Feedback_1));	
			 myRadio.openReadingPipe(1, addresses[0]);
			 myRadio.startListening();
			 cbi(PORTC,AlarmBuzzer); // ON ALARM
			
		 }
		 else if(_Receive_1 == '2')
		 {
			 _delay_ms(200);
			 sbi(PORTC,AlarmBuzzer); // OFF ALARM
			
		 }
		 
		  else if(_Receive_1 == 'I') // OFF WARNING
		  {
			  _delay_ms(200);
			 cbi(PORTC,WarningLed); // OFF WARNING
		  }
		 

		}
			
	if(bit_is_clear(PIND,5))	// Send off Alarm to Center
	{
		while(bit_is_clear(PIND,5));
		 myRadio.stopListening();
		 myRadio.openWritingPipe(addresses[1]);
		 myRadio.write(&_Send_Alarm_Off_1, sizeof(_Send_Alarm_Off_1));
		 myRadio.openReadingPipe(1, addresses[0]);
		 myRadio.startListening();
		 //send('Z');
		 //sbi(PORTC,3);
		
	}
	else if(bit_is_clear(PIND,6))
	{
		while(bit_is_clear(PIND,6));
		cbi(PORTC,WarningLed); // OFF WARNING
	}
	
	else if(bit_is_clear(PIND,7))	// Send Warning to Center
	{
		while(bit_is_clear(PIND,7));
		 myRadio.stopListening();
		 myRadio.openWritingPipe(addresses[1]);
		 myRadio.write(&_Send_W_1, sizeof(_Send_W_1));
		 myRadio.openReadingPipe(1, addresses[0]);
		 myRadio.startListening();
		 //send('Z');
		 sbi(PORTC,WarningLed); // ON WARNING
	}
	
	

}
}
