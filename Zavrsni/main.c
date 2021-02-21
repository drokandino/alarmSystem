#define F_CPU 7372800UL

#define keypadDirectionReg DDRB
#define keypadPortControl PORTB
#define keypadPortValue PINB

#define PASSWORDLENGTH 4
#define NONE 'n' //Represents that nothing is pressed on keypad

#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include <avr/io.h>
#include "lcd.h"

char lcdString[16];  //String that is written to lcd
char alarmPassword[PASSWORDLENGTH] = {'0', '0', '0', '0'}; //Alarm deactivate password, default password is 0000
char password[PASSWORDLENGTH]; //Potential alarm password, one which is written to deactivate alarm
uint8_t numberOfDigits = 0; //Used to index password strings
uint8_t modeOfOperationPolling = 1;  //States whether main loop should poll for modes changes, could be either 0 or 1
uint8_t countdownEnable = 0; //Tells if timer ISR should countdown countdownValue variable
uint8_t countdownValue = 65; //value which is decremented in alarmMode on, represents time passed since alarm tirgger
char modeOfOperation = 'A';  //Modes of operations are: A - Welcome screen, B - Set alarm password, C - Activate alarm
uint8_t alarmTriggered = 0; 

char getPressedKey(){
	
	//default key value is 'n', or NONE
	char pressedKey = NONE;
	uint8_t keyPressCode;
	
	//Reading rows	
	keypadDirectionReg = 0b00001111;
	keypadPortControl = 0b11110000;  
	_delay_ms(50);					 
										
	
	//Represents keypadPortValue from columns and rows combined 4 higher bits represent rows and 4 lower bits are columns
	keyPressCode = keypadPortValue;
	
	//Reading columns
	keypadDirectionReg ^= 0b11111111;
	keypadPortControl ^= 0b11111111;
	_delay_ms(50);
	
	keyPressCode |= keypadPortValue;
	
	if (keyPressCode == 255) return NONE;
	else if(keyPressCode == 0b01110111) pressedKey = '1';
	else if(keyPressCode == 0b01111011) pressedKey = '2';
	else if(keyPressCode == 0b01111101) pressedKey = '3';
	else if(keyPressCode == 0b01111110) pressedKey = 'A';
	else if(keyPressCode == 0b10110111) pressedKey = '4';
	else if(keyPressCode == 0b10111011)	pressedKey = '5';
	else if(keyPressCode == 0b10111101) pressedKey = '6';
	else if(keyPressCode == 0b10111110) pressedKey = 'B';
	else if(keyPressCode == 0b11010111) pressedKey = '7';
	else if(keyPressCode == 0b11011011) pressedKey = '8';
	else if(keyPressCode == 0b11011101) pressedKey = '9';
	else if(keyPressCode == 0b11011110) pressedKey = 'C';
	else if(keyPressCode == 0b11100111) pressedKey = '*';
	else if(keyPressCode == 0b11101011) pressedKey = '0';
	else if(keyPressCode == 0b11101101) pressedKey = '#';
	else if(keyPressCode == 0b11101110) pressedKey = 'D';
	
	return pressedKey;
}

void lcdInit(){
	
	DDRD = _BV(4);
	TCCR1A = _BV(COM1B1) | _BV(WGM10);
	TCCR1B = _BV(WGM12) | _BV(CS11);
	OCR1B = 128;
	DDRA  = 0b00001111;
	lcd_init(LCD_DISP_ON);
	lcd_clrscr();
}

void setPasswordModeInit(){
	
	lcd_clrscr();
	lcd_puts("Set 4 digit Pass:");
	lcd_gotoxy(0, 1);
	
	numberOfDigits = 0;
	_delay_ms(300);
}

void alarmOnModeInit(){
	
	lcd_clrscr();
	lcd_puts("ALARM ON!!");
	
	numberOfDigits = 0;
	modeOfOperationPolling = 0;
	alarmTriggered = 0;
	
	countdownEnable = 0;
	countdownValue = 65;
	
	_delay_ms(500);
}

void welcomeMode(){
	
	lcd_clrscr();
	//Intial LCD message
	lcd_gotoxy(0, 0);
	lcd_puts("#RiTeh Security#");
	lcd_gotoxy(0, 1);
	lcd_puts("Chose Mode:B | C");
	
}


void setAlarmPassword(){
	//Disable mode of opertaion changing
	modeOfOperationPolling = 0;
	
	//Poll for pressed key, NONE represents that nothing is pressed
	char pressedKey	= getPressedKey();
	if(pressedKey != NONE && numberOfDigits < PASSWORDLENGTH){
		//Print pressed key
		sprintf(lcdString, "%c", pressedKey);
		lcd_puts(lcdString);
		
		//Set password character
		alarmPassword[numberOfDigits] = pressedKey;
		numberOfDigits++;
		
		_delay_ms(300);
	}
	
	//If password is written(set)
	if(numberOfDigits == PASSWORDLENGTH){
		lcd_clrscr();
		lcd_gotoxy(0, 0);
		lcd_puts("Password is set");
		
		//Print password
		lcd_gotoxy(0, 1);
		sprintf(lcdString, "%c%c%c%c", alarmPassword[0], alarmPassword[1], alarmPassword[2], alarmPassword[3]);
		lcd_puts(lcdString);
		
		modeOfOperationPolling = 1;
		modeOfOperation = 'A';
		_delay_ms(1000);
		welcomeMode();
		
	}
	
}

void robbery(){
	DDRA = 0xff;
	PORTA = 0xff;
	PORTC = 0b00000001;
	
	lcd_clrscr();
	lcd_puts("!Calling Police!");
	lcd_gotoxy(0, 1);
	lcd_puts("!#!#!#!#!#!#!#!#");
	
	while(1) {
		PORTA = 0xff;
		_delay_ms(200);
		PORTA ^= 0xff;
		_delay_ms(200);
		PORTA ^= 0xff;
		_delay_ms(200);
		PORTA ^= 0xff;
		_delay_ms(200);
		
		PORTA = 0b01010101;
		_delay_ms(200);
		PORTA ^= 0xff;
		_delay_ms(200);
		PORTA ^= 0xff;
		_delay_ms(200);
	}
}

void alarmOn(){
	
	if(bit_is_clear(PINC, 0) && alarmTriggered == 0){
		alarmTriggered = 1;
		lcd_clrscr();
		lcd_puts("!!!!!!!!!!!!!!!");
		lcd_gotoxy(0, 1);
		countdownEnable = 1;
		PORTC &= 0b00000001;
	}
	
	if(alarmTriggered == 1){
		PORTC ^= 0b10000000;
	}
	
	if(countdownEnable){
		countdownValue--;
		
		if(countdownValue == 0){
			countdownEnable = 0;
			robbery();
		}
		_delay_ms(100);
		
	}
	
	char pressedKey = getPressedKey();
	if(pressedKey != NONE && numberOfDigits < PASSWORDLENGTH){
		lcd_puts("*");
		
		password[numberOfDigits] = pressedKey;
		pressedKey = NONE;
		numberOfDigits++;
		_delay_ms(300);
	}
	
	if(numberOfDigits == PASSWORDLENGTH){
		if(password[0] == alarmPassword[0] 
		&& password[1] == alarmPassword[1]
		&& password[2] == alarmPassword[2] 
		&& password[3] == alarmPassword[3])
		{
			lcd_clrscr();
			lcd_puts("Alarm off!");
			PORTC = 0b10000001;
			_delay_ms(1000);
			modeOfOperationPolling = 1;
			countdownEnable = 0;
			modeOfOperation = 'A';
			welcomeMode();
			
		}
		else {
			lcd_clrscr();
			lcd_puts("Wrong password!");
			numberOfDigits = 0;
			_delay_ms(2000);
			lcd_clrscr();
			lcd_puts("!!!!!!!!!!!!!!!");
			lcd_gotoxy(0, 1);
		}
	}
	
}


int main(void){	
	
	lcdInit();
	//Local variables
	char pressedKey = NONE; //NONE represents none or nothing is pressed
											
	//Sound sensor and buzzer init
	DDRC = 0b11111110;
	PORTC = 0b10000001;
	
	//defalut mode
	welcomeMode();
								 
	while (1){
		
		if(modeOfOperationPolling){
			pressedKey = getPressedKey();
			
			if(pressedKey == 'A'){
				welcomeMode();
				modeOfOperation = pressedKey;
			}
			
			else if(pressedKey == 'B'){
				setPasswordModeInit();
				modeOfOperation = pressedKey;
				pressedKey = 'n';	 
			}
			
			else if(pressedKey == 'C'){
				alarmOnModeInit();
				modeOfOperation = pressedKey;
			}
		}
		
		switch(modeOfOperation){
			case 'A':
				break;
				
			case 'B':
				setAlarmPassword();
				break;
			
			case  'C':
				alarmOn();
				
		}					
	}
}

