#include "Lcd.h"

#define SET_BIT(reg, bit) ((reg) |= (1 << (bit)))
#define CLEAR_BIT(reg, bit) ((reg) &= ~(1 << (bit)))

// Variables de estado y modos de operación
#define MODO_APAGADO 0  // Modo de apagado
#define MODO_ENCENDIDO 1  // Modo de encendido 
#define MODO_CONTROL_MANUAL 2  // Modo de control manual 
#define MODO_TERMOSTATO 3  // Modo de termostato 
#define MODO_POTENCIOMETRO 4  // Modo de potenciómetro

extern const int pinTeclado; // Pin del teclado
extern volatile byte estadoTecladoAnterior; // Estado anterior del teclado
extern volatile byte estadoTecladoActual; // Estado actual del teclado
extern int modo_operacion; // Modo de operación de base

// Variables para el teclado
extern byte key;
extern byte filtro;
extern byte lastKey;
extern int holdTime;

void manejarTeclado();

byte leerTecladoFiltrado();