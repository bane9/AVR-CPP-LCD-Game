#ifndef LCDDRV_H
#define LCDDRV_H

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_INIT8BIT 0x30
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_5x8DOTS 0x00

class LCDDrv{
	public:
	void Init();
	void SetCursor(uint8_t line, uint8_t coll);
	void Clear();
	void Home();
	void Write(uint8_t chr);
	void Command(uint8_t comm);
	void FourBitComd(uint8_t comm);
	void BlinkOn();
	void BlinkOff();
	void DisplayOn();
	void DisplayOff();
	void AddCharacter(uint8_t address, uint8_t *chr);
	void CursorOn();
	void CursorOff();
	void RightToLeft();
	void LeftToRight();
	void AutoIncrement();
	void AutoDecrement();
	void ShiftDisplayLeft();
	void ShiftDisplayRight();
	void ShiftCursorLeft();
	void ShiftCursorRight();
	void BacklightOn();
	void BacklightOff();
	uint8_t dispControl, funcControl, modeControl;
	};
#endif