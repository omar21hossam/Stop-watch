/*

 *      Author: omar2
 */
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


unsigned char sec1=3;
unsigned char sec2=2;
unsigned char min1=9;
unsigned char min2=5;
unsigned char hour1=3;
unsigned char hour2=2;


void Timer1_CTC(void){
	SREG  &= ~(1<<7);
	TCCR1A = (1<<FOC1A);
	TCCR1B = (1<<WGM12) | (1<<CS12) |(1<<CS10);
	OCR1A =1000;
	TIMSK = (1<<OCIE1A);
	SREG  |= (1<<7); 					// enable I bit
}

void INT0_Init(void){
	SREG  &= ~(1<<7);
	DDRD  &= ~(1<<PD2);               // Configure INT0 as input pin
	PORTD |= (1<<PD2);					// enable the pull up resistor
	GICR  |= (1<<INT0);                 // Enable external interrupt pin INT0
	MCUCR |= (1<<ISC01);				//enable falling edge
	SREG  |= (1<<7); 					// enable I bit
}

void INT1_Init(void){
	SREG  &= ~(1<<7);
	DDRD  &= ~(1<<PD3);               // Configure INT1 as input pin
	GICR  |= (1<<INT1);                 // Enable external interrupt pin INT1
	MCUCR |= (1<<ISC11) | (1<<ISC10);	//enable rising edge
	SREG  |= (1<<7); 					// enable I bit
}

void INT2_Init(void){
	SREG  &= ~(1<<7);
	DDRB  &= ~(1<<PB2);               // Configure INT2 as input pin
	PORTB |= (1<<PB2);					// enable the pull up resistor
	GICR  |= (1<<INT2);                 // Enable external interrupt pin INT2
	MCUCSR &= ~(1<<ISC2); 				//enable falling edge
	SREG  |= (1<<7);  					// enable I bit
}


ISR(INT0_vect){
			hour2=0;
			hour1=0;
			min2=0;
			min1=0;
			sec2=0;
			sec1=0;
			TCNT1=0;
}

ISR(INT1_vect){
	TCCR1B &= ~(1<<CS12) & ~(1<<CS11) & ~(1<<CS10);      //disable the clock so the stop watch is paused
}

ISR(INT2_vect){
	TCCR1B |= (1<<CS12) | (1<<CS10);      //enable the clock so the stop watch is resumed
}


ISR(TIMER1_COMPA_vect){
	sec1++;
	if(sec1==10)
	{
		sec1=0;
		sec2++;
	}
	if(sec2==6)
	{
		sec2=0;
		min1++;
	}
	if(min1==10)
	{
		min1=0;
		min2++;
	}
	if(min2==6)
	{
		min2=0;
		hour1++;
	}
	if(hour1==10)
	{
		hour1=0;
		hour2++;
	}
	if(hour2==2 && hour1==4)
	{
		hour2=0;
		hour1=0;
		min2=0;
		min1=0;
		sec2=0;
		sec1=0;
	}
}

int main(void){
	DDRC |=0x0f;				//Configure first 4 pin in port c as output pins
	PORTC &=0xf0;
	DDRA |=0x3f;				//Configure first 6 pin in port a as output pins
	PORTA &=0xc0;
	INT0_Init();
	INT1_Init();
	Timer1_CTC();
	INT2_Init();
	while(1)
	{
		PORTA=1;
		PORTC=sec1;
		_delay_ms(1);
		PORTA=(PORTA<<1);
		PORTC=sec2;
		_delay_ms(1);
		PORTA=(PORTA<<1);
		PORTC=min1;
		_delay_ms(1);
		PORTA=(PORTA<<1);
		PORTC=min2;
		_delay_ms(1);
		PORTA=(PORTA<<1);
		PORTC=hour1;
		_delay_ms(1);
		PORTA=(PORTA<<1);
		PORTC=hour2;
		_delay_ms(1);
	}

}
