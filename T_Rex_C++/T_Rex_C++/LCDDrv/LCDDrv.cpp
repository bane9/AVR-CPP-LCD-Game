#include <avr/io.h>
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <util/delay.h>
#include "LCDDrv.h"

static LCDDrv LCD;

void LCDDrv::Init(){	
	_delay_ms(500);
	DDRD |= 0b11110000;
	DDRB |= 0b00000111;
	LCD.BacklightOn();
	LCD.FourBitComd(LCD_INIT8BIT);
	_delay_ms(5);
	LCD.FourBitComd(LCD_INIT8BIT);
	_delay_us(200);
	LCD.FourBitComd(LCD_INIT8BIT);
	_delay_us(200);
	LCD.funcControl = LCD_FUNCTIONSET | LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS; 
	LCD.FourBitComd(LCD.funcControl);
	_delay_us(200);
	LCD.Command(LCD.funcControl);
	LCD.Command(LCD_DISPLAYCONTROL | LCD_DISPLAYOFF);
	LCD.Clear();
	LCD.modeControl = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
	LCD.Command(LCD_ENTRYMODESET | LCD.modeControl);
	LCD.dispControl = LCD_DISPLAYCONTROL | LCD_DISPLAYON;
	LCD.Command(LCD.dispControl);
}

void LCDDrv::SetCursor(uint8_t coll, uint8_t row){
	uint8_t SelectRow = 0x00;
	if(--row) SelectRow = 0x40;
	LCD.Command(LCD_SETDDRAMADDR | (coll + SelectRow));
}


void LCDDrv::Clear(){
	LCD.Command(LCD_CLEARDISPLAY);
	_delay_ms(5);
}

void LCDDrv::Home(){
	LCD.Command(LCD_RETURNHOME);
	_delay_ms(5);
}

void LCDDrv::Write(uint8_t chr){
	PORTB |= (1 << DDB0);
	LCD.FourBitComd(chr);
	LCD.FourBitComd(chr << 4);
	_delay_us(80);
}

void LCDDrv::Command(uint8_t comm){
	PORTB &= ~(1 << DDB0);
	LCD.FourBitComd(comm);
	LCD.FourBitComd(comm << 4);
	_delay_us(80);
}

void LCDDrv::FourBitComd(uint8_t comm){
	PORTD &= 0b00001111;
	if(comm & 1 << 7) PORTD |= (1 << DDD7);
	if(comm & 1 << 6) PORTD |= (1 << DDD6);
	if(comm & 1 << 5) PORTD |= (1 << DDD5);
	if(comm & 1 << 4) PORTD |= (1 << DDD4);
	PORTB |= (1 << DDB1);
	_delay_us(1);
	PORTB &= ~(1 << DDB1);
	_delay_us(1);
}

void LCDDrv::BlinkOn(){
	LCD.dispControl |= LCD_BLINKON;
	LCD.Command(LCD_DISPLAYCONTROL | LCD.dispControl);
}

void LCDDrv::BlinkOff(){
	LCD.dispControl &= ~LCD_BLINKON;
	LCD.Command(LCD_DISPLAYCONTROL | LCD.dispControl);
}

void LCDDrv::DisplayOn(){
	LCD.dispControl |= LCD_DISPLAYON;
	LCD.Command(LCD_DISPLAYCONTROL | LCD.dispControl);
	_delay_ms(5);
}


void LCDDrv::DisplayOff(){
	LCD.dispControl &= ~LCD_DISPLAYON;
	LCD.Command(LCD_DISPLAYCONTROL | LCD.dispControl);
	_delay_ms(5);
}

void LCDDrv::CursorOn(){
	LCD.dispControl |= LCD_CURSORON;
	LCD.Command(LCD_DISPLAYCONTROL | LCD.dispControl);
	_delay_ms(5);
}

void LCDDrv::CursorOff(){
	LCD.dispControl &= ~LCD_CURSORON;
	LCD.Command(LCD_DISPLAYCONTROL | LCD.dispControl);
	_delay_ms(5);
}

void LCDDrv::AddCharacter(uint8_t address, uint8_t *chr){
	if(address > 7) return;
	address = LCD_SETCGRAMADDR + (address * 0x08);
	for(uint8_t i = 0; i < 8; i++){
		LCD.Command(address + i);
		LCD.Write(chr[i]);
	}
	LCD.Command(LCD_SETDDRAMADDR);
}

void LCDDrv::RightToLeft(){
	LCD.modeControl &= ~LCD_ENTRYLEFT;
	LCD.Command(LCD_ENTRYMODESET | LCD.modeControl);
}

void LCDDrv::LeftToRight(){
	LCD.modeControl |= LCD_ENTRYLEFT;
	LCD.Command(LCD_ENTRYMODESET | LCD.modeControl);
}

void LCDDrv::AutoIncrement(){
	LCD.modeControl |= LCD_ENTRYSHIFTINCREMENT;
	LCD.Command(LCD_ENTRYMODESET | LCD.modeControl);
}

void LCDDrv::AutoDecrement(){
	LCD.modeControl &= ~LCD_ENTRYSHIFTINCREMENT;
	LCD.Command(LCD_ENTRYMODESET | LCD.modeControl);
}

void LCDDrv::ShiftDisplayLeft(){
	LCD.Command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

void LCDDrv::ShiftDisplayRight(){
	LCD.Command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

void LCDDrv::ShiftCursorLeft(){
	LCD.Command(LCD_CURSORSHIFT | LCD_CURSORMOVE | LCD_MOVELEFT);
}

void LCDDrv::ShiftCursorRight(){
	LCD.Command(LCD_CURSORSHIFT | LCD_CURSORMOVE | LCD_MOVERIGHT);
}

void LCDDrv::BacklightOn(){
	PORTB |= 1 << DDB2;
}

void LCDDrv::BacklightOff(){
	PORTB &= ~(1 << DDB2);
}