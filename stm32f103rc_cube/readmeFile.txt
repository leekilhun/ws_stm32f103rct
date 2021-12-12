

OBJECT List 
=================================================================
STM32103RCT 

uart 1 		- USB 		(D-		PA11, D+	PA12)
uart 2 		- uart1 	(TX 	PA9	, RX	PA10), DMA Receive

LED 1 		- D0	(PA8)
LED 2 		- D1	(PD2)

Button 1 	- KEY0 	(PC1)
Button 2 	- KEY1 	(PC13)


@spi3(slave)
 #	Name	Function	Connected to
 1	SCK		-			PB3
 2	SO		-			PB4
 3	SI		-			PB5
 4	CS		input		PB6

@can
 #	Name	Function	Connected to
 1	can_rx	-			PB8
 2	can_tx	-			PB9


@LCD1602 BLUE
- PCF8574 I2C Communication
	VCC	-	5V
	GND	-	GND
	SDA	-	PB6
	SCL	-	PB7


@ OLED or TFT LCD header properties  (Pin header	2.54mm, 8x1, female)
	#	Name	Function	Connected to
	1	3V		-			+3.3V rail
	2	GND		-			Ground plane
	3	SDI		-			PB15
	4	SCL		-			PB13
	5	CS		-			PB12
	6	RST		-			NRST
	7	A0		-			PC6
	8	NC		-			N.C.


@ nRF24L01 module header properties  (Pin header 2.54mm, 4x2, female)
	Pin		Name	Function	Connected to
	1		GND		GND			Ground plane
	2		3V3		VCC			+3.3V rail
	3		PE4		CE			PA4
	4		PE2		CSN			PC4
	5		PB3		SCK			PA5
	6		PB5		MOSI		PA7
	7		PB4		MISO		PA6
	8		PC13	IRQ			PC

@ Serial ROM
-W25Q16JV properties	2MiB Dual/Quad SPI FLASH
	Pin		Name	Function	Connected to
	1		-		/CS			PA2 pulled up +3.3V rail via 10kΩ resistor
	2		-		DO			PA6
	3		-		/WP			+3.3V rail
	4		-		GND			Ground plane
	5		-		DI			PA7
	6		-		CLK			PA5
	7		-		/HOLD		+3.3V rail
	8		-		VCC			+3.3V rail