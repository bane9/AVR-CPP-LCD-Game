#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <string.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "LCDDRV/LCDDrv.h"
#define reverse true
#define normal false

LCDDrv LCD;
volatile uint8_t goUp = false, canUp = true, longhold = false;

template <class T>
void printNum(T num, bool rev);
void printString(char *chr, bool rev);
void printString(char *chr);
void printScore(uint16_t score);
uint16_t aRead();

int main(void){
	ADMUX |= (1 << REFS0);
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) | (1 << ADEN);
	
	OCR0A = 127;
	TCCR0A = (1 << WGM01);
	TCCR0B = (1 << CS01) | (1 << CS00);
	TIMSK0 = (1 << OCIE0A);
	sei();
	
	LCD.Init();
	uint8_t din[] = {0x0E, 0x17, 0x1E, 0x1F, 0x18, 0x1F, 0x1A, 0x12}, cact[] = {0x04, 0x05, 0x15, 0x15, 0x16, 0x0C, 0x04, 0x04};
	char downBuff[15];
	for(uint8_t i = 0; i < 16; i++) downBuff[i] = ' ';
	
	LCD.AddCharacter(0x01, din);
	LCD.AddCharacter(0x02, cact);
	
	const uint8_t dino = 0x01, cactus = 0x02;
	uint8_t distance = 6, speed = 200, dontprint = 0;
	uint16_t aVal = 0, score = 1, bestscore = 0;
	
	while (1) {
		
		ADMUX |= (1 << MUX2) | (1 << MUX1);
		srand(aRead());
		ADMUX &= ~(1 << MUX2) & ~(1 << MUX1);
		
		if(aRead() > 900) longhold = false;
		for(uint8_t  i= 0; i < 15; i++) downBuff[i] = downBuff[i + 1];
		if((rand() % 100) > (rand() % 100)  && !dontprint){
			downBuff[15] = cactus;
			dontprint++;
		}
		else downBuff[15] = ' ';
		char lastchar = downBuff[3];
		if(!goUp){
				downBuff[3] = dino;
				LCD.SetCursor(0, 2);
				printString(downBuff);
				downBuff[3] = lastchar;
				canUp = true;
			} else {
				LCD.SetCursor(3, 1);
				LCD.Write(dino);
				LCD.SetCursor(0, 2);
				printString(downBuff);
				canUp = false;
		}

		if(dontprint) dontprint++;
		if(dontprint > distance) dontprint = 0;
		
		if(goUp) goUp++;
		if(goUp > 4){
			LCD.SetCursor(3, 1);
			LCD.Write(' ');
			goUp = 0;
		}

		score++;
		if(score > bestscore) bestscore = score;
		printScore(score);
		
		if(lastchar == cactus && !goUp){
			LCD.Clear();
			LCD.SetCursor(0, 1);
			printString("Score: ");
			printNum(score, normal);
			LCD.SetCursor(0, 2);
			printString("Best: ");
			printNum(bestscore, normal);
			while (1){
				aVal = aRead();
				if(aVal > 635 && aVal < 645){
					LCD.Clear();
					for(uint8_t i = 0; i < 15; i++) downBuff[i] = ' ';
					dontprint = 0;
					goUp = 0;
					score = 1;
					speed = 200;
					longhold = 0;
					distance = 6;
					canUp = 1;
					break;
				}
			}
		}
		
		if(score % 5 == 0) speed -=2;
		if(speed < 85) speed = 85;
		if(score % 175 == 0) distance--;
		if(distance < 5) distance = 5;
		for(uint8_t i = 0; i < speed; i++) _delay_ms(1);
		
	}
}

template <class T>
void printNum(T num, bool rev){
	if(num < 0){
		LCD.Write('-');
		num = -num;
	}
	uint8_t pastzero = 0, cnum, cnt = 0;
	char buff[10];
	for(uint8_t i = 0; i < 10; i++) buff[i] = 255;
		for(long i = 1000000000L; i > 0; i /=10){
			cnum = (num / i) % 10;
			if(cnum)pastzero = 1;
			if(pastzero) buff[cnt] = cnum + '0';
			cnt++;
		}
	if(!pastzero) buff[0] = '0';
	buff[10] = '\0';
	if(rev) printString(buff, rev);
	else printString(buff);
}

void printString(char *chr, bool rev){
	for(int8_t i = strlen(chr) - 1; i >= 0; i--)
		if(chr[i] != 255)
			LCD.Write(chr[i]);
		
}

void printString(char *chr){
	for(uint8_t i = 0; i < strlen(chr); i++)
		if(chr[i] != 255)
			LCD.Write(chr[i]);
}

void printScore(uint16_t score){
	LCD.RightToLeft();
	LCD.SetCursor(15, 1);
	printNum(score, reverse);
	printString("Score:", reverse);
	LCD.LeftToRight();
}

uint16_t aRead(){
	ADCSRA |= (1 << ADSC);
	while  (ADCSRA & (1 << ADSC));
	return ADCL | (ADCH << 8);
}

ISR(TIMER0_COMPA_vect){
		uint16_t aVal = aRead();
		if(aVal > 95 && aVal < 104 && canUp && !longhold){
			goUp = true;
			canUp = false;
			longhold = true;
		}
}
