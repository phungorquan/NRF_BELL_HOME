/*
 * RF24.h
 *
 * Created: 6/20/19 10:45:42 PM
 *  Author: Xiu
 */ 


#ifndef RF24_H_
#define RF24_H_

#include "nRF24L01.h"

#define F_CPU 20000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#define sbi(PORT,bit) {PORT |=(1<<bit);}
#define cbi(PORT,bit) {PORT &=~(1<<bit);}
#define tbi(PORT,bit) {PORT ^= (1<<bit);}
#define SCK_PIN		5
#define MOSI_PIN	3
#define MISO_PIN 	4
#define SS			2
#define CSN			1
#define WarningLed  1
#define AlarmBuzzer 3

#define rf24_max(a,b) (a>b?a:b)
#define rf24_min(a,b) (a<b?a:b)


typedef enum { RF24_PA_MIN = 0,RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX, RF24_PA_ERROR } rf24_pa_dbm_e ;

/**
 * Data rate.  How fast data moves through the air.
 *
 * For use with setDataRate()
 */
typedef enum { RF24_1MBPS = 0, RF24_2MBPS, RF24_250KBPS } rf24_datarate_e;

/**
 * CRC Length.  How big (if any) of a CRC is included.
 *
 * For use with setCRCLength()
 */
typedef enum { RF24_CRC_DISABLED = 0, RF24_CRC_8, RF24_CRC_16 } rf24_crclength_e;




class RF24
{
	private:
	bool p_variant; /* False for RF24L01 and true for RF24L01P */
	uint8_t payload_size; /**< Fixed size of payloads */
	bool dynamic_payloads_enabled; /**< Whether dynamic payloads are enabled. */
	uint8_t pipe0_reading_address[5]; /**< Last address set on pipe 0 for reading. */
	uint8_t addr_width; /**< The address width to use - 3,4 or 5 bytes. */
	uint16_t ce_pin; /**< "Chip Enable" pin, activates the RX or TX role */
	uint16_t csn_pin; /**< SPI Chip select */
	uint16_t spi_speed; /**< SPI Bus Speed */
	
	public:
	uint32_t csDelay;
	uint32_t txDelay;
	 RF24(uint16_t _cepin, uint16_t _cspin);
	 
	  bool begin(void);
	  void startListening(void);
	  void stopListening(void);
	   bool available(void);
	   void setPALevel ( uint8_t level );
	   void openWritingPipe(const uint8_t *address);
	  // void openWritingPipe(uint64_t address);
	   void setChannel(uint8_t channel);
	   void setRetries(uint8_t delay, uint8_t count);
	   uint8_t flush_tx(void);
	   void toggle_features(void);
	   uint8_t spiTrans(uint8_t cmd);
	   void closeReadingPipe( uint8_t pipe ) ;
	     void powerUp(void) ;
		 uint8_t read_register(uint8_t reg, uint8_t* buf, uint8_t len);
		 uint8_t write_register(uint8_t reg, const uint8_t* buf, uint8_t len);

		 uint8_t write_register(uint8_t reg, uint8_t value);
		uint8_t read_register(uint8_t reg);
	   bool setDataRate(rf24_datarate_e speed);
	   //void openReadingPipe(uint8_t number, uint64_t address);
	   void openReadingPipe(uint8_t number, const uint8_t *address);
	   uint8_t flush_rx(void);
	    bool write( const void* buf, uint8_t len );
		  bool write( const void* buf, uint8_t len, const bool multicast );
	    bool available(uint8_t* pipe_num);
		void read( void* buf, uint8_t len );
		  uint8_t read_payload(void* buf, uint8_t len);
		uint8_t get_status(void);
		void powerDown(void);
		 void startFastWrite( const void* buf, uint8_t len, const bool multicast, bool startTx = 1 );

		    uint8_t write_payload(const void* buf, uint8_t len, const uint8_t writeType);
	};



#endif /* RF24_H_ */