#include "analog.h"

int16_t convertions[6];
bool isConverting[6] = {false, false, false, false, false, false};
bool goodConvertion[6] = {true, true, true, true, true, true};
bool canConvert = true;


// Variables globales
volatile bool is_converted = false;  // Indica si hay una conversión lista
volatile int16_t conversion = 0;    // Almacena el resultado de la conversión
volatile bool working = false;      // Indica si el ADC está trabajando

// Configuración inicial del ADC
void configureADC(uint8_t adcPin) {
    sbi(ADCSRA, ADEN); // Habilitar el ADC
    sbi(ADCSRA, ADPS2); // Prescaler de /128 al activar ADPS2..ADPS0
    sbi(ADCSRA, ADPS1);
    sbi(ADCSRA, ADPS0);
    sbi(ADCSRA, ADIE); // Habilitar interrupciones del ADC
    // Configurar el pin ADC
    ADMUX = (ADMUX & 0xE0) | (adcPin & 0b011111); // Configurar MUX4..MUX0
    ADCSRB = (ADCSRB & 0xDF) | ((adcPin & 0b100000)); // Configura MUX5 | Primero aplicamos mascara (bit 5 a '0') y luego aplicamos el valor de adcPin
    // Seleccionar referencia de voltaje como Vcc
    cbi(ADMUX, REFS1);
    sbi(ADMUX, REFS0);
}

// Configuración del temporizador
void configureTimer() {
    cli(); // Deshabilitar interrupciones globales
    sbi(TCCR1B, CS12); // Prescaler de /1024
    cbi(TCCR1B, CS11);
    sbi(TCCR1B, CS10);
    cbi(TCCR1B, WGM13); // Modo CTC
    sbi(TCCR1B, WGM12);
    cbi(TCCR1A, WGM11);
    cbi(TCCR1A, WGM10);
    OCR1A = 156; // Valor TOP para 10 milisegundos
    sbi(TIMSK1, OCIE1A); // Habilitar interrupciones en el canal A
    sei(); // Habilitar interrupciones globales
}

// Leer si hay una conversión lista
bool isConversionComplete() {
    cli();
    bool value = is_converted;
    sei();
    return value;
}

// Obtener el resultado de la conversión
int16_t getConversionResult() {
    cli();
    int16_t value = conversion;
    is_converted = false; // Reiniciar la bandera
    sei();
    return value;
}


// Interrupción del Timer1
ISR(TIMER1_COMPA_vect) {
    if (!bit_is_set(ADCSRA, ADSC)) {
        sbi(ADCSRA, ADSC);
    }
}

// Interrupción del ADC
ISR(ADC_vect) {
    conversion = ADC;       // Guardar el resultado
    is_converted = true;    // Señalar que la conversión está lista
}


int16_t getConvertion(uint8_t adcPin){
  switch(adcPin){
    case 7: return convertions[5];
    case 6: return convertions[4];
    case 5: return convertions[3];
    case 4: return convertions[2];
    case 1: return convertions[1];
    case 0: return convertions[0];
  }
}

void setConvertion(uint8_t adcPin, int16_t value){
  switch(adcPin){
    case 7: convertions[5] = value; break; 
    case 6: convertions[4] = value; break;
    case 5: convertions[3] = value; break;
    case 4: convertions[2] = value; break;
    case 1: convertions[1] = value; break;
    case 0: convertions[0] = value; break;
  }
}

bool isConvertionReady(uint8_t adcPin){
  switch(adcPin){
    case 7: return isConverting[5];
    case 6: return isConverting[4];
    case 5: return isConverting[3];
    case 4: return isConverting[2];
    case 1: return isConverting[1];
    case 0: return isConverting[0];
  }
}


void putConverting(uint8_t adcPin){
  switch(adcPin){
    case 7: isConverting[5] = !isConverting[5]; break;
    case 6: isConverting[4] = !isConverting[4]; break;
    case 5: isConverting[3] = !isConverting[3]; break;
    case 4: isConverting[2] = !isConverting[2]; break;
    case 1: isConverting[1] = !isConverting[1]; break;
    case 0: isConverting[0] = !isConverting[0]; break;
  }
}

bool isConvertionGood(uint8_t adcPin){
  switch(adcPin){
    case 7: goodConvertion[5] = !goodConvertion[5];
    return goodConvertion[5];
    case 6: goodConvertion[4] = !goodConvertion[4];
    return goodConvertion[4];
    case 5: goodConvertion[3] = !goodConvertion[3];
    return goodConvertion[3];
    case 4: goodConvertion[2] = !goodConvertion[2];
    return goodConvertion[2];
    case 1: goodConvertion[1] = !goodConvertion[1];
    return goodConvertion[1];
    case 0: goodConvertion[0] = !goodConvertion[0];
    return goodConvertion[0];
  }
}
// Función para iniciar la lectura del ADC
int16_t customAnalogRead(uint8_t adcPin) {
  if(canConvert && !isConvertionReady(adcPin)){
    configureADC(adcPin); // Configurar el ADC para el pin deseado
    canConvert = false;
    putConverting(adcPin);
  }
  if(isConversionComplete() && isConvertionReady(adcPin)){
      putConverting(adcPin);
      canConvert = true;
    if(isConvertionGood(adcPin))// desecha la primera conversion
      setConvertion(adcPin, getConversionResult());
    else return customAnalogRead(adcPin);
  } 
  // La conversión es iniciada automáticamente por el temporizador
  return getConvertion(adcPin);
}


