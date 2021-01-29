# include <avr/io.h>
# include <util/delay.h>
# include <avr/interrupt.h>

# define ENCODER_1_A_REGISTER DDRE // PIN 3
# define ENCODER_1_A_PORT PORTE
# define ENCODER_1_A_INPUT PINE
# define ENCODER_1_A_PIN PE5

# define ENCODER_1_B_REGISTER DDRA // PIN   22
# define ENCODER_1_B_PORT PORTA
# define ENCODER_1_B_INPUT PINA
# define ENCODER_1_B_PIN PA0     //pin 22

# define ENCODER_2_A_REGISTER DDRE  //

int encoder_1_init();
int encoder_2_init();



# define ENCODER_2_A_PORT PORTE
# define ENCODER_2_A_INPUT PINE
# define ENCODER_2_A_PIN PE4 //pin 2

# define ENCODER_2_B_REGISTER DDRA //
# define ENCODER_2_B_PORT PORTA
# define ENCODER_2_B_INPUT PINA
# define ENCODER_2_B_PIN PA2    // 24


int countEncoderOne = 0;
int countEncoderTwo = 0;

int encoder_1_init()
{
	ENCODER_1_A_REGISTER |=
	0
	| (0 << ENCODER_1_A_PIN)
	;
	ENCODER_1_B_REGISTER |=
	0
	| (0 << ENCODER_1_B_PIN)
	;


	ENCODER_1_A_PORT |=
	0
	| (1 << ENCODER_1_A_PIN)
	;
	ENCODER_1_B_PORT |=
	0
	| (1 << ENCODER_1_B_PIN)
	;
	
	EIMSK |=
	0
	|(1 << INT5)
	;
	EICRB |=
	0
	| (0 << ISC50) // INT5 : ANY EDGE WILL GENRATE INTURRUPT
	| (1 << ISC51)
	;
	sei();
}


int encoder_2_init()
{
	ENCODER_2_A_REGISTER |=
	0
	| (0 << ENCODER_2_A_PIN)
	;
	ENCODER_2_B_REGISTER |=
	0
	| (0 << ENCODER_2_B_PIN)
	;

	ENCODER_2_A_PORT |=
	0
	| (1 << ENCODER_2_A_PIN)
	;
	ENCODER_2_B_PORT |=
	0
	| (1 << ENCODER_2_B_PIN)
	;
	EIMSK |=
	0
	|(1 << INT4)
	;
	EICRB |=
	0
	| (0 << ISC40) // INT5 : ANY EDGE WILL GENRATE INTURRUPT
	| (1 << ISC41)
	;
	sei();
}

ISR (INT5_vect)
{
	if(bit_is_clear(ENCODER_1_B_INPUT,ENCODER_1_B_PIN))
	{
		countEncoderOne++;
	}
	else
	{
		countEncoderOne--;
	}
}




ISR (INT4_vect)
{
	
	if(bit_is_clear(ENCODER_2_B_INPUT,ENCODER_2_B_PIN))
	{
		countEncoderTwo++;
	}
	else
	{
		countEncoderTwo--;
	}
}
