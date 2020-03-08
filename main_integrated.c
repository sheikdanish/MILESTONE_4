// Milestone 3 - Embedded C implementation of integrated systems of SUNROOF, AC, POWER WINDOWS & POWER SIDE MIRRORS
// Author - Shashikala, Divyang, Ravi & Danish
// Version - 1.0
#include "header_integrated.h"
int main(void)
{
    CLR_BIT(DDRC,PC4);
    CLR_BIT(DDRC,PC6);
    while(1)
    {
        PIN_READ=PINC;
        if(!(PIN_READ&(1<<PC4))&&(!(PIN_READ&(1<<PC6))))    //SUNROOF
        {
            PIN_READ=PINC;
            uint16_t ADC_FINAL=0;
            CLR_BIT(DDRD,PD2);   //For Opening Of Switch. PD2 as input.
            SET_BIT(PORTD,PD2);  //PULL up configuration.
            CLR_BIT(DDRD,PD3);   //For Closing Of Switch. PD3 as Input
            SET_BIT(PORTD,PD3);  //PULL up configuration.
            CLR_BIT(DDRD,PD4);   //For Checking the status Of AC switch. PD4 as Input
            SET_BIT(PORTD,PD4);  //PULL up configuration
            CLR_BIT(DDRD,PD5);   // For Checking the status of Ignition. PD5 as Input.
            SET_BIT(PORTD,PD5);  //PULL up configuration
            CLR_BIT(DDRD,PD0);   // For Checking the status Of Rain. Making PD0 as Input
            SET_BIT(PORTD,PD0);  //PULL up configuration
            ADC_init();
            ENABLE_ADC;
            timer_init();
            PWM_init();
            TIMSK0 |= (1<<TOIE0);
            TIFR0 |= (1<<TOV0);
            //STOP_TIMER;
            INTERRUPT_INIT;
            while(!(PIN_READ&(1<<PC4))&&(!(PIN_READ&(1<<PC6))))         // Super Loop
            {
                PIN_READ=PINC;
                if(!(READ_BIT(PIND,PD5))) //Checking the engine ignition
                {
                    if(!(READ_BIT(PIND,PD2))) //For Opening //Test-Case-1
                    {
                        if(!(READ_BIT(PIND,PD0)))  //For Rain. If Rain Started.. //Test-Case-2
                        {
                            OCR0A = 64;   //If Rain started, the sunroof will close. 25% duty cycle.
                        }
                        else if((READ_BIT(PIND,PD0)))  //If Rain Stopped
                        {
                            if (!(READ_BIT(PIND,PD4))) //For AC. If AC is ON. //Test-Case-3
                            {


                                OCR0A = 64;  //If AC is On, the sunroof will close. For Closing 25% duty-cycle.
                            }
                            else
                            {
                                OCR0A = 192;
                            }
                        }
                        else
                        {
                            OCR0A = 192;  //If none AC is On or it is not raining, If I press the switch to open, it will open. For Opening 50% duty-cycle.
                        }
                    }
                    else if(!(READ_BIT(PIND,PD3))) //For Closing. Test-Case-4
                    {
                        ADC_FINAL = READ_ADC(); //For checking any object detect during Sunroof Panel Close. Test-case-5
                        if(Sunroof.index==1)
                        {
                            //Read ADC Value
                            if(ADC_FINAL<=750) //If it is in the range of 0-750 it mean no object and it will continue Closing or else it will open.
                            {
                                OCR0A = 64;
                            }
                            else
                            {
                                OCR0A = 0; //Stop. If it detect some object, the sunroof will stop functioning there.
                            }
                            Sunroof.index = 0;
                        }
                    }
                }
                PIN_READ=PINC;
            }
            PIN_READ=PINC;
        }
        else if(!(PIN_READ&(1<<PC4))&&((PIN_READ&(1<<PC6))))    // AC
        {
            PIN_READ=PINC;
            unsigned int temp_pwm=128;
            unsigned int adc_value=0;
            port_config();  //Configuration of all ports
            unsigned int flag_hot=0;
            while(!(PIN_READ&(1<<PC4))&&((PIN_READ&(1<<PC6))))
            {
                PIN_READ=PINC;
                if((PIND & (1<<PD4))==0)
                {
                    if (flag_hot)
                    {
                        CLR_BIT(PORTD, PD7); // HOT Air Mode
                    }
                    else
                    {
                        SET_BIT(PORTD, PD7); // COLD Air Mode
                    }

                    if((PINB&(1<<PB0)) && (!(PINB&(1<<PB1))))
                    {
                        CLR_BIT(PORTB,PB2);
                        CLR_BIT(PORTB,PB4);
                        CLR_BIT(PORTB,PB5);
                        SET_BIT(PORTB,PB3);  // Leg Mode
                    }
                    if((!(PINB&(1<<PB0))) && ((PINB&(1<<PB1))))
                    {
                        CLR_BIT(PORTB,PB2);  // Chest Mode
                        CLR_BIT(PORTB,PB3);
                        CLR_BIT(PORTB,PB5);
                        SET_BIT(PORTB,PB4);
                    }
                    if((!(PINB&(1<<PB0))) && ((!(PINB&(1<<PB1)))))
                    {
                        CLR_BIT(PORTB,PB2);
                        CLR_BIT(PORTB,PB4);
                        CLR_BIT(PORTB,PB3);
                        SET_BIT(PORTB,PB5);  // Windshield Mode
                    }
                    if (((PINB&(1<<PB0))) && ((PINB&(1<<PB1))))
                    {
                        CLR_BIT(PORTB,PB3);
                        CLR_BIT(PORTB,PB4);
                        CLR_BIT(PORTB,PB5);
                        SET_BIT(PORTB,PB2);  // Face Mode
                    }
                    OCR0A=temp_pwm;
                    if(flag.isr_Flag1==1)
                    {
                        OCR0A=OCR0A+64;
                        temp_pwm=OCR0A;
                        _delay_ms(200);  //Delay
                        flag.isr_Flag1=0;
                    }
                    if(flag.isr_Flag2==1)
                    {
                        OCR0A=OCR0A-64;
                        temp_pwm=OCR0A;
                        _delay_ms(200);
                        flag.isr_Flag2=0;
                    }
                }
                else
                {
                    if((PIND & (1<<PD5)))
                    {
                        OCR0A=255;
                    }
                    CLR_BIT(PORTD,PD7);  //Default - Cool Air
                    CLR_BIT(PORTB,PB2);  //Default - Face
                }
                if(((PIND & (1<<PD5))==0))
                {
                    SET_BIT(ADCSRA,ADSC);
                    while(ADCSRA & (1<<ADSC));
                    adc_value=ADC;
                    if(adc_value>750)
                    {
                        flag_hot =1;
                    }
                    else
                    {
                        flag_hot =0;
                    }
                }
                PIN_READ=PINC;
            }
            PIN_READ=PINC;
        }
        else if((PIN_READ&(1<<PC4))&&(!(PIN_READ&(1<<PC6)))) // POWER WINDOW
        {
            PIN_READ=PINC;
            stepper_config();
            switch_config();

            uint8_t count = 0;

            wind.lock = 0;
            wind.p_up = 0;
            wind.p_down = 0;
            wind.d_up = 0;
            wind.d_down = 0;

            while((PIN_READ&(1<<PC4))&&(!(PIN_READ&(1<<PC6))))
            {
                if(PIND & (1<<PD3))
                    wind.lock = 1;
                else
                    wind.lock= 0;

                if(PINB & (1<<PB6))
                    wind.p_up = 1;
                else
                    wind.p_up = 0;

                if(PINB & (1<<PB7))
                    wind.p_down = 1;
                else
                    wind.p_down = 0;

                if(PIND & (1<<PD1))
                    wind.d_up = 1;
                else
                    wind.d_up = 0;

                if(PIND & (1<<PD2))
                    wind.d_down = 1;
                else
                    wind.d_down = 0;

                if(wind.d_up == 1 && count < 5)
                {
                    window_up();
                    count++;
                }
                else if(wind.d_down == 1 && count > 0)
                {
                    window_down();
                    count--;
                }


                if(wind.lock == 0)
                {
                    if(wind.p_up == 1 && count < 5)
                    {
                        window_up();
                        count++;
                    }
                    else if(wind.p_down == 1 && count > 0)

                    {
                        window_down();
                        count--;
                    }
                }
            }
            PIN_READ=PINC;
        }
        else if((PIN_READ&(1<<PC4))&&((PIN_READ&(1<<PC6)))) // POWER SIDE MIRROR
        {
            PIN_READ=PINC;
            int lxcount=0;
            int lycount=0;
            int tilt_flag=0;
            int lycount_temp=0;
            int i;
            int lxcount_mem=0;
            int lycount_mem=0;
            uint8_t PIND_READ;
            uint8_t PINB_READ;
            fold.lock_flag=0;
            fold.button_flag=0;
            init();
            while((PIN_READ&(1<<PC4))&&((PIN_READ&(1<<PC6))))
            {
                PIN_READ=PINC;
                PIND_READ = PIND;
                PINB_READ = PINB;
                if(fold.lock_flag == 1) // unlock
                {
                    if(fold.button_flag == 1) // unfold
                    {
                        SET_BIT(PORTD,PD1); // unfold mirrors

                        if((PIND_READ&(1<<PD0)))  // reverse gear
                        {
                            if(!tilt_flag)
                            {
                                lycount_temp=lycount;
                                for(i=lycount+5; i>0; i--)
                                {
                                    adjust_lydown();
                                    lycount-=1;
                                }
                                tilt_flag = 1;
                            }
                        }
                        else
                        {
                            if(tilt_flag)
                            {
                                while(lycount!=lycount_temp)
                                {
                                    if(lycount<lycount_temp)
                                    {
                                        lycount+=1;
                                        adjust_lyup();
                                    }
                                }
                                tilt_flag = 0;
                            }
                            if((PIND_READ&(1<<PD4))) // left mirror select
                            {
                                if((PINB_READ&(1<<PB6)) && (lycount < 5))
                                {
                                    lycount+=1;
                                    adjust_lyup();
                                }
                                if((PINB_READ&(1<<PB7)) && (lycount>-5))
                                {
                                    lycount-=1;
                                    adjust_lydown();
                                }
                                if((PIND_READ&(1<<PD5)) && (lxcount < 5))
                                {
                                    lxcount+=1;
                                    adjust_lxleft();
                                }
                                if((PIND_READ&(1<<PD7)) && (lxcount>-5))
                                {
                                    lxcount-=1;
                                    adjust_lxright();
                                }
                            }
                            if((PIND_READ&(1<<PD1))) // right mirror select
                            {
                                // same as left mirror select
                                // unable to implement due to shortage of pins
                            }
                            if((PINB_READ&(1<<PB5))) // set mirror position
                            {
                                while(lycount!=lycount_mem)
                                {
                                    if(lycount<lycount_mem)
                                    {
                                        lycount+=1;
                                        adjust_lyup();
                                    }
                                    if(lycount>lycount_mem)
                                    {
                                        lycount-=1;
                                        adjust_lydown();
                                    }
                                }
                                while(lxcount!=lxcount_mem)
                                {
                                    if(lxcount<lxcount_mem)
                                    {
                                        lycount+=1;
                                        adjust_lxleft();
                                    }
                                    if(lxcount>lxcount_mem)
                                    {
                                        lxcount-=1;
                                        adjust_lxright();
                                    }
                                }
                            }
                            if((PINB_READ&(1<<PB4))) // save mirror position
                            {
                                lxcount_mem=lxcount;
                                lycount_mem=lycount;
                            }
                        }
                    }
                    else // fold
                    {
                        CLR_BIT(PORTD,PD1); // fold mirrors
                    }
                }
                else // lock
                {
                    CLR_BIT(PORTD,PD1); // fold mirrors
                }
            }
            PIN_READ=PINC;
        }
    }
}

