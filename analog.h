#define sbi(reg, bit) ((reg) |= (1 << (bit)))
#define cbi(reg, bit) ((reg) &= ~(1 << (bit)))

#define getbit(reg, bit) ((reg) & (1<< (bit)))

void configureADC(uint8_t adcPin);
void configureTimer();
bool isConversionComplete();
int16_t getConversionResult();
ISR(TIMER1_COMPA_vect);
ISR(ADC_vect);
int16_t customAnalogRead(uint8_t adcPin);