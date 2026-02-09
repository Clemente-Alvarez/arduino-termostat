#include "Lcd.h"
#include "teclado.h"


// Variables de estado y modos de operación
#define MODO_APAGADO 0  // Modo apagado
#define MODO_TERMOSTATO 1  // Modo de termostato 
#define MODO_INTENSIDAD_LUZ 2  // Modo de intensidad de luz
#define MODO_HOME 3 //Modo menu
#define MODO_CONFIG 4 //Modo configuracion
#define MODO_MANUAL 5 //Modo para controlar la bombilla manual
#define MODO_TEMP_LUZ 6 //Submenu para temperatura o luz
#define MODO_ENCENDIDO 7 //Modo Encendido al maximo

#define CONFIG_TECLADO 0
#define CONFIG_POTENCIOMETRO 1

#define DEFAULT_TEMPERATURE 20

extern double Setpoint, Input, Output;
extern int modo_operacion; // Modo de operación

//control de teclado
extern unsigned int vertical;
extern unsigned char config;