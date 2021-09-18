# TeensyMM SparkFun Input and Display Board Demo

![Demo Picture](/TeensyMM_Demo_small.jpg)

This program demonstrates the SparkFun Input and Display Board with a Teensy processor with the following features:

* 2.4" TFT ILI9341 display
* various buttons/joystick
* microSD slot
* 6 APA102 LEDs
* Buzzer

It requires the following libraries to be loaded via Tools -> Manage Libraries...

* Wire \*
* ILI9341_t3 \*
* SD \*
* SparkFun_MicroMod_Button
* Adafruit_DotStar

\* These libraries may already be installed with Teensyduino.

Pins for Input and Display Board:

|Teensy Pin	|MM Pin	|MM Name	|Display Carrier
|---------------|-------|---------------|---------------
| 0	|19	|UART_RX1 (I)	
| 1	|17	|UART_TX1 (O)	
| 2	|47	|PWM1	|Buzzer
| 3	|32	|PWM0	|Display Backlight
| 4	|10	|D0	|Display CS
| 5	|18	|D1	|Display D/C
| 6	|71	|G6	
| 7	|56	|AUD_OUT	
| 8	|54	|I2S_SDI	
| 9	|69	|G7	
|10	|55	|SPI_CS#	|uSD CS
|11	|59	|SPI_COPI	|Display
|12	|61	|SPI_CIPO	|Display
|13	|57	|SPI_SCK	|Display
|14/A0	|34	|A0	
|15/A1	|38	|A1	
|16/A2	|20	|UART_RX2 (I)	
|17/A3	|22	|UART_TX2 (O)	
|18/A4	|12	|I2C_SDA (Qwiic)	|ATTiny 0x71
|19/A5	|14	|I2C_SCL (Qwiic)	|ATTiny
|20/A6	|52	|I2S_WS	
|21/A7	|50	|I2S_SCL	
|22/A8	|49	|VIN/3  ADC	
|23/A9	|58	|MCLK	
---|			
|24/A10	|53	|SCL1	
|25/A11	|51	|SDA1	
|26/A12	|67	|G8	
|27/A13	|8	|G11	
|28	|4	|3.3V	
|29	|16	|I2C_INT	|Button INT
|30	|41	|CAN_RXI	
|31	|43	|CAN_TXO	
|32	|65	|G9	
|33	|63	|G10	
|SD Pins			
|34	|66		
|35	|64		
|36	|60		
|37	|62		
|38	|68		
|39	|70		
|MM new pins			
|40	|40	|G0	|APA102 Clock
|41	|42	|G1	|APA102 Data
|42	|44	|G2	|Display Reset#
|43	|46	|G3	
|44	|48	|G4	
|45	|73	|G5	
