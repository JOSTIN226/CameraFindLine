#ifndef __CONTROL_H__
#define __CONTROL_H__


#define Motor_PWM_MAX (200)
#define Motor_PWM_MIN (-100)


//**********************�������**********************************************
extern signed long target_offset,last_offset;
extern double Steer_kp,Steer_kd;//���P��Dֵ
extern unsigned int Steer_PWM[4];//������ֵ��¼


//**********************�������**********************************************
extern word speedcounter1,speedcounter2;
extern signed int currentspeed,targetspeed;		//��ǰ�ٶȡ�Ŀ���ٶ�
extern signed int Error,PreError,SumError;

extern double Speed_kp,Speed_ki,Speed_kd;	//���PIDֵ
extern signed int Motor_PWM;				//����ײ������

//********************�������Բ���******************************************
extern unsigned int Counter_Error;		//���Ӵ����ο����������
extern unsigned int StartTime2s;		//��ʼ��ʱ�����(��2s,��ʱһ��ʱ���ټ����ʼ��,�ڶ��μ�����ʼ��ͣ��)
extern byte stop_delay;
extern byte stop_flag;

//********************�Ƕȡ��ٶȿ���******************************************
extern void PitISR(void);
void SteerControl(void);
void SpeedControl(void);




#endif