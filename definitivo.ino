#include <Arduino.h>
#include <PID_v1.h> // PID
#include "teclado.h"
#include "timer.h"


// ----------------- INT0 ---------------------
#define INT0 0 // Identificación de INT0
#define ISC01 1 // Bit de configuración para flanco descendente
#define ISC00 0 // Otro bit de configuración para INT0

extern volatile byte estadoTecladoActual = 0;
extern volatile byte estadoTecladoAnterior = 0;
// *------------------------------------------*

// Variables de estado y modos de operación
#define NUM_MUESTRAS 10
#define PI 3.14159

// Variables para el teclado
byte key;

// Parámetros para los controladores PID de temperatura y luz
double SetpointTemperatura, InputTemperatura, OutputTemperatura;
double Setpoint, Input, Output;
double KpT = 2.0, KiT = 5.0, KdT = 1.0; // Temperatura PID
double KpL = 1.0, KiL = 1.0, KdL = 0.1; // Luz PID

// Definición de los PID
PID pidTemperatura(&InputTemperatura, &OutputTemperatura, &SetpointTemperatura, KpT, KiT, KdT, DIRECT);
PID pidLuz(&Input, &Output, &Setpoint, KpL, KiL, KdL, DIRECT);

// Definición de pines para los sensores y actuadores
#define pinCalefactor 9
#define pinPotenciometro A1

#define USAR_SENSOR_TEMPERATURA 1  // 1 para temperatura, 0 para luz
#if USAR_SENSOR_TEMPERATURA == 1
  #define pinSensorTemperatura A0
#else
  #define pinSensorLuz A0
#endif

// ---------------------------------------- FUNCIONES TERMOSTATO -------------------------------------------------
// Función para ajustar el porcentaje de encendido
// Definición de pines
#define PIN_CRUCECERO 8 // Entrada del cruce por cero
#define PIN_CALIFACTOR 3  // Asumimos que el calefactor está conectado al pin digital 3
#define PORT_CALIFACTOR PORTD // Asumiendo que el calefactor está en PORTD

// Variables para control de cruce y potencia
volatile double retardoEncendido = 0;
volatile bool encenderCalefactor = false;
byte estadoAnterior = HIGH;
byte estadoActual = HIGH;

// Función para establecer la potencia del calefactor (0 - 100%)
void setIntensidadCalefactor(double intensidad) {
  // Calcula el retardo en función del porcentaje de energía deseado
  retardoEncendido = 10000 * (PI - acos(1 - 2 * intensidad / 100)) / PI;
  encenderCalefactor = intensidad > 0;  // Permite el encendido si la potencia es mayor a 0
}

// Interrupción para la lectura del teclado
ISR(INT0_vect) {
  // Leer el estado del teclado
  estadoTecladoActual = leerTecladoFiltrado();

  // Si se ha detectado un cambio en el estado del teclado (de LOW a HIGH)
  if (estadoTecladoActual != 0 && estadoTecladoAnterior == 0) {
    manejarTeclado(estadoTecladoActual);  // Llama a la función para manejar el teclado
    Serial.print("Tecla presionada: ");
    Serial.println(estadoTecladoActual);
  }

  // Actualiza el estado anterior del teclado
  estadoTecladoAnterior = estadoTecladoActual;
}

void crucePorCeroISR() {
  if (encenderCalefactor && retardoEncendido > 0) {
    delayMicroseconds(retardoEncendido);  // Espera 
    SET_BIT(PORT_CALIFACTOR, PIN_CALIFACTOR); // Enciende el calefactor
    delay_ms(1);               // Pequeño pulso para asegurar el encendido
    CLEAR_BIT(PORT_CALIFACTOR, PIN_CALIFACTOR); // Apaga el calefactor después del pulso
  }
}
// Interrupción del temporizador para refrescar la pantalla y verificar el teclado
/*ISR(TIMER1_COMPA_vect) {
  manejarTeclado();             // Lee el teclado y actualiza el modo de operación
  lcd_actualizarDisplay();       // Actualiza la pantalla LCD con los valores actuales
}*/
// ---------------------------------------- MAIN CODE -------------------------------------------------
void setup() {
  Serial.begin(115200);
  while(!Serial){}
  // Configuramos los pines como salida para el LCD
  setDDR_RS(1);
  setDDR_RW(1);
  setDDR_E(1);
  setDDR_D4(1);
  setDDR_D5(1);
  setDDR_D6(1);
  setDDR_D7(1);
  setPORT_RW(0); // RW en 0 para escritura

  // Inicialización del LCD
  lcd_init();

  
  // Configuración del LCD
  lcd_clear();               // Limpiar pantalla
  lcd_blinkOn();            // Activar cursor parpadeante
  lcd_setCursor(0, 0);
  lcd_print("TERMOSTATO");  // Imprimir mensaje en la primera línea

  lcd_setCursor(0, 1);
  lcd_print("v1.0");         // Imprimir mensaje en la segunda línea
  
  // Configuración de pines para el calefactor
  pinMode(PIN_CALIFACTOR, OUTPUT); // Configura el pin del calefactor como salida
  pinMode(PIN_CRUCECERO, INPUT);
  pinMode(pinPotenciometro, INPUT);
  pinMode(pinTeclado, INPUT_PULLUP);  // Pin del teclado como entrada con pull-up

  pidTemperatura.SetMode(AUTOMATIC);
  pidTemperatura.SetOutputLimits(0, 255); // Ajusta el límite de salida (0-255)
  
  pidLuz.SetMode(AUTOMATIC);
  pidLuz.SetOutputLimits(0, 255); 


  // Configuración manual de la interrupción INT0
  EICRA |= (1 << ISC01);  // Flanco descendente: ISC01 = 1, ISC00 = 0
  EICRA &= ~(1 << ISC00);
  EIMSK |= (1 << INT0);   // Habilitar INT0

// Configuración de la interrupción de cruce por cero (usando registros directamente, no attachInterrupt)
  EIMSK |= (1 << PIN_CRUCECERO);



  
  // Configuración de la interrupción del temporizador para la actualización del display y teclado
  cli(); // Desactiva las interrupciones globales
  TCCR1A = 0; // Configuración del Timer1 en modo CTC
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 15624;  // Genera una interrupción cada 1 segundo (para refresco de display)


  // ----------------------------------------------------------------------------------------------------
  // Utilizamos macros para configurar el temporizador
  SET_BIT(TCCR1B, WGM12);             // Modo CTC
  SET_BIT(TCCR1B, CS12);               // Prescaler 1024
  SET_BIT(TCCR1B, CS10);               // CS10 = 1 para completar prescaler
  SET_BIT(TIMSK1, OCIE1A);             // Activa interrupción para el timer 1
  /*LEEME: Estas líneas de código son esenciales para configurar y utilizar el temporizador 1 en un microcontrolador AVR. 
  La configuración en modo CTC permite generar interrupciones a intervalos regulares, 
  lo que es útil para tareas como la actualización de un display o el manejo de sensores sin bloquear el flujo del programa en el bucle principal (loop). 
  La división del reloj mediante el prescaler permite que el temporizador funcione con intervalos más largos,
  y habilitar las interrupciones globales asegura que el sistema responda adecuadamente a eventos de hardware.*/
  // ----------------------------------------------------------------------------------------------------
  sei(); // Activa las interrupciones globales
}

void loop() {
  // Leer la temperatura del sensor o luz, dependiendo de la configuración
  #if USAR_SENSOR_TEMPERATURA == 1
    InputTemperatura = analogRead(pinSensorTemperatura);  // Lee el valor del sensor de temperatura
  #else
    InputLuz = analogRead(pinSensorLuz);  // Lee el valor del sensor de luz si no usamos temperatura
  #endif

  // Calcular el PID de temperatura (si estamos usando el sensor de temperatura)
  if (USAR_SENSOR_TEMPERATURA == 1) {
    pidTemperatura.Compute();  // Calcula la salida del PID para la temperatura
    setIntensidadCalefactor(OutputTemperatura);  // Ajusta la intensidad del calefactor según el PID
  } else {
    pidLuz.Compute();  // Calcula el PID para luz si se está usando un sensor de luz
  }
  // Un pequeño delay para evitar rebote del teclado o sobrecarga del microcontrolador
  delay(50);  // Este delay puede ser ajustado según tus necesidades (o eliminado si usas interrupciones para el teclado)
}
// ---------------------------------------- END MAIN CODE -------------------------------------------------
