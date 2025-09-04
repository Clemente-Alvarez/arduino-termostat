#include "timer.h"

volatile uint32_t time;

ISR(TIMER1_CAPT_vect) {
  time++;
}

static void setup_timer1() {
  cli();
  TCCR1A = 0x00;
  TCCR1B = 0x00;
  TCCR1C = 0x00;
  TIMSK1 = 0x00;

  // 1. Setup mode: CTC with ICR1 as the top value
  SET_BIT(TCCR1B, WGM13);
  SET_BIT(TCCR1B, WGM12);

  // 2. Setup IRQs
  SET_BIT(TIMSK1, ICIE1);

  // 3. Setup timer frequency:
  //tope
  ICR1 = 15999;
  //prescaler
  SET_BIT(TCCR1B, CS10);

  sei();
}

static uint32_t clock_ms() {
  cli();
  uint32_t current_time = time;
  sei();
  return current_time;
}

static void delay_ms(uint32_t ms) {
  uint32_t wait = clock_ms() + ms;
  while(clock_ms() < wait){}
}

static void delay_until_ms(uint32_t end_ms) {
  while(end_ms > clock_ms()){}
}