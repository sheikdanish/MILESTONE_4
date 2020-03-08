#include "header_integrated.h"
// SUNROOF
void timer_init()
{
    TCCR0A=0x00; //Intialise timer counter register A
    TCNT0 = 0x00;
    TCCR0B |= ((1<<CS00)|((1<<CS01))); //clock and WGM02
}
void PWM_init()
{
    SET_BIT(DDRD,PD6); // PD6 (OC0A)output
    TCNT0 =0;
    TCCR0A |= ((1<<WGM00)|(1<<WGM01)|(1<<COM0A1)); // enabled the mode and effect
    TCCR0B |= ((1<<CS00)|((1<<CS01))); //clock and WGM02
}
void ADC_init()
{
    CLR_BIT(DDRC,PC5);   // this will be the input from Temperature and humidity sensor PC5 Is analog PIN
    ADMUX |= ((1<<REFS0));
    ADMUX |= ((1<<MUX0));
    ADMUX |= ((1<<MUX2)); // reference voltage and left adjust
    ADCSRA |= ((1<<ADPS1)|(1<<ADPS2)); //adc enable // division factor
}

int READ_ADC()
{
    START_ADC;
    while(ADCSRA & (1<<ADSC));
    //  Adc_val = ((ADCH<<8)&0xFF00)|(ADCL);
    // return Adc_val;
    return ADC;
}

// AC
extern void port_config()
{
    SET_BIT(DDRB,PB2);  // Face Output
    SET_BIT(DDRB,PB3);  // Legs Output
    SET_BIT(DDRB,PB4);  // Chest Output
    SET_BIT(DDRB,PB5);  // Windshield Output
    CLR_BIT(DDRB,PB0);
    SET_BIT(PORTB,PB0);
    CLR_BIT(DDRB,PB1);
    SET_BIT(PORTB,PB1);

    SET_BIT(DDRD,PD7); // Output - Hot/Cold Air Mode

    CLR_BIT(DDRD,PD2); //Interrupt Configuration
    CLR_BIT(DDRD,PD3);
    CLR_BIT(DDRD,PD4);
    CLR_BIT(DDRD,PD5);
    SET_BIT(DDRD,PD6);
    SET_BIT(PORTD,PD2);
    SET_BIT(PORTD,PD3);
    SET_BIT(PORTD,PD4);
    SET_BIT(PORTD,PD5);
    SET_BIT(EICRA,ISC10); // Any logical change INT1
    SET_BIT(EICRA,ISC01); // Any logical change INT0
    SET_BIT(EIMSK,INT0);
    SET_BIT(EIMSK,INT1);

    SET_BIT(TCCR0A,COM0A1);   //PWM Configuration
    SET_BIT(TCCR0A,WGM00);
    SET_BIT(TCCR0A,WGM01);
    OCR0A = 0;
    SET_BIT(TCCR0B,CS02);
    SET_BIT(TCCR0B,CS00);
    TCNT0=0;

    ADMUX&=0x05;            //ADC Configuration
    SET_BIT(ADMUX,REFS0);
    SET_BIT(ADCSRA,ADEN);
    sei();   //Global interrupt
}

// POWER WINDOWS
void stepper_config()
{
    SET_BIT(DDRC,PC4);
    SET_BIT(DDRC,PC3);
    SET_BIT(DDRC,PC2);
    SET_BIT(DDRC,PC1);

    SET_BIT(DDRB,PB4);
    SET_BIT(DDRB,PB3);
    SET_BIT(DDRB,PB2);
    SET_BIT(DDRB,PB1);
}
void switch_config()
{
    CLR_BIT(DDRD,PD1);
    CLR_BIT(DDRD,PD2);
    CLR_BIT(DDRD,PD3);
    CLR_BIT(DDRB,PB6);
    CLR_BIT(DDRB,PB7);
}
void window_up()
{
    PORTB = 0x12;
    _delay_ms(100);
    PORTB = 0x06;
    _delay_ms(100);
    PORTB = 0x0C;
    _delay_ms(100);
    PORTB = 0x18;
    _delay_ms(100);
}
void window_down()
{
    PORTB = 0x18;
    _delay_ms(100);
    PORTB = 0x0C;
    _delay_ms(100);
    PORTB = 0x06;
    _delay_ms(100);
    PORTB = 0x12;
    _delay_ms(100);
}

// POWER SIDE MIRRORS
void adjust_lxleft()
{
    SET_BIT(PORTB,PB3);
    SET_BIT(PORTB,PB2);
    CLR_BIT(PORTB,PB1);
    CLR_BIT(PORTB,PB0); // 12
    _delay_ms(100);

    CLR_BIT(PORTB,PB3);
    SET_BIT(PORTB,PB2);
    SET_BIT(PORTB,PB1);
    CLR_BIT(PORTB,PB0);  // 6
    _delay_ms(100);

    CLR_BIT(PORTB,PB3);
    CLR_BIT(PORTB,PB2);
    SET_BIT(PORTB,PB1);
    SET_BIT(PORTB,PB0); //3
    _delay_ms(100);

    SET_BIT(PORTB,PB3);
    CLR_BIT(PORTB,PB2);
    CLR_BIT(PORTB,PB1);
    SET_BIT(PORTB,PB0);  // 9
    _delay_ms(100);
}
void adjust_lxright()
{
    SET_BIT(PORTB,PB3);
    CLR_BIT(PORTB,PB2);
    CLR_BIT(PORTB,PB1);
    SET_BIT(PORTB,PB0);  // 9
    _delay_ms(100);

    CLR_BIT(PORTB,PB3);
    CLR_BIT(PORTB,PB2);
    SET_BIT(PORTB,PB1);
    SET_BIT(PORTB,PB0); //3
    _delay_ms(100);

    CLR_BIT(PORTB,PB3);
    SET_BIT(PORTB,PB2);
    SET_BIT(PORTB,PB1);
    CLR_BIT(PORTB,PB0);  // 6
    _delay_ms(100);

    SET_BIT(PORTB,PB3);
    SET_BIT(PORTB,PB2);
    CLR_BIT(PORTB,PB1);
    CLR_BIT(PORTB,PB0); // 12
    _delay_ms(100);
}
void adjust_lyup()
{
    SET_BIT(PORTC,PC3);
    SET_BIT(PORTC,PC2);
    CLR_BIT(PORTC,PC1);
    CLR_BIT(PORTC,PC0); // 12
    _delay_ms(100);

    CLR_BIT(PORTC,PC3);
    SET_BIT(PORTC,PC2);
    SET_BIT(PORTC,PC1);
    CLR_BIT(PORTC,PC0);  // 6
    _delay_ms(100);

    CLR_BIT(PORTC,PC3);
    CLR_BIT(PORTC,PC2);
    SET_BIT(PORTC,PC1);
    SET_BIT(PORTC,PC0); //3
    _delay_ms(100);

    SET_BIT(PORTC,PC3);
    CLR_BIT(PORTC,PC2);
    CLR_BIT(PORTC,PC1);
    SET_BIT(PORTC,PC0);  // 9
    _delay_ms(100);
}
void adjust_lydown()
{
    SET_BIT(PORTC,PC3);
    CLR_BIT(PORTC,PC2);
    CLR_BIT(PORTC,PC1);
    SET_BIT(PORTC,PC0);  // 9
    _delay_ms(100);

    CLR_BIT(PORTC,PC3);
    CLR_BIT(PORTC,PC2);
    SET_BIT(PORTC,PC1);
    SET_BIT(PORTC,PC0); //3
    _delay_ms(100);

    CLR_BIT(PORTC,PC3);
    SET_BIT(PORTC,PC2);
    SET_BIT(PORTC,PC1);
    CLR_BIT(PORTC,PC0);  // 6
    _delay_ms(100);

    SET_BIT(PORTC,PC3);
    SET_BIT(PORTC,PC2);
    CLR_BIT(PORTC,PC1);
    CLR_BIT(PORTC,PC0); // 12
    _delay_ms(100);
}

void init()
{
    CLR_BIT(DDRD,PD2); // Interrupt0
    CLR_BIT(DDRD,PD3); // Interrupt1
    SET_BIT(DDRC,PD1); // Fold Signal
    CLR_BIT(DDRD,PD0); // reverse_gear_input
    CLR_BIT(PORTD,PD0);
    CLR_BIT(DDRD,PD1); // right_mirror_select
    CLR_BIT(DDRD,PD4); // left_mirror_select

    SET_BIT(DDRB,PB3); // Angle x
    SET_BIT(DDRB,PB2); // Angle x
    SET_BIT(DDRB,PB1); // Angle x
    SET_BIT(DDRB,PB0); // Angle x

    SET_BIT(DDRC,PC3); // Angle y
    SET_BIT(DDRC,PC2); // Angle y
    SET_BIT(DDRC,PC1); // Angle y
    SET_BIT(DDRC,PC0); // Angle y

    CLR_BIT(DDRB,PB6); // UP
    CLR_BIT(DDRB,PB7); // DOWN
    CLR_BIT(DDRD,PD5); // LEFT
    CLR_BIT(DDRD,PD6); // RIGHT

    CLR_BIT(DDRB,PB5); // memory_button
    CLR_BIT(DDRB,PB4); // set_button

    //Configure interrupts
    EIMSK|=(1<<INT0);
    EIMSK|=(1<<INT1);
    EICRA=0x00;
    EICRA|=(1<<ISC01);
    EICRA|=(1<<ISC11);
    sei();
    CLR_BIT(PORTD,PD0); // reverse_gear_input
    CLR_BIT(PORTD,PD1); // right_mirror_select
    CLR_BIT(PORTD,PD4); // left_mirror_select
    CLR_BIT(PORTD,PD5); // memory_button
    CLR_BIT(PORTD,PD6); // set_button
    CLR_BIT(PORTC,PD1); // fold signal

}

