/*
 * main.cpp
 *
 *  Created on: 30 ���. 2018 �.
 *      Author: hudienko_a
 */
#include <avr/io.h>
#include <util/delay.h>

//#include <avr/interrupt.h>

//#include"EngineLogics/Benzo.h"
#include "Driver/Stepper.h"
#include "Driver/Adc.h"
//Benzo _benz;
Stepper _step;

//����� ����������� ���� 180��������
int counter=0;
bool continue_=true;

bool SearchStart()
{
	continue_=true;
	counter=0;
	//����� ���������� ���������
		while(continue_)
		{
			if((PINC &(1<<0))==0 )
			{
				PORTC&=~(1<<2);
				continue_ = false;
			}
			else
			{
				PORTC|=(1<<2);
			}

			if(!continue_) break;

			if(counter++>=310)
			{
				break;
			}
			_step.MultiStep(5,false);
		}

		if(counter>=310) return false;
		return true;
}

bool SearchEnd()
{
	counter=0;
	continue_ = true;
	PORTC|=(1<<2);

	//����� ��������� ���������
	while(continue_)
	{

		if((PINC &(1<<1))==0 )
		{
			PORTC&=~(1<<2);
			continue_ =false;
		}
		else
		{
			PORTC|=(1<<2);
		}

		if(!continue_) break;

		if(counter++>=310)
		{
			break;
		}
		_step.MultiStep(5,true);
	}

	if(counter++>=310) return false;

	counter = counter*5;
	return true;
}

int main(void)
{

	//_benz.init();
	DDRC=0xFF;
	PORTC=0x00;
	DDRC &= ~(1 << 0);
	DDRC &= ~(1 << 1);
	DDRC &= ~(1 << 5);
	PORTC |= (1 << 0);
	PORTC |= (1 << 1);
	PORTC &=~(1 << 5);

	PORTC|=(1<<2);


	DDRD =0xFF;
	PORTD=0x00;
	_step.init();
	adc_init();

	int current_stap =0;
	int adc=0;
	int _proAdc=0;

	if(!SearchStart()) {while(true);}

	if(!SearchEnd()){ SearchStart(); while(true);}

	//������� �� �����
	_step.MultiStep(counter,false);

	for(;;)
	{
		//_benz.processing();
		if((PINC &(1<<0))==0 || (PINC &(1<<1))==0 )
		{
			PORTC&=~(1<<2);
		}
		else
		{
			PORTC|=(1<<2);
		}

		_proAdc = adc_read(5);

		adc = (_proAdc*1.22);

		if(current_stap<adc  && (adc-current_stap)>10 && (PINC &(1<<1))!=0 )
		{
			//_step.MultiStep((adc-current_stap),true);
			_step.MultiStep(5,true);
			current_stap+=5;// = adc;
		}
		else if(current_stap>adc && (current_stap-adc)>10 && (PINC &(1<<0))!=0 )
		{
			//_step.MultiStep((current_stap-adc),false);
			_step.MultiStep(5,false);
			current_stap-=5;// = adc;
		}
		else
		{

		}




		//_delay_ms(10);

	}

return 0;
} //main ends here


