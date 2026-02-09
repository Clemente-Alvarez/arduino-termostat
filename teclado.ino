#include "teclado.h"

// Variables para el teclado
byte filtro = 5;
int holdTime = 0;

int valorAnalogico = 0;

// ---------------------------------------- TECLADO -------------------------------------------------
// Función de lectura del teclado con filtro para evitar el rebote de la tecla presionada
// Verificar
byte leerTecladoFiltrado() {
  byte botonDetectado;
  byte consecutive = 0; // Conteo de muestras consecutivas
  byte mostRecent = 0;

  while (true) {
    valorAnalogico = customAnalogRead(PIN_TECLADO); // Leer el valor del teclado conectado al pin A1
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

int16_t readPotenciometer(){
  return customAnalogRead(PIN_POTENCIOMETRO);
}