//#include "timer.h"
//---------------------------------------- LCD -------------------------------------------------
#define SET_BIT(reg, bit) ((reg) |= (1 << (bit)))
#define CLEAR_BIT(reg, bit) ((reg) &= ~(1 << (bit)))

// Definimos los pines del Arduino para la conexión LCD
#define setPORT_RS(bit) if(bit) SET_BIT(PORTD, PORTD6); else CLEAR_BIT(PORTD, PORTD6); //RS pin d12 
#define setDDR_RS(bit) if(bit) SET_BIT(DDRD, DDD6); else CLEAR_BIT(DDRD, DDD6);

#define setPORT_RW(bit) if(bit) SET_BIT(PORTB, PORTB7); else CLEAR_BIT(PORTB, PORTB7); //RW pin d11
#define setDDR_RW(bit) if(bit) SET_BIT(DDRB, DDB7); else CLEAR_BIT(DDRB, DDB7);

#define setPORT_E(bit) if(bit) SET_BIT(PORTB, PORTB6); else CLEAR_BIT(PORTB, PORTB6); //E pin d10
#define setDDR_E(bit) if(bit) SET_BIT(DDRB, DDB6); else CLEAR_BIT(DDRB, DDB6);

#define setPORT_D4(bit) if(bit) SET_BIT(PORTC, PORTC6); else CLEAR_BIT(PORTC, PORTC6); //D4 pin d5
#define setDDR_D4(bit) if(bit) SET_BIT(DDRC, DDC6); else CLEAR_BIT(DDRC, DDC6);

#define setPORT_D5(bit) if(bit) SET_BIT(PORTD, PORTD4); else CLEAR_BIT(PORTD, PORTD4); //D5 pin d4
#define setDDR_D5(bit) if(bit) SET_BIT(DDRD, DDD4); else CLEAR_BIT(DDRD, DDD4);

#define setPORT_D6(bit) if(bit) SET_BIT(PORTD, PORTD0); else CLEAR_BIT(PORTD, PORTD0); //D6 pin d3
#define setDDR_D6(bit) if(bit) SET_BIT(DDRD, DDD0); else CLEAR_BIT(DDRD, DDD0);

#define setPORT_D7(bit) if(bit) SET_BIT(PORTD, PORTD1); else CLEAR_BIT(PORTD, PORTD1); //D7 pin d2
#define setDDR_D7(bit) if(bit) SET_BIT(DDRD, DDD1); else CLEAR_BIT(DDRD, DDD1);

#define D7 PIND1
#define registerD7 PIND
// ---------------------------------------- END LCD -------------------------------------------------
// Declaraciones de las funciones que vamos a usar más tarde
void lcd_init();
void lcd_command(uint8_t cmd);
void lcd_write(uint8_t data);
void lcd_setCursor(uint8_t col, uint8_t row);
void lcd_print(const char *str);
void lcd_write4bits(uint8_t nibble);
void lcd_clear();
void lcd_home();
void lcd_displayOn();
void lcd_displayOff();
void lcd_cursorOn();
void lcd_cursorOff();
void lcd_blinkOn();
void lcd_blinkOff();
void lcd_scrollDisplayLeft();
void lcd_scrollDisplayRight();
void lcd_createChar(uint8_t location, uint8_t charmap[]);
void pulseEnable();
bool lcd_checkIfBusy();