#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay_basic.h>
#include "robopoly.h"

#define VITESSE_SERVO 10

#define POSER 0
#define PRENDRE 1

#define PINCE_OUVERTE 25
#define PINCE_FERMEE 45


void bouger_bras(int pos, int action);

static unsigned char pos_servo[5][4][4] = {{{50, 80, 30, 0},{50, 60, 80, 80},
											{36, 75, 60, 0},{36, 60, 69, 0}}

											,{{50, 80, 30, 0},{50, 60, 80, 80},
											{64, 75, 60, 0},{64, 60, 69, 0}}

											,{{50, 80, 30, 0},{50, 60, 80, 80},
											{50, 65, 50, 0},{50, 55, 63, 13}}

											,{{50, 80, 30, 0},{50, 60, 80, 80},
											{42, 60, 30, 0},{42, 38, 40, 0}}

											,{{50, 80, 30, 0},{50, 60, 80, 80},
											{58, 60, 30, 0},{58, 38, 40, 0}}};

static unsigned char type_pos;
static unsigned char type_pos_prochain;

int main(void)
{
	DDRC = 0xFF;

	PORTC = 0b100;
	unsigned char etat_pince = 1;
	unsigned char change_pos = 0;


	// tab[a][b] = {{b}*a}
	// tableau 3D fuck yeah! d'abord position à atteindre, puis position courante et enfin servo
	

	

	unsigned char etat_capteur = (~digitalRead('A', BYTE))&0b00011111;
	type_pos = 255;
	if(etat_capteur != 0)
	{
		for(int i = 0; i<5 ; i++)
		{
			if((etat_capteur)&(0x01<<i))
			{
				if(type_pos == 255)
					type_pos = i;
			}
			
		}
		if(type_pos == 255)
				PORTC = 0;
	}
	else 
		PORTC = 0;

	/*
//	pos cobra
	set_servo(5, 50);
	set_servo(6, 80);
	set_servo(7, 30);
	set_servo(8, 0);
	set_servo(9, 30);*/

/*	pos bas
	set_servo(5, 50);
	set_servo(6, 60);
	set_servo(7, 80);
	set_servo(8, 80);
	set_servo(9, 30);*/

/*	pos 3 h
	set_servo(5, 50);
	set_servo(6, 65);
	set_servo(7, 50);
	set_servo(8, 0);
	set_servo(9, 30);*/

/*	//pos 3 b
	set_servo(5, 50);
	set_servo(6, 55);
	set_servo(7, 57);
	set_servo(8, 4);
	set_servo(9, 30);*/

	set_servo(5, pos_servo[type_pos][0][0]);
	set_servo(6, pos_servo[type_pos][0][1]);
	set_servo(7, pos_servo[type_pos][0][2]);
	set_servo(8, pos_servo[type_pos][0][3]);

	
	set_servo(9, 25);

/*	if(type_pos != 255)
		bouger_bras(type_pos, PRENDRE);*/

	while(type_pos != 255)
	{
	/*	if(digitalRead('A', 3)) //voit rien
		{
			set_servo(9, 25);
		}
		else
		{
			set_servo(9, 45);
		}
	
*/
		PORTC = 0b100;
		etat_capteur = (~digitalRead('A', BYTE))&0b00011111;
		if(etat_capteur != 0)
		{
			for(int i = 0; i<5 ; i++)
			{
				if((etat_capteur)&(0x01<<i) && ((0x01<<i) != 1<<type_pos) && change_pos == 0)
				{
					type_pos_prochain = i;
					change_pos = 1;
				}
			}
		}
		else 
			PORTC = 0;

		if(change_pos && (etat_capteur&~(0b11100000|1<<type_pos) )== 0)
		{
			bouger_bras(type_pos, PRENDRE);
			bouger_bras(type_pos_prochain, POSER);
			type_pos = type_pos_prochain;
			change_pos = 0;
		}
		
	
	/*	if(type_pos > 3)
			type_pos = 0;
		else
			type_pos++;*/
		
	}
}

void bouger_bras(int pos, int action)
{
	if(action == PRENDRE)
		set_servo(9, PINCE_OUVERTE);
	else
		set_servo(9, PINCE_FERMEE);

	for(int i = 0; i < 100; i++)
	{
		set_servo(5, pos_servo[pos][0][0] + ((pos_servo[pos][1][0] - pos_servo[pos][0][0])*i)/100);
		set_servo(6, pos_servo[pos][0][1] + ((pos_servo[pos][1][1] - pos_servo[pos][0][1])*i)/100);
		set_servo(7, pos_servo[pos][0][2] + ((pos_servo[pos][1][2] - pos_servo[pos][0][2])*i)/100);
		set_servo(8, pos_servo[pos][0][3] + ((pos_servo[pos][1][3] - pos_servo[pos][0][3])*i)/100);
		waitms(VITESSE_SERVO);
	}
	for(int i = 0; i < 100; i++)
	{
		set_servo(5, pos_servo[pos][1][0] + ((pos_servo[pos][2][0] - pos_servo[pos][1][0])*i)/100);
		set_servo(6, pos_servo[pos][1][1] + ((pos_servo[pos][2][1] - pos_servo[pos][1][1])*i)/100);
		set_servo(7, pos_servo[pos][1][2] + ((pos_servo[pos][2][2] - pos_servo[pos][1][2])*i)/100);
		set_servo(8, pos_servo[pos][1][3] + ((pos_servo[pos][2][3] - pos_servo[pos][1][3])*i)/100);
		waitms(VITESSE_SERVO);
	}
	for(int i = 0; i < 100; i++)
	{
		set_servo(5, pos_servo[pos][2][0] + ((pos_servo[pos][3][0] - pos_servo[pos][2][0])*i)/100);
		set_servo(6, pos_servo[pos][2][1] + ((pos_servo[pos][3][1] - pos_servo[pos][2][1])*i)/100);
		set_servo(7, pos_servo[pos][2][2] + ((pos_servo[pos][3][2] - pos_servo[pos][2][2])*i)/100);
		set_servo(8, pos_servo[pos][2][3] + ((pos_servo[pos][3][3] - pos_servo[pos][2][3])*i)/100);
		waitms(VITESSE_SERVO);
	}
	waitms(1000);
	if(action)
		set_servo(9, PINCE_FERMEE);
	else
		set_servo(9, PINCE_OUVERTE);
	waitms(1000);
	for(int i = 0; i < 100; i++)
	{
		set_servo(5, pos_servo[pos][3][0] + ((pos_servo[pos][2][0] - pos_servo[pos][3][0])*i)/100);
		set_servo(6, pos_servo[pos][3][1] + ((pos_servo[pos][2][1] - pos_servo[pos][3][1])*i)/100);
		set_servo(7, pos_servo[pos][3][2] + ((pos_servo[pos][2][2] - pos_servo[pos][3][2])*i)/100);
		set_servo(8, pos_servo[pos][3][3] + ((pos_servo[pos][2][3] - pos_servo[pos][3][3])*i)/100);
		waitms(VITESSE_SERVO);
	}

	for(int i = 0; i < 100; i++)
	{
		set_servo(5, pos_servo[pos][2][0] + ((pos_servo[pos][1][0] - pos_servo[pos][2][0])*i)/100);
		set_servo(6, pos_servo[pos][2][1] + ((pos_servo[pos][1][1] - pos_servo[pos][2][1])*i)/100);
		set_servo(7, pos_servo[pos][2][2] + ((pos_servo[pos][1][2] - pos_servo[pos][2][2])*i)/100);
		set_servo(8, pos_servo[pos][2][3] + ((pos_servo[pos][1][3] - pos_servo[pos][2][3])*i)/100);
		waitms(VITESSE_SERVO);
	}
	for(int i = 0; i < 100; i++)
	{
		set_servo(5, pos_servo[pos][1][0] + ((pos_servo[pos][0][0] - pos_servo[pos][1][0])*i)/100);
		set_servo(6, pos_servo[pos][1][1] + ((pos_servo[pos][0][1] - pos_servo[pos][1][1])*i)/100);
		set_servo(7, pos_servo[pos][1][2] + ((pos_servo[pos][0][2] - pos_servo[pos][1][2])*i)/100);
		set_servo(8, pos_servo[pos][1][3] + ((pos_servo[pos][0][3] - pos_servo[pos][1][3])*i)/100);
		waitms(VITESSE_SERVO);
	}

}
