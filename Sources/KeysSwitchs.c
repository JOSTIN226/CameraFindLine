#include "includes.h"

byte mode=0;
byte RowSAIC_DelayTime=43;
byte ChangeCENTERFlag=0;

unsigned int CENTER=3943;
unsigned int RIGHT=3053;
unsigned int LEFT=4833;

//*****************************************************************************************************************
//****************************************拨码开关函数*****************************************************    	  *
//*****************************************************************************************************************
void ChooseMode(void)
{
	mode=15-(switch1*8+switch2*4+switch3*2+switch4);
}

//*****************************************************************************************************************
//****************************************按键函数*********************************************************    	  *
//*****************************************************************************************************************
void DelayKeys()
{
	byte i;
	for(i=0;i<100;i++){}
}

//******************************************改图像延时时间*************************************
//按键1加DelayTime,按键2减DelayTime
void ChangeVideoDelayTime(void)
{
	if(key1==0)
	{
		DelayKeys();
		if(key1==0)	{while(!key1);RowSAIC_DelayTime++;}
	}
	
	if(key2==0)
	{
		DelayKeys();
		if(key2==0)	{while(!key2);RowSAIC_DelayTime--;}
	}
}


void ChangeCENTER(void)
{
	if(key1==0)
	{
		DelayKeys();
		if(key1==0)	{while(!key1);CENTER++;}//向左
	}
	
	if(key2==0)
	{
		DelayKeys();
		if(key2==0)	{while(!key2);CENTER--;}//向右
	}
	
	if(key3==0)
	{
		DelayKeys();
		if(key3==0)	{
			while(!key3);
			ChangeCENTERFlag++;
			if(ChangeCENTERFlag==2) ChangeCENTERFlag=0;
		}
	}
	
	if(ChangeCENTERFlag)
	{
		SET_motor(80);
	}
	else
	{
		SET_motor(0);
	}
}