#include "includes.h"
void ModeGo(void);			//mode0-4
void ModeNokia5110(void);	//mode8-11
void ModeGoStraight(void);	//mode14
void ModeBluetooth(void);	//mode15


int main(void) {
  
	initALL();

  
  /* Loop forever */
  for (;;) 
  {
	ChooseMode();
	if(mode==0||mode==1||mode==2||mode==3||mode==4) ModeGo();    	 	//正常跑车
	else if(mode==8||mode==9||mode==10||mode==11||mode==12)   ModeNokia5110();	//显示屏
	//8----图像对中
	//9----图像+中线+target_offset+舵机输出值
	//10---图像+中线+赛道类型及参数
	//11---三条线+参数
	else if(mode==14) ModeGoStraight();	//mode14
	else if(mode==15) ModeBluetooth();									//蓝牙发送
  }
}



void ModeGo(void)
{	
	initPIT();
	initEMIOS_0ModulusCounter();

 	EMIOS_0.CH[3].CCR.B.FEN=1;  //开场中断
 	for(;;) 
  	{		
		if(fieldover)//本场采样结束标志
  	  	{
      		fieldover=0;         //标志清零
  			FindBlackLine();
			SteerControl();
			/*		LINFlex_TX(0xFF);
					LINFlex_TX(targetspeed);
					LINFlex_TX(currentspeed);
				 	LINFlex_TX(0xFE);
		    */
		    if(Slope==0)
		    	LINFlex_TX(RoadType);
		    else if(Slope==1)
		    	LINFlex_TX(0x31);
		    else if(Slope==2)
		    	LINFlex_TX(0x32);
		    EMIOS_0.CH[3].CSR.B.FLAG = 1;//清除场中断标志位	
		    EMIOS_0.CH[3].CCR.B.FEN=1;  //开场中断
    	}
  	}
}

void ModeNokia5110(void)
{
	EMIOS_0.CH[3].CCR.B.FEN=1;  //开场中断
	LCD_Init();
	for(;;) 
  	{
		if(fieldover)   
  	  	{//本场采样结束标志
  	  		fieldover=0;         //标志清零
  			
  			FindBlackLine();
 			SteerControl();
 			if(mode==11) ThreeLineNoVideo();
 			else CenterLineWithVideo();
 			
  	  		LCD_Show();
  
		    EMIOS_0.CH[3].CSR.B.FLAG = 1;//清除场中断标志位	
		    EMIOS_0.CH[3].CCR.B.FEN=1;  //开场中断
    	}
  	}
}

void ModeGoStraight(void)
{
	EMIOS_0.CH[3].CCR.B.FEN=1;  //开场中断
	LCD_Init();
	for(;;) 
  	{
		if(fieldover)   
  	  	{//本场采样结束标志
  	  		fieldover=0;         //标志清零
  	  		SET_steer(CENTER);
  	  		
  	  		LCD_write_english_string(0,0,"LEFT>RIGHT");
  	  		LCD_write_english_string(0,2,"key1-LEFT");
  	  		LCD_write_english_string(0,3,"key2-RIGHT");
			LCD_write_english_string(0,5,"CENTER");
			LCD_Write_Num(40,5,CENTER,4);
			ChangeCENTER();

		    EMIOS_0.CH[3].CSR.B.FLAG = 1;//清除场中断标志位	
		    EMIOS_0.CH[3].CCR.B.FEN=1;  //开场中断
    	}
  	}
}

void ModeBluetooth(void)
{
//	SetupBKL();
	SetupCCD();	
 	EMIOS_0.CH[3].CCR.B.FEN=1;  //开场中断
 	for(;;) 
  	{		
		if(fieldover)   
  	  	{//本场采样结束标志
      		fieldover=0;         //标志清零
 			FindBlackLine();
			SteerControl();
 			
		    Send_CCD_Video(); 
//		    Send_CCD_Blackline();
			
		    EMIOS_0.CH[3].CSR.B.FLAG = 1;//清除场中断标志位	
		    EMIOS_0.CH[3].CCR.B.FEN=1;  //开场中断
    	}
  	}
}
