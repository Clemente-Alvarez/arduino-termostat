#include "lightbulb.h"

const double pi = 3.141592;
volatile uint16_t firingDelayMs = 0; // Retardo de disparo en milisegundos
// --------------------------------------------------------------------
// Configuración inicial del sistema
// --------------------------------------------------------------------
void lightbulbSetup() {
  // Configurar INT6 (D7) como entrada para detección de cruce por cero
  CLEAR_BIT(DDRE, DDE6);  // Configurar INT6 como entrada
  CLEAR_BIT(PORTE, PORTE6); // Sin pull-up

  // Configurar interrupción INT6 para detección de cruce por cero
  EICRB &= ~((1 << ISC61) | (1 << ISC60)); // Flanco ascendente
  EICRB |= (1 << ISC60);
  EIFR |= (1 << INTF6);   // Limpiar bandera de interrupción
  EIMSK |= (1 << INT6);   // Habilitar interrupción INT6

  // Configurar PB5 como salida para controlar el triac
  SET_BIT(DDRB, DDB5);    // Configurar PB5 como salida
  SET_BIT(PORTB, PB5);    // Inicialmente en HIGH (optotriac desactivado)

  // Configurar temporizador principal
  setup_timer3();
}

// --------------------------------------------------------------------
// Interrupción por cruce por cero
// --------------------------------------------------------------------
ISR(INT6_vect) {
    TCNT3 = 0; // Reiniciar contador
    TCCR3B |= (1 << CS31) | (1 << CS30); // Iniciar temporizador con prescaler 64
}

// --------------------------------------------------------------------
// Interrupción del temporizador para disparar el triac
// --------------------------------------------------------------------
ISR(TIMER3_COMPA_vect) {
    CLEAR_BIT(PORTB, PB5);   // Activar optotriac (PB5 = LOW)
    _delay_us(30);           // Pulso corto al LED del optotriac
    SET_BIT(PORTB, PB5);     // Desactivar optotriac (PB5 = HIGH)

    TCCR3B &= ~((1 << CS32) | (1 << CS31) | (1 << CS30)); // Detener temporizador
}

// --------------------------------------------------------------------
// Configuración del temporizador
// --------------------------------------------------------------------
void setup_timer3() {
    TCCR3A = 0;                          // Temporizador en modo normal
    TCCR3B = (1 << WGM32);               // Modo CTC (Clear Timer on Compare Match)
    TIMSK3 = (1 << OCIE3A);              // Habilitar interrupción por comparación
}

// --------------------------------------------------------------------
// Ajuste del porcentaje de brillo
// --------------------------------------------------------------------
void setLightPercent(double f) {
    if (f < 0.0) f = 0.0;
    if (f > 0.9) f = 0.9;

    // Ajustar el valor de brillo para una mejor distribución
    double adjustedF = f; // Relación directa para brillo proporcional

    // Calcular el tiempo de retardo en milisegundos
    double delayMs = (acos(1.0 - 2.0 * (1.0 - adjustedF)) / pi) * 10.0;
    delayMs += 0.3; // Ajustar desfase de corriente
    //if (delayMs < 0.5) delayMs = 0;   // Asegurar retardo mínimo
    //if (delayMs > 9.0) delayMs = 9;   // Limitar retardo máximo

    // Convertir directamente en ticks sin redondear
    OCR3A = (uint16_t)(delayMs * 250); // Milisegundos a ticks
}
