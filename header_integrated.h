#include <avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>
#define SET_BIT(PORT,PIN)  PORT |= (1<<PIN)
#define CLR_BIT(PORT,PIN)  PORT &= ~(1<<PIN)
uint8_t PIN_READ;
// SUNROOF
#define START_TIMER TIMSK0|=(1<<TOIE0)
#define STOP_TIMER  TIMSK0 &= ~(1<<TOIE0)
#define START_ADC  ADCSRA |= (1<<ADSC)
#define ENABLE_ADC ADCSRA |= (1<<ADEN)
#define INTERRUPT_INIT  SREG |= (1<<7) // GLOBAL interrupt enabled
#define READ_BIT(PORT,PIN) (PORT & (1<<PIN))
struct
{
    uint16_t Timer_count;
    volatile uint8_t index;
} Sunroof;
void PWM_init();
void timer_init();
void ADC_init();
int READ_ADC();

// AC
struct
{
    volatile unsigned int isr_Flag1:1;
        volatile unsigned int isr_Flag2:1;
    } flag;

    void port_config();

// POWER WINDOWS
#include<stdint.h>
    struct window
{
    volatile unsigned int lock : 1;
        volatile unsigned int d_up : 1;
        volatile unsigned int p_down : 1;
        volatile unsigned int p_up : 1;
        volatile unsigned int d_down : 1;
    } wind;
    void stepper_config();
    void switch_config();
    void window_up();
    void window_down();
// POWER SIDE MIRRORS
    struct
{
    volatile unsigned lock_flag:1;
        volatile unsigned button_flag:1;
    } fold;
    void init();
    void adjust_lxleft();
    void adjust_lxright();
    void adjust_lydown();
    void adjust_lyup();
