#include "includes.h"

//*****************************************************************************************************************
//************************************************控制参数************************************************    	  *
//*****************************************************************************************************************

//**********************舵机参数**********************************************
signed long target_offset=0,last_offset=0;	//舵机偏差值记录
double Steer_kp=0,Steer_kd=0;//舵机P、D值
unsigned int Steer_PWM[4]={0,0,0,0};//舵机输出值记录


//**********************电机参数**********************************************
word speedcounter1=0,speedcounter2=0;
signed int currentspeed=0,targetspeed=0;		//当前速度、目标速度
signed int Error=0,PreError=0,SumError=0;

double Speed_kp=0,Speed_ki=0,Speed_kd=0;	//电机PID值
signed int Motor_PWM=0;				//电机底层控制量

//********************辅助调试参数******************************************
unsigned int Counter_Error=0;		//光编接触不牢靠错误计数量
unsigned int StartTime2s=0;			//起始线时间计算(起车2s,延时一段时间再检测起始线,第二次见到起始线停车)
////********************起始线停车参数******************************************
byte stop_flag=0;
byte stop_delay=0;
	

//*****************************************************************************************************************
//************************************************角度控制************************************************    	  *
//*****************************************************************************************************************
void SteerControl()
{
	//*1***********出错图像角度控制,输出为前三次平均值**************
	if(RoadType==NoLine||RoadType==Wrong) {
		Steer_PWM[3]=(Steer_PWM[2]+Steer_PWM[1])/2;
		SET_steer(Steer_PWM[3]);
		//存舵机值
		Steer_PWM[0]=Steer_PWM[1];Steer_PWM[1]=Steer_PWM[2];Steer_PWM[2]=Steer_PWM[3];
		return;
	}
	
	if(mode==0)//最高速
	{
		if(Slope==1)					{Steer_kp=10;Steer_kd=5;}
		else if(Slope==2)				{Steer_kp=8;Steer_kd=5;}
		
		else if(RoadType==Straight)		{Steer_kp=3+ABS(target_offset)*0.4;Steer_kd=10;}
		
		else if(ABS(target_offset)<6) 	{Steer_kp=5;Steer_kd=5;}
		else if(ABS(target_offset)<26)  {Steer_kp=15+target_offset*target_offset/100;Steer_kd=15;}
		else {Steer_kp=15.5+target_offset*target_offset/550;Steer_kd=4;}
	}
	
	else if(mode==1)//次高速(华东赛比较靠谱)
	{
		if(Slope==1)					{Steer_kp=10;Steer_kd=5;}
		else if(Slope==2)				{Steer_kp=8;Steer_kd=5;}
		
		else if(RoadType==Straight)		{Steer_kp=3+ABS(target_offset)*0.4;Steer_kd=10;}
		
		else if(ABS(target_offset)<6) 	{Steer_kp=5;Steer_kd=5;}
		else if(ABS(target_offset)<26)  {Steer_kp=15+target_offset*target_offset/100;Steer_kd=12;}
		else {Steer_kp=16+target_offset*target_offset/500;Steer_kd=8;}
	}
	else if(mode==2)//再低速
	{
		if(Slope==1)					{Steer_kp=10;Steer_kd=5;}
		else if(Slope==2)				{Steer_kp=8;Steer_kd=5;}
		
		else if(ABS(target_offset)<6) 	{Steer_kp=5;Steer_kd=5;}
		else if(ABS(target_offset)<26)  {Steer_kp=15+target_offset*target_offset/100;Steer_kd=10;}
		else {Steer_kp=15.5+target_offset*target_offset/500;Steer_kd=5;}
	}
	else if(mode==3)//最低速
	{
		if(Slope==1)					{Steer_kp=10;Steer_kd=5;}
		else if(Slope==2)				{Steer_kp=8;Steer_kd=5;}
		
		else if(ABS(target_offset)<6) 	{Steer_kp=5;Steer_kd=5;}
		else if(ABS(target_offset)<26)  {Steer_kp=15.2+target_offset*target_offset/100;Steer_kd=10;}
		else {Steer_kp=15.8+target_offset*target_offset/500;Steer_kd=5;}
	}
	
	else//刷图像时的打角
	{
		if(Slope==1)					{Steer_kp=8;Steer_kd=5;}
		else if(Slope==2)				{Steer_kp=8;Steer_kd=5;}
		
		else if(ABS(target_offset)<6) 	{Steer_kp=5;Steer_kd=5;}
		else if(ABS(target_offset)<26)  {Steer_kp=15+target_offset*target_offset/100;Steer_kd=20;}
		else {Steer_kp=15.8+target_offset*target_offset/500;Steer_kd=8;}	
	}


	
	Steer_PWM[3]=CENTER-Steer_kp*target_offset-Steer_kd*(target_offset-last_offset);
	//if(ABS(Steer_PWM[3]-Steer_PWM[2])>250) Steer_PWM[3]=(Steer_PWM[2]+Steer_PWM[1])/2;
	//感觉不太靠谱，调的不好
	
	//舵机限值+舵机输出
	if(Steer_PWM[3]>LEFT) Steer_PWM[3]=LEFT;
	else if(Steer_PWM[3]<RIGHT) Steer_PWM[3]=RIGHT;
	SET_steer(Steer_PWM[3]);
	
	//存舵机值和offset值
	Steer_PWM[0]=Steer_PWM[1];Steer_PWM[1]=Steer_PWM[2];Steer_PWM[2]=Steer_PWM[3];
	last_offset=target_offset;
}


//*****************************************************************************************************************
//************************************************PIT中断************************************************    	  *
//*****************************************************************************************************************

void PitISR(void)//10ms一个控制周期
{  
    if(StartTime2s<200){StartTime2s++;}
	else{
    	//得到光编反馈值的绝对值及符号
		speedcounter1=EMIOS_0.CH[24].CCNTR.R;
		if(speedcounter1<speedcounter2)
		{
			currentspeed=speedcounter1+65536-speedcounter2;
		}
		else currentspeed=speedcounter1-speedcounter2;
		
		if(foreward) currentspeed=currentspeed;
		else currentspeed=-currentspeed;
	    speedcounter2=speedcounter1;
	    
	    
	    //光编错误计数
	    if(currentspeed==0) Counter_Error++;
	    else Counter_Error=0;
	    
	    SpeedControl();
	}
  	PIT.CH[1].TFLG.B.TIF = 1;	//write 1 to clear PIT1 清除标志位
}

//*****************************************************************************************************************
//************************************************速度控制************************************************    	  *
//*****************************************************************************************************************
void SpeedControl()
{
	//1*******************************起始线停车速度控制及光编线接触不牢控制***********************
	if(StartLine){
    	stop_delay++;
    	if(mode==0||mode==1)
    	{
    		if(stop_delay==35){
		    	if(stop_flag==2) SET_motor(0);
		    	else if(foreward) SET_motor(-300);
		    	else stop_flag++;
		    	stop_delay--;
		      	return;
		    }
    	}
    	else 
    	{
    		if(stop_delay==43){
		    	if(stop_flag==2) SET_motor(0);
		    	else if(foreward) SET_motor(-300);
		    	else stop_flag++;
		    	stop_delay--;
		      	return;
		    }
    	}
	}
   	if(Counter_Error>50) {SET_motor(0);return;}
   	
   	StartTime2s++;
	//2*****正常速度控制...速度分配还要结合最远行和offset
   if(mode==0)//最高速
   {
	    if(Slope==1)				{targetspeed=220;
	    								Speed_kp=6.5;Speed_ki=0.1;Speed_kd=0.2;}
	    else if(Slope==2)			{targetspeed=165;
	    								Speed_kp=6;Speed_ki=0.1;Speed_kd=0.2;}
	    								
	    else if(RoadType==Straight)	{targetspeed=280;
	    								Speed_kp=5.5;Speed_ki=0.1;Speed_kd=0.2;}
	    else if(RoadType==SmallS)	{targetspeed=240;
	    								Speed_kp=5.5;Speed_ki=0.1;Speed_kd=0.2;}
	    else if(RoadType==ToT)		{targetspeed=180;
	    								Speed_kp=6;Speed_ki=0.1;Speed_kd=0.2;}
	    else if(RoadType==UTurn)	{targetspeed=148;
	    								Speed_kp=7;Speed_ki=0.1;Speed_kd=0.2;}
	    								
	    else if(RoadEnd<15)			{targetspeed=200;
	    								Speed_kp=6.5;Speed_ki=0.1;Speed_kd=0.2;}
	    else if(RoadEnd<30)			{targetspeed=180-target_offset*target_offset/35;
	    								Speed_kp=6.5;Speed_ki=0.2;Speed_kd=0.2;}
	    else						{targetspeed=148;
	    								Speed_kp=7;Speed_ki=0.2;Speed_kd=0.3;}
   }
   else if(mode==1)//次高速
   {
	    if(Slope==1)				{targetspeed=220;
	    								Speed_kp=6.5;Speed_ki=0.1;Speed_kd=0.2;}
	    else if(Slope==2)			{targetspeed=165;
	    								Speed_kp=6;Speed_ki=0.1;Speed_kd=0.2;}
	    								
	    else if(RoadType==Straight)	{targetspeed=265;
	    								Speed_kp=5.5;Speed_ki=0.1;Speed_kd=0.2;}
	    else if(RoadType==SmallS)	{targetspeed=235;
	    								Speed_kp=5.5;Speed_ki=0.1;Speed_kd=0.2;}
	    else if(RoadType==ToT)		{targetspeed=170;
	    								Speed_kp=6;Speed_ki=0.1;Speed_kd=0.2;}
	    else if(RoadType==UTurn)	{targetspeed=145;
	    								Speed_kp=5.5;Speed_ki=0.1;Speed_kd=0.2;}
	    								
	    else if(RoadEnd<15)			{targetspeed=187;
	    								Speed_kp=5.5;Speed_ki=0.1;Speed_kd=0.2;}
	    else if(RoadEnd<30)			{targetspeed=172-target_offset*target_offset/35;
	    								Speed_kp=5.5;Speed_ki=0.2;Speed_kd=0.2;}
	    else						{targetspeed=145;
	    								Speed_kp=7;Speed_ki=0.2;Speed_kd=0.3;}
   }
   else if(mode==2)//再低速
   {
	    if(Slope==1)				{targetspeed=180;
	    								Speed_kp=6.5;Speed_ki=0.1;Speed_kd=0.2;}
	    else if(Slope==2)			{targetspeed=140;
	    								Speed_kp=6;Speed_ki=0.1;Speed_kd=0.2;}
	    								
	    else if(RoadEnd<15)			{targetspeed=180;
	    								Speed_kp=5.5;Speed_ki=0.1;Speed_kd=0.2;}
	    else if(RoadEnd<30)			{targetspeed=160-target_offset*target_offset/35;
	    								Speed_kp=5.5;Speed_ki=0.2;Speed_kd=0.2;}
	    else						{targetspeed=135;
	    								Speed_kp=7.5;Speed_ki=0.2;Speed_kd=0.3;}
   }
   else if(mode==3)//最低速
   {
	    if(Slope==1)				{targetspeed=180;
	    								Speed_kp=6.5;Speed_ki=0.1;Speed_kd=0.2;}
	    else if(Slope==2)			{targetspeed=140;
	    								Speed_kp=6;Speed_ki=0.1;Speed_kd=0.2;}
	    								
	    else if(RoadEnd<15)			{targetspeed=175;
	    								Speed_kp=5.5;Speed_ki=0.1;Speed_kd=0.2;}
	    else if(RoadEnd<30)			{targetspeed=155-target_offset*target_offset/40;
	    								Speed_kp=5.5;Speed_ki=0.2;Speed_kd=0.2;}
	    else						{targetspeed=130;
	    								Speed_kp=5.5;Speed_ki=0.2;Speed_kd=0.2;}
   }


	if(StartTime2s<290)	{if(currentspeed>targetspeed) SumError=0;}
	else {StartTime2s--;}
	
	
	
    Error=(signed int)(targetspeed)-(signed int)(currentspeed);
    
    SumError+=Error;
    if(SumError>3000) SumError=3000;
    if(SumError<-3000) SumError=-3000;
    

   	Motor_PWM=Speed_kp*Error+Speed_ki*SumError+Speed_kd*(Error-PreError);
   	
    if(Motor_PWM>Motor_PWM_MAX)  Motor_PWM=Motor_PWM_MAX;
	else if(Motor_PWM<Motor_PWM_MIN)  Motor_PWM=Motor_PWM_MIN;
	SET_motor(Motor_PWM);
	
	PreError=Error;
}