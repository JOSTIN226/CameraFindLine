#ifndef __INITIAL_H__
#define __INITIAL_H__

//*************************************************************************
//*	 *************************初始化函数*************************	      *
//*************************************************************************
void initModesAndClock(void);			//时钟初始化80M
void disableWatchdog(void);				//关看门狗
void initEMIOS_0MotorAndSteer(void);	//电机舵机PWM初始化   
void initEMIOS_0Image(void);			//场行中断初始化
void initPIT(void);						//PIT定时初始化10ms
void initEMIOS_0ModulusCounter(void);	//计数器初始化(光编)
void initLINFlex_0_UART (void);			//SCI初始化 57600
void initNokia5110(void);				//显示屏初始化
void initTestIO(void);					//LED+测试IO
void initKeys_Switchs(void);			//按键+拨码开关
void enableIrq(void);					//开总中断

void initADC(void);						//AD模块初始化未使用
void initALL(void);



//*************************************************************************
//*		*************************接口函数***********************	      *
//*************************************************************************

//***********************电机舵机接口函数**********************************************************************//
void SET_steer(unsigned int steer);	//50Hz 占空比:7.5%+-5% 中位：50000*7.5%=3750
void SET_motor(signed int speed); //5kHz 正反转,要限位     
//*************************蓝牙接口函数************************************************************************//
void LINFlex_TX(unsigned char data);
void LINFlex_RX_Interrupt(void);

#endif
