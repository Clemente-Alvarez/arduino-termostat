#include <PID_v1.h>

#include "UI.h"
#include "sensors.h"
#include "analog.h"
#include "lightbulb.h"
uint32_t t = 0;

extern double Setpoint = DEFAULT_TEMPERATURE;
extern double Input = 0;
extern double Output = 0;


//Inicialización para pantalla
extern unsigned int vertical = 0;
extern unsigned char config = 0;

// Configuración del PIDs
PID tPID(&Input, &Output, &Setpoint, 2.0, 0.25, 0.25, DIRECT);
PID lPID(&Input, &Output, &Setpoint, 1.0, 0.1, 0.1, DIRECT);

void setup() {
  Serial.begin(115200);
  while(!Serial){}

  lightbulbSetup();           // Configurar el sistema
  lcd_init();//iniciar el lcd
  lcd_clear();//Limpiar pantalla
  lcd_setCursor(0, 0);//poner cursor en 0, 0
  configureADC(PIN_TECLADO);
  configureTimer();

  // Configura los PID
  tPID.SetMode(AUTOMATIC);
  tPID.SetOutputLimits(0, MAX_TEMPERATURE);
  tPID.SetSampleTime(200);
  lPID.SetMode(AUTOMATIC);
  lPID.SetOutputLimits(0, MAX_LIGHT);
  lPID.SetSampleTime(100);
}

void loop() {
  switch(modo_operacion){
    case MODO_TERMOSTATO:
    if(config == CONFIG_POTENCIOMETRO) Setpoint = readPotenciometer()/MAX_TEMPERATURE + DEFAULT_TEMPERATURE;
    Input = getTemperature();
    tPID.Compute();
    setLightPercent(Output/MAX_TEMPERATURE);
    break;
    case MODO_INTENSIDAD_LUZ:
    if(config == CONFIG_POTENCIOMETRO) Setpoint = readPotenciometer();
    Input = getBrightness();
    lPID.Compute();
    setLightPercent(Output/MAX_LIGHT);
    break;
    case MODO_MANUAL:
    if(config == CONFIG_POTENCIOMETRO) vertical = (readPotenciometer()*100)/1024;
    setLightPercent((float)(vertical%101)/100.0);
    break;
    case MODO_ENCENDIDO:
    setLightPercent(1);
    break;
    case MODO_APAGADO:
    setLightPercent(0);
    Input = getTemperature();
    break;
  }
  manejarTeclado();
  if(t + 250 < millis()){
    lcd_actualizarDisplay();
    t = millis();
  }

  Serial.print(Input);
  Serial.print(",");
  Serial.print(Output);
  Serial.print(",");
  Serial.println(Setpoint);
}


