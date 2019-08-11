# NRF_BELL_HOME
Commute 3 boards via NRF24l01 with ATMEGA8.

- I have a TAG (downstair), and 2 ANCHORS(1 in my room [A] and 1 in my young sister's room [B] ).My mom don't need to yell us anymore, she just press the buttons to call us.
- This is a small project to help my mother not tired.  
  
# TAG 
  - Components (1 NRF, 1 ATMEGA8, 3 buttons, 3 leds, 1 buzzer, ..): 
    + This NRF is TAG, commute with 2 ANCHORS
    + This ATMEGA8 is MCU to control NRF and the other devices on board
    + Buttons (Send signal to A, Send signal to B, Turn off warning from A and B)
    + Leds  (Signal check from A, Signal check from B, Warning signal from A and B)
    + Buzzer use for warning situation from A and B.
    
  - Functions:
    + Send signal to A, my mom will press until the TAG's led is on (this mean the ANCHOR A is received and its led is also turned on)
    + Send signal to B, my mom will press until the TAG's led is on (this mean the ANCHOR B is received and its led is also turned on)
    + Turn on TAG's led and buzzer in order to inform my mom need to go upstair.

# ANCHORS A & B 
  - Components (1 NRF, 1 ATMEGA8, 2 buttons, 2 leds, 1 buzzer, ..): 
    + This NRF are ANCHORS, commute with TAG
    + This ATMEGA8 is MCU to control NRF and the other devices on board
    + Buttons (Send signal to TAG to turn off buzzer, Send warning signal to TAG)
    + Leds  (1 led for normal signal , 1 led for warning signal)
    + Buzzer will be turned on when my mom press the button to call us.

# IDE - STIMULINK - RE-EDIT FROM ARDUINO LIB - POWER
  - Atmel Studio 7.0
  - Proteus 8.6
  - NRF24L01 Library in the zip package
  - Adapter 5V-1A
  
