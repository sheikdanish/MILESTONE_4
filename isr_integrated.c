#include "header_integrated.h"
ISR(INT0_vect)
{
    cli();
    if(!(PIN_READ&(1<<PC4))&&(PIN_READ&(1<<PC6)))
    {
        flag.isr_Flag1=1;
    }
    if((PIN_READ&(1<<PC4))&&(PIN_READ&(1<<PC6)))
    {
        if(fold.lock_flag == 0)
            fold.lock_flag = 1;
        else
            fold.lock_flag = 0;
    }
    sei();
}

ISR(INT1_vect)
{
    cli();
    if(!(PIN_READ&(1<<PC4))&&(PIN_READ&(1<<PC6)))
    {
        flag.isr_Flag2=1;
    }
    if((PIN_READ&(1<<PC4))&&(PIN_READ&(1<<PC6)))
    {
        if(fold.button_flag == 0)
            fold.button_flag = 1;
        else
            fold.button_flag = 0;
    }
    sei();
}
ISR(TIMER0_OVF_vect)
{
    Sunroof.Timer_count++;          // Incrementing the timer count
    if(Sunroof.Timer_count>=1000)   // 4883 for 5 sec delay
    {
        Sunroof.Timer_count=0;
        Sunroof.index=1;
        //PORTD ^= (1<<7);
    }
}

