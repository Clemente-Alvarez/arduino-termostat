#include "UI.h"

extern int modo_operacion = MODO_APAGADO; // Modo de operación de base

void lcd_actualizarDisplay() {
  lcd_clear();
  lcd_setCursor(0, 0);

  switch (modo_operacion) {
    //Ejemplo de lo que se deberia de mostrar por pantalla siendo || el borde fisicao del display
    /* 
     * ||Apagado
     * ||Temp: 0.0
     * ||
     * ||
     */
    case MODO_APAGADO:
      lcd_print("Apagado");
      lcd_setCursor(0, 1);
      lcd_print("Temp: ");
      lcd_print(String(Input).c_str());
      break;

    /*
     * ||>Modo Temp/Luz
     * || Modo Encendido
     * || Modo Configuracion
     * || Modo Manual
     */
    case MODO_HOME:
      lcd_print(" Modo Temp/Luz");
      lcd_setCursor(0, 1);
      lcd_print(" Modo Encendido");
      lcd_setCursor(0, 2);
      lcd_print(" Modo Configuracion");
      lcd_setCursor(0, 3);
      lcd_print(" Modo Manual");
      lcd_setCursor(0, vertical%4);
      lcd_print(">");
      break;

    /*
     * ||Modo Termostato
     * ||Temp: 0.0
     * ||Objetivo: 20
     * ||
     */
    case MODO_TERMOSTATO:
      lcd_print("Modo Termostato");
      lcd_setCursor(0, 1);
      lcd_print("Temp: ");
      lcd_print(String(Input).c_str());
      lcd_setCursor(0, 2);
      lcd_print("Objetivo: ");
      if(config == CONFIG_TECLADO) Setpoint = (5 * vertical);
      lcd_print(String(Setpoint).c_str());
      break;

    /*
     * ||Modo Intensidad Luz
     * ||Light: 205
     * ||Objetivo: 425
     * ||
     */
    case MODO_INTENSIDAD_LUZ:
      lcd_print("Modo Intensidad Luz");
      lcd_setCursor(0, 1);
      lcd_print("Light: ");
      lcd_print(String(Input).c_str());
      lcd_setCursor(0, 2);
      lcd_print("Objetivo: ");
      if(config == CONFIG_TECLADO) Setpoint = vertical %1024;
      lcd_print(String(Setpoint).c_str());
      break;
    
    /*
     * ||Modo de uso
     * || *Teclado
     * ||> Potenciometro
     * ||
     */
    case MODO_CONFIG:
      lcd_print("Modo de uso");
      lcd_setCursor(0, 1);
      lcd_print("  Teclado");
      lcd_setCursor(0, 2);
      lcd_print("  Potenciometro");
      lcd_setCursor(0, vertical%2 +1);
      lcd_print(">");
      lcd_setCursor(1, config%2 +1);
      lcd_print("*");
      break;

    /*
     * ||Modo Manual
     * ||%: 20
     * ||
     * ||
     */
    case MODO_MANUAL:
      lcd_print("Modo Manual");
      lcd_setCursor(0, 1);
      lcd_print("%: ");
      lcd_print(String(vertical%101).c_str());
      break;

    /*
     * ||>Modo Temperatura
     * || Modo Intnesidad Luz
     * ||
     * ||
     */
    case MODO_TEMP_LUZ:
      lcd_print(" Modo Temperatura");
      lcd_setCursor(0, 1);
      lcd_print(" Modo Intnesidad Luz");
      lcd_setCursor(0, vertical%2);
      lcd_print(">");
      break;
    /*
     * ||Modo Encendido
     * ||  
     * ||
     * ||
     */
    case MODO_ENCENDIDO:
      lcd_print("Modo Encendido");
  }
}

byte lastKey = 0;
byte key = 0;

void manejarTeclado() {
  key = leerTecladoFiltrado();
  if(lastKey != key){
    switch (key) {
      case 1: // boton de apagar/encender
        vertical = 0;
        if(modo_operacion == MODO_APAGADO) modo_operacion = MODO_HOME;
        else modo_operacion = MODO_APAGADO;
        break;
      case 2://boton de aumentar 
        vertical--;
        break;
      case 3: //boton de reducir
        vertical++;
        break;
      case 4: //boton de confirmar
        switch(modo_operacion){
          case MODO_HOME:
          switch(vertical%4){
            case 0: modo_operacion = MODO_TEMP_LUZ;
            vertical = DEFAULT_TEMPERATURE/5;
            break;
            case 1: modo_operacion = MODO_ENCENDIDO;
            vertical = 0;
            break;
            case 2: modo_operacion = MODO_CONFIG;
            vertical = 0;
            break;
            case 3: modo_operacion = MODO_MANUAL;
            vertical = 101;
          }
          break;
          case MODO_CONFIG:
          
          switch(vertical%2){
            case 0: config = CONFIG_TECLADO;
            break;
            case 1: config = CONFIG_POTENCIOMETRO;
          }
          break;
          case MODO_TEMP_LUZ:
          switch(vertical%2){
            case 0: modo_operacion = MODO_TERMOSTATO;
            vertical = DEFAULT_TEMPERATURE/5;
            break;
            case 1: modo_operacion = MODO_INTENSIDAD_LUZ;
            vertical = 0;
          }
          break;
        }
        break;
      case 5: //boton de menu
        modo_operacion = MODO_HOME;
        break;
    }
  }
  lastKey = key;
}