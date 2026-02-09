#include "sensors.h"


int getBrightness(){
  return customAnalogRead(SENSOR_LUZ);
}

float getTemperature(){//TODO comprobar si la comberison es correcta
  float T = customAnalogRead(SENSOR_TEMPERATURA);
  return T * (MAX_TEMPERATURE- MIN_TEMPERATURE)/1024;
}