#include<stdint.h>
#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#define SET_BIT(PORT,BIT) PORT|= (1<<BIT)
#define CLR_BIT(PORT,BIT) PORT&= ~(1<<BIT)
struct {
  
  volatile unsigned int FLAG_ISR1:1;
  volatile unsigned int FLAG_ISR2:1;
}FLAG_BIT;
volatile uint16_t us_value=0;
volatile uint16_t *pus_value;
typedef void (*funpointer)(void);
funpointer array_fp[2]={extint1};
void output(int );
int main()
  
{  pus_value=&us_value;
    SET_BIT(DDRD,PD7); //RED LED       
    SET_BIT(DDRD,PD6); //BUZZER
    SET_BIT(DDRD,PD5);//GREEN LED
    CLR_BIT(PORTD,PD7);        
    CLR_BIT(PORTD,PD6); 
    CLR_BIT(PORTD,PD5);
    Serial.begin(9600);
  	array_fp[0]();
    
  while(1)
  {   
  if(FLAG_BIT.FLAG_ISR1==1)
  {
    SET_BIT(PORTD,PD5);
   cli();
    
  
    array_fp[0]();
   *pus_value=ultra_sonicread();
   output(*pus_value);
    

  }
   else
   {
     CLR_BIT(PORTD,PD7);
      CLR_BIT(PORTD,PD6);
      CLR_BIT(PORTD,PD5);
   }
   sei();
  }
  
}
void extint1()
{
  EICRA|=(1<<ISC10);
    EICRA&=~(1<<ISC11);
    EIMSK|=(1<<INT1);
  sei();
}
ISR(INT1_vect)
            {
   			cli();
            FLAG_BIT.FLAG_ISR1=!FLAG_BIT.FLAG_ISR1;
            sei(); 
            }
void extint0()
{
  EICRA|=(1<<ISC00);
    EICRA&=~(1<<ISC01);
    EIMSK|=(1<<INT0);
  sei();
}
ISR(INT0_vect)
            {
   			cli();
            FLAG_BIT.FLAG_ISR2=!FLAG_BIT.FLAG_ISR2;
            sei(); 
            }
int ultra_sonicread()
{
  
  SET_BIT(DDRD,PD4);
  SET_BIT(PORTD,PD4);
  _delay_ms(5);
  CLR_BIT(PORTD,PD4);
  CLR_BIT(DDRD,PD4);
  int time_duration = pulseIn(PD4,HIGH);
  float distance=0.343*time_duration*0.5*0.1;
  return(distance);
}
void output(int sensor)
{
 if(sensor>2 && sensor<15)
 {
  SET_BIT(PORTD,PD6);
  SET_BIT(PORTD,PD7);
 }
  else
  {
    CLR_BIT(PORTD,PD6);
    CLR_BIT(PORTD,PD7);
  }
}
