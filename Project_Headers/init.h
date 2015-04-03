#ifndef __INITIAL_H__
#define __INITIAL_H__

//*************************************************************************
//*	 *************************��ʼ������*************************	      *
//*************************************************************************
void initModesAndClock(void);			//ʱ�ӳ�ʼ��80M
void disableWatchdog(void);				//�ؿ��Ź�
void initEMIOS_0MotorAndSteer(void);	//������PWM��ʼ��   
void initEMIOS_0Image(void);			//�����жϳ�ʼ��
void initPIT(void);						//PIT��ʱ��ʼ��10ms
void initEMIOS_0ModulusCounter(void);	//��������ʼ��(���)
void initLINFlex_0_UART (void);			//SCI��ʼ�� 57600
void initNokia5110(void);				//��ʾ����ʼ��
void initTestIO(void);					//LED+����IO
void initKeys_Switchs(void);			//����+���뿪��
void enableIrq(void);					//�����ж�

void initADC(void);						//ADģ���ʼ��δʹ��
void initALL(void);



//*************************************************************************
//*		*************************�ӿں���***********************	      *
//*************************************************************************

//***********************�������ӿں���**********************************************************************//
void SET_steer(unsigned int steer);	//50Hz ռ�ձ�:7.5%+-5% ��λ��50000*7.5%=3750
void SET_motor(signed int speed); //5kHz ����ת,Ҫ��λ     
//*************************�����ӿں���************************************************************************//
void LINFlex_TX(unsigned char data);
void LINFlex_RX_Interrupt(void);

#endif
