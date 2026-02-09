#include "Lcd.h"
#include "analog.h"

#define SET_BIT(reg, bit) ((reg) |= (1 << (bit)))
#define CLEAR_BIT(reg, bit) ((reg) &= ~(1 << (bit)))

#define PIN_TECLADO 6 //A1
#define PIN_POTENCIOMETRO 7 //A0
extern volatile byte estadoTecladoAnterior; // Estado anterior del teclado
extern volatile byte estadoTecladoActual; // Estado actual del teclado

void manejarTeclado();

byte leerTecladoFiltrado();

int16_t readPotenciometer();