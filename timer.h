
#define SET_BIT(reg, bit) ((reg) |= (1 << (bit)))
#define CLEAR_BIT(reg, bit) ((reg) &= ~(1 << (bit)))

extern volatile uint32_t time;

ISR(TIMER1_CAPT_vect);

static void setup_timer1();

static uint32_t clock_ms();

static void delay_ms(uint32_t ms);

static void delay_until_ms(uint32_t end_ms);