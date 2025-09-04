#include "teclado.h"


extern const int pinTeclado = A1; // Pin del teclado
//extern volatile byte estadoTecladoAnterior = LOW; // Estado anterior del teclado
extern int modo_operacion = MODO_APAGADO; // Modo de operación de base

// Variables para el teclado
extern byte filtro = 15;
extern byte lastKey = 0;
extern int holdTime = 0;
// ---------------------------------------- TECLADO -------------------------------------------------
void manejarTeclado() {
  key = leerTecladoFiltrado();
  if (key != lastKey) {
    switch (key) {
      case 1:
        modo_operacion = MODO_ENCENDIDO;
        lcd_actualizarDisplay(MODO_ENCENDIDO);
        break;
      case 2:
        modo_operacion = MODO_CONTROL_MANUAL;
        lcd_actualizarDisplay(MODO_CONTROL_MANUAL);
        break;
      case 3:
        modo_operacion = MODO_TERMOSTATO;
        lcd_actualizarDisplay(MODO_TERMOSTATO);
        break;
      case 4:
        modo_operacion = MODO_POTENCIOMETRO;
        lcd_actualizarDisplay(MODO_POTENCIOMETRO);
        break;
      case 5:
        modo_operacion = MODO_APAGADO;
        lcd_actualizarDisplay(MODO_APAGADO);
        break;
    }
    lastKey = key;
    delay(500); // Para dar tiempo de ver el mensaje
  }
}
// Función de lectura del teclado con filtro para evitar el rebote de la tecla presionada
// Verificar
byte leerTecladoFiltrado() {
  int valorAnalogico;
  byte botonDetectado;
  byte consecutive = 0; // Conteo de muestras consecutivas
  byte mostRecent = 0;

  while (true) {
    valorAnalogico = analogRead(pinTeclado); // Leer el valor del teclado conectado al pin A1

    // Verifica el rango de voltaje para determinar qué botón está presionado
    if (valorAnalogico > 900) {
      if (mostRecent != 0) {
        mostRecent = 0;
        consecutive = 0;
      } else if (consecutive > filtro) {
        botonDetectado = 0;
        break;
      } else consecutive++;
    } 
    else if (valorAnalogico > 700) {
      if (mostRecent != 5) {
        mostRecent = 5;
        consecutive = 0;
      } else if (consecutive > filtro) {
        botonDetectado = 5;
        break;
      } else consecutive++;
    } 
    else if (valorAnalogico > 480) {
      if (mostRecent != 4) {
        mostRecent = 4;
        consecutive = 0;
      } else if (consecutive > filtro) {
        botonDetectado = 4;
        break;
      } else consecutive++;
    }
     else if (valorAnalogico > 300) {
      if (mostRecent != 3) {
        mostRecent = 3;
        consecutive = 0;
      } else if (consecutive > filtro) {
        botonDetectado = 3;
        break;
      } else consecutive++;
    } 
    else if (valorAnalogico > 135) {
      if (mostRecent != 2) {
        mostRecent = 2;
        consecutive = 0;
      } else if (consecutive > filtro) {
        botonDetectado = 2;
        break;
      } else consecutive++;
    } 
    else {
      if (mostRecent != 1) {
        mostRecent = 1;
        consecutive = 0;
      } else if (consecutive > filtro) {
        botonDetectado = 1;
        break;
      } else consecutive++;
    }
  }
  return botonDetectado;
}