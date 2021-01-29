/*
 * CYTRON_ROBOCON.c
 *
 * Created: 27-11-2020 02:23:08
 *  Author: parth thakkar 
*/ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


#define PWM_REGISTER_1 DDRE   // for pin 5 // OC3A
#define PWM_REGISTER_2 DDRH   // for pin  7 // OC4B
#define PWM_REGISTER_3 DDRB   // for pin 11 // OC1A
#define PWM_REGISTER_4 DDRB   // for pin 13 // OC0A

#define PWM_PIN_1 PE3   // pin 5
#define PWM_PIN_2 PH4   // pin 7
#define PWM_PIN_3 PB5   // pin 11
#define PWM_PIN_4 PB7  // pin 13



#define direction_REGISTER_1 DDRG   // for pin 4
#define direction_REGISTER_2 DDRH   // for pin 6
#define direction_REGISTER_3 DDRH   // for pin 8
#define direction_REGISTER_4 DDRB   // for pin 12

#define direction_PORT_1 PORTG   // for pin 4
#define direction_PORT_2 PORTH   // for pin 6
#define direction_PORT_3 PORTH   // for pin 8
#define direction_PORT_4 PORTB   // for pin 12

#define direction_pin_1 PG5   // pin 4
#define direction_pin_2 PH3   // pin 6
#define direction_pin_3 PH5   // pin 8
#define direction_pin_4 PB6   // pin 12

/***********************/

int  duty_cycle;
int rotation;

/************************/
void PWM_1_init(float duty_cycle);
void PWM_2_init(float duty_cycle);
void PWM_3_init(float duty_cycle);
void PWM_4_init(float duty_cycle);
/************************/
void DIRECTION_1_init(int rotation);
void DIRECTION_2_init(int rotation);
void DIRECTION_3_init(int rotation);
void DIRECTION_4_init(int rotation);
/************************/
void cytron(int cytron_no, int duty_cycle, int rotation);
/************************/
void convert_rotation(int rotation); 


void cytron(int cytron_no, int duty_cycle, int rotation)
{
  switch(cytron_no)
  {
    case 1:
      PWM_1_init(duty_cycle);
      DIRECTION_1_init(rotation);
      break;
    case 2:
      PWM_2_init(duty_cycle);
      DIRECTION_2_init(rotation);
      break;
    case 3:
      PWM_3_init(duty_cycle);
      DIRECTION_3_init(rotation);
      break;
    case 4:
      PWM_4_init(duty_cycle);
      DIRECTION_4_init(rotation);
      break;
    default:
      PWM_1_init(0);
      PWM_2_init(0);
      PWM_3_init(0);
      PWM_4_init(0);

      DIRECTION_1_init(0);
      DIRECTION_2_init(0);
      DIRECTION_3_init(0);
      DIRECTION_4_init(0);
      
  }
}
/*******************************/
//Mode | WGM2 | WGM1 | WGM0 | Timer/Counter Mode of Operation | TOP  
//0       0       0     0             Normal                    0xFF                 
//1       0       0     1       PWM, Phase Correct              0xFF 
//2       0       1     0             CTC                       OCRA 
//3       0       1     1           Fast PWM                    0xFF
//5       1       0     1       PWM, Phase Correct              OCRA
//7       1       1     1           Fast PWM                    OCRA 

/*******************************/

void PWM_1_init(float duty_cycle)
{
 uint8_t sreg = SREG;
    cli();

    // Stop timer before configuring
    TCCR3B = 0;

    // TCCR3A – Timer/Counter1 Control Register A
    TCCR3A =
        0
        | (1 << COM3A1) | (0 << COM3A0)  // Clear OC3A on Compare Match, set OC1A at BOTTOM (non-inverting mode)
        | (1 << WGM31) | (0 << WGM30)    // Fast PWM mode 14 (TOP = ICR1)
        ;

    // TCCR1B – Timer/Counter1 Control Register B
    TCCR3B =
        0
        | (1 << WGM33) | (1 << WGM32)    // Fast PWM mode 14 (TOP = ICR1), part 2/2
        ;


    // Set TOP value
    ICR3 = 0xFFFF;
    
    //   "The OCR1X Register is double buffered when using any of the
    //   twelve Pulse Width Modulation (PWM) modes. For the Normal
    //   and Clear Timer on Compare (CTC) modes of operation, the
    //   double buffering is disabled."
    // If initializing OCR1A before configuring TCCR1A and TCCR1B to
    // a PWM mode the value is written to the non-buffered OCR1A
    // register and the buffered OCR1A register contains some "random",
    // unused garbage value. When later changing to PWM the buffered
    // register will be enabled, and its existing garbage value will
    // be used.

    OCR3A = ((duty_cycle/100)*ICR3);

    // 14.4.3 DDRB – The Port B Data Direction Register
    PWM_REGISTER_1 =
        0
        | (1 << PWM_PIN_1) // (OC3A) as output - pin 9
        ;

    // Start the timer with no prescaler
    TCCR3B |= (0 << CS32) | (0 << CS31) | (1 << CS30);

    SREG = sreg;
}


/*******************************/


void PWM_2_init(float duty_cycle)
{
 uint8_t sreg = SREG;
    cli();
    TCCR4B = 0;
    TCCR4A =
        0
        | (1 << COM4B1) | (0 << COM4B0)  // Clear OC3A on Compare Match, set OC1A at BOTTOM (non-inverting mode)
        | (1 << WGM41) | (0 << WGM40)    // Fast PWM mode 14 (TOP = ICR1)
        ;
    TCCR4B =
        0
        | (1 << WGM43) | (1 << WGM42)    // Fast PWM mode 14 (TOP = ICR1)
        ;

    ICR4 = 0xFFFF;

    OCR4B = ((duty_cycle/100)*ICR4);
    
    PWM_REGISTER_2 =
        0
        | (1 << PWM_PIN_2) // (OC4B) as output - pin 9
        ;

    TCCR4B |= (0 << CS42) | (0 << CS41) | (1 << CS40);

    SREG = sreg;
}



/********************************/


void PWM_3_init(float duty_cycle)
{
 uint8_t sreg = SREG;
    cli();
    
    PWM_REGISTER_3 =
        0
        | (1 << PWM_PIN_3) // (OC1A) as output - pin 9
        ;
        
    TCCR1B = 0;
    TCCR1A =
        0
        | (1 << COM1A1) | (0 << COM1A0)  // Clear OC1A on Compare Match, set OC1A at BOTTOM (non-inverting mode)
        | (1 << WGM11) | (0 << WGM10)    // Fast PWM mode 14 (TOP = ICR1)
        ;
    TCCR1B =
        0
        | (1 << WGM13) | (1 << WGM12)    // Fast PWM mode 14 (TOP = ICR1)
        ;

    ICR1 = 0xFFFF;

    OCR1A = ((duty_cycle/100)*ICR1);
    

    TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10);

    SREG = sreg;
}

/********************************/


void PWM_4_init(float duty_cycle)
{
  uint8_t sreg = SREG;
    cli();
    TCCR0B = 0;
    TCCR0A =
        0
        | (1 << COM0A1) | (0 << COM0A0)  // Clear OC0A on Compare Match, set OC1A at BOTTOM (non-inverting mode)
        | (1 << WGM01) | (1 << WGM00)    // Fast PWM mode 3 
        ;
    TCCR0B =
        0
        | (0 << WGM02)    // Fast PWM mode 3
        ;

    OCR0A = ((duty_cycle/100)*255);
    
    PWM_REGISTER_4 =
        0
        | (1 << PWM_PIN_4) // (OC0A) as output - pin 13
        ;

    TCCR0B |= (0 << CS02) | (0 << CS01) | (1 << CS00);

    SREG = sreg;
}


/**************************************/

void DIRECTION_1_init(int rotation)
{
  direction_REGISTER_1 |= (1 << direction_pin_1);
  convert_rotation(rotation);
  direction_PORT_1 |= (rotation << direction_pin_1);
}

void DIRECTION_2_init(int rotation)
{
  direction_REGISTER_2 |= (1 << direction_pin_2);
  convert_rotation(rotation);
  direction_PORT_2 |= (rotation << direction_pin_2);
}

void DIRECTION_3_init(int rotation)
{
  direction_REGISTER_3 |= (1 << direction_pin_3);
  convert_rotation(rotation);
  direction_PORT_3 |= (rotation << direction_pin_3);
}

void DIRECTION_4_init(int rotation)
{
  direction_REGISTER_4 |= (1 << direction_pin_4);
  convert_rotation(rotation);
  direction_PORT_4 |= (rotation << direction_pin_4);
}


/***********************************************/

void convert_rotation(int rotation)
{
  if (rotation > 1) rotation = 1;
  if (rotation < 1 && rotation >0) rotation = 0;
  if (rotation < 0) rotation = 0;
}




// DDRB|=(1<<PINB5)|(1<<PINB6)|(1<<PINB7);
//  DDRH|=(1<<PINH3)|(1<<PINH4)|(1<<PINH5);
//  
//  //PWM PIN NUMBER 11-motor3-(PH5,PB5)
//  TCCR1A=0x81;
//  TCCR1B=0x01;
//  OCR1A=255; //255
//  
//  //PWM PIN NUMBER 7-motor2-(PH3,PH4)
//  TCCR4A=0x21;
//  TCCR4B=0x01;
//  OCR4B=255; //255
//  
//  //PWM PIN NUMBER 13-motor1-(PB6,PB7)
//  TCCR0A=0x81;
//  TCCR0B=0x01;
//  OCR0A=255; //255
