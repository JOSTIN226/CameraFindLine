//#include "MPC5604B_M27V.h"
#include "MPC5604B.h" 
#include "init.h" 
#include "video.h"
#include "analyse.h"
#include "control.h"
#include "Nokia5110.h"
#include "KeysSwitchs.h"
#include "IntcInterrupts.h" 




#define byte unsigned char
#define word unsigned int


/***************foreward*****************/
#define foreward SIU.GPDI[59].R	//����⳵ʵ�ʷ���:1ǰ�� 0����

/***************keys*****************/
#define key1 SIU.GPDI[46].R
#define key2 SIU.GPDI[2].R
#define key3 SIU.GPDI[64].R	//E0
#define key4 SIU.GPDI[1].R	//A1
/***************switchs*****************/
#define switch1 SIU.GPDI[56].R
#define switch2 SIU.GPDI[54].R
#define switch3 SIU.GPDI[52].R
#define switch4 SIU.GPDI[50].R
/***************LEDs*****************/
#define D0  SIU.GPDO[12].R
#define D1  SIU.GPDO[13].R
#define D2  SIU.GPDO[14].R
#define D3  SIU.GPDO[15].R
/***************����IO��*****************/
#define TestIO SIU.GPDO[11].R

