#include "includes.h"


void initALL(void)
{
	disableWatchdog();
	initModesAndClock();
	initEMIOS_0MotorAndSteer();
	initEMIOS_0Image();
	initPIT();
	initEMIOS_0ModulusCounter();
	initADC();
		
	initLINFlex_0_UART();
	initNokia5110();
	initKeys_Switchs();
	initTestIO();
	
	enableIrq();
}

//*****************************************************************************************************************
//*			 *************************sysclk初始化*******************************************************    	  *
//*****************************************************************************************************************
void initModesAndClock(void) 
{
    ME.MER.R = 0x0000001D;          /* Enable DRUN, RUN0, SAFE, RESET modes */
  //Initialize PLL before turning it on                    
    /*前16位:1.NDIV=[7:0](范围32-96);  2.ODF=2^([9:8]+1)   3.IDF=[13:10]+1（1111:Clock Inhibit）
           /*******  PHI=xtal*NDIV\(ODF*IDF)  ******/
    /*Note:Fvco=xtal*NDIV/IDF(范围在256 MHz to 512 MHz)
    /*后16位保持不变*/
  //设置sysclk
    //CGM.FMPLL_CR.R = 0x02400100;    /* 8 MHz xtal: Set PLL0 to 64 MHz */  
    CGM.FMPLL_CR.R = 0x01280000;    /* 8 MHz xtal: Set PLL0 to 80 MHz */
    //CGM.FMPLL_CR.R = 0x013C0000;    /* 8 MHz xtal: Set PLL0 to 120 MHz */   
    ME.RUN[0].R = 0x001F0064;       /* RUN0 cfg: 16MHzIRCON,OSC0ON,PLL0ON,syclk=PLL     sysclk选择锁相环时钟*/
    ME.RUNPC[0].R = 0x00000010;    /* Peri. Cfg. 1 settings: only run in RUN0 mode      选择RUN0模式*/
  
/*************************PCTL[?] 选择需要时钟模块****************************/
	//ME.PCTL[32].R = 0x00; 			/* MPC56xxB/P/S ADC 0: select ME.RUNPC[0] */
    //ME.PCTL[48].R = 0x00;           /* MPC56xxB/P/S LINFlex 0: select ME.RUNPC[0] */
    //ME.PCTL[68].R = 0x00;           /* MPC56xxB/S SIUL:  select ME.RUNPC[0] */ 
    //ME.PCTL[72].R = 0x00;           /* MPC56xxB/S EMIOS 0:  select ME.RUNPC[0] */ 
	
    //Mode Transition to enter RUN0 mode                                
    ME.MCTL.R = 0x40005AF0;         /* Enter RUN0 Mode & Key */
    ME.MCTL.R = 0x4000A50F;         /* Enter RUN0 Mode & Inverted Key */ 
    
    while (ME.GS.B.S_MTRANS) {}     // Wait for mode transition to complete 等待模式转换完成    
    /********************************** Note: could wait here using timer and/or I_TC IRQ*/                          
    while(ME.GS.B.S_CURRENTMODE != 4) {} // Verify RUN0 is the current mode 等待选择RUN0模式
  //开peri0、1、2
	CGM.SC_DC[0].R = 0x80;//LIN
	CGM.SC_DC[1].R = 0x80;//FLEXCAN,DSPI
    CGM.SC_DC[2].R = 0x80;//eMIOS,CTU,ADC
}

void disableWatchdog(void)
{
	SWT.SR.R = 0x0000c520; /* Write keys to clear soft lock bit */
	SWT.SR.R = 0x0000d928;
	SWT.CR.R = 0x8000010A; /* Clear watchdog enable (WEN) */
}
//*****************************************************************************************************************
//*************************	PWM初始化:电机正反转E5、E6；舵机输出A4   **********************************************
//*****************************************************************************************************************
void initEMIOS_0MotorAndSteer(void)
{
  //eMIOS0初始化80MHz分为1MHz
	EMIOS_0.MCR.B.GPRE= 31;   //GPRE+1=分频系数；/* Divide 80 MHz sysclk by 31+1 = 32 for 2.5MHz(0.4us) eMIOS clk*/
	EMIOS_0.MCR.B.GPREN = 1;	/* Enable eMIOS clock */
	EMIOS_0.MCR.B.GTBE = 1;   /* Enable global time base */
	EMIOS_0.MCR.B.FRZ = 1;    /* Enable stopping channels when in debug mode */
	
  /**********电机PWM 5kHZ E5、E6口*********************************************************************************/ 
    //eMIOS0D通道16设置/* EMIOS 0 CH 16: Modulus Up Counter */
    EMIOS_0.CH[16].CCR.B.UCPRE=0;	    /* Set channel prescaler to divide by 1 */
	EMIOS_0.CH[16].CCR.B.UCPEN = 1;   /* Enable prescaler; uses default divide by 1 */
	EMIOS_0.CH[16].CCR.B.FREN = 1; 	/* Freeze channel counting when in debug mode */
	EMIOS_0.CH[16].CADR.R = 500;	/********设置周期200us  5kHZ********/
	EMIOS_0.CH[16].CCR.B.MODE = 0x50; /* Modulus Counter Buffered (MCB) */
	EMIOS_0.CH[16].CCR.B.BSL = 0x3;	/* Use internal counter */
	
    /* EMIOS 0 CH 21前进输出: Output Pulse Width Modulation */
	EMIOS_0.CH[21].CCR.B.BSL = 0x1;	/* Use counter bus D (default) */
	EMIOS_0.CH[21].CCR.B.MODE = 0x60; /* Mode is OPWM Buffered */
    EMIOS_0.CH[21].CCR.B.EDPOL = 1;	/* Polarity-leading edge sets output/trailing clears*/
	EMIOS_0.CH[21].CADR.R = 0;     /* Leading edge when channel counter bus=250*/
	EMIOS_0.CH[21].CBDR.R = 0;      /* Trailing edge when channel counter bus=500*/
	SIU.PCR[69].R = 0x0600;    //[11:10]选择AFx 此处AF1 /* MPC56xxS: Assign EMIOS_0 ch 21 to pad */
	/* EMIOS 0 CH 22后退输出: Output Pulse Width Modulation */
	EMIOS_0.CH[22].CCR.B.BSL = 0x1;	/* Use counter bus D (default) */
	EMIOS_0.CH[22].CCR.B.MODE = 0x60; /* Mode is OPWM Buffered */
    EMIOS_0.CH[22].CCR.B.EDPOL = 1;	/* Polarity-leading edge sets output/trailing clears*/
	EMIOS_0.CH[22].CADR.R = 0;     /* Leading edge when channel counter bus=250*/
	EMIOS_0.CH[22].CBDR.R = 0;     /* Trailing edge when channel counter bus=500*/
	SIU.PCR[70].R = 0x0600;    //[11:10]选择AFx 此处AF1 /* MPC56xxS: Assign EMIOS_0 ch 21 to pad */
	
	
  /**********舵机PWM 50HZ A4口输出50000*7.5%=3750中位**********/
    //eMIOS0 B通道0设置/* EMIOS 0 CH 0: Modulus Counter */
    EMIOS_0.CH[0].CCR.B.UCPRE=0;	    /* Set channel prescaler to divide by 1 */
	EMIOS_0.CH[0].CCR.B.UCPEN = 1;   /* Enable prescaler; uses default divide by 1 */
	EMIOS_0.CH[0].CCR.B.FREN = 1; 	/* Freeze channel counting when in debug mode */
	EMIOS_0.CH[0].CADR.R = 50000;/********设置周期20ms  50HZ*******/
	EMIOS_0.CH[0].CCR.B.MODE = 0x50; /* Modulus Counter Buffered (MCB) */
	EMIOS_0.CH[0].CCR.B.BSL = 0x3;	/* Use internal counter */
	
    /* EMIOS 0 CH 4: Output Pulse Width Modulation */
	EMIOS_0.CH[4].CCR.B.BSL = 0x1;	/* Use counter bus B (default) */
	EMIOS_0.CH[4].CCR.B.MODE = 0x60; /* Mode is OPWM Buffered */  
    EMIOS_0.CH[4].CCR.B.EDPOL = 1;	/* Polarity-leading edge sets output/trailing clears*/
	EMIOS_0.CH[4].CADR.R = 0;//半占空比/* Leading edge when channel counter bus=250*/
	EMIOS_0.CH[4].CBDR.R = CENTER;            /* Trailing edge when channel counter bus=500*/
	SIU.PCR[4].R = 0x0600;    //[11:10]选择AFx 此处AF1   A4口舵机输出
}
/*************************电机接口函数*********************/
void SET_motor(signed int speed)
{
	if(speed>=0) {EMIOS_0.CH[21].CBDR.R = 0;EMIOS_0.CH[22].CBDR.R = speed;}
	else {EMIOS_0.CH[21].CBDR.R = -speed;EMIOS_0.CH[22].CBDR.R = 0;}
}
/*************************舵机接口函数***********************/
void SET_steer(unsigned int steer)
{EMIOS_0.CH[4].CBDR.R = steer;}



//*****************************************************************************************************************
//****************************************中断初始化******************************************************    	  *
//*****************************************************************************************************************

void enableIrq(void) {
  INTC.CPR.B.PRI = 0;          /* Single Core: Lower INTC's current priority */
  asm(" wrteei 1");	    	   /* Enable external interrupts */
}


//*****************************************************************************************************************
//************************************视频信号场行中断初始化***********************************************  	  *
//*****************************************************************************************************************
void initEMIOS_0Image(void) 
{ 
	//A3场中断捕捉上升沿及下降沿
	EMIOS_0.CH[3].CCR.B.MODE = 0x02; // Mode is SAIC, continuous 
	EMIOS_0.CH[3].CCR.B.BSL = 0x01; /* Use counter bus B (default) */
	EMIOS_0.CH[3].CCR.B.EDSEL = 1;  //Both edges
//	EMIOS_0.CH[3].CCR.B.EDPOL=1; //Edge Select falling edge
//	EMIOS_0.CH[3].CCR.B.FEN=1;  //interupt enbale
	SIU.PCR[3].R = 0x0102;  // Initialize pad for eMIOS channel Initialize pad for input 
	INTC_InstallINTCInterruptHandler(FieldInputCapture,142,1);  
	
	//A7行中断捕捉上升沿
	EMIOS_0.CH[7].CCR.B.MODE = 0x02; // Mode is SAIC, continuous 
	EMIOS_0.CH[7].CCR.B.BSL = 0x01; /* Use counter bus B (default) */
	EMIOS_0.CH[7].CCR.B.EDPOL=1; //Edge Select rising edge
	//EMIOS_0.CH[7].CCR.B.FEN=1;  //interupt enbale
	SIU.PCR[7].R = 0x0102;  // Initialize pad for eMIOS channel Initialize pad for input 
	INTC_InstallINTCInterruptHandler(RowInputCapture,144,3); 
	
	//C9口二值化入口
	SIU.PCR[41].R = 0x0102;  // C9口二值化入口
}

//*****************************************************************************************************************
//**********************************************速度控制*************************************************    	  *
//*****************************************************************************************************************
void initPIT(void) {//PIT1速度反馈10ms一个控制周期
                           	       // NOTE:  DIVIDER FROM SYSCLK TO PIT ASSUMES DEFAULT DIVIDE BY 1 
  PIT.PITMCR.R = 0x00000001;       // Enable PIT and configure timers to stop in debug mode 
  PIT.CH[1].LDVAL.R = 800000;      // PIT1 timeout = 80000 sysclks x 1sec/80M sysclks = 1 msec 
  PIT.CH[1].TCTRL.R = 0x000000003; // Enable PIT1 interrupt and make PIT active to count 
  INTC_InstallINTCInterruptHandler(PitISR,60,2); 
}

void initEMIOS_0ModulusCounter(void) //PD12模数计数器入口，上升沿//PD11光编正反转
{
//	EMIOS_0.CH[24].CCR.B.MODE = 0x51; // Mode is MCB, 
	EMIOS_0.CH[24].CCR.B.MODE = 0x13; // Mode is MCB, 
	EMIOS_0.CH[24].CCR.B.BSL = 0x3; // Use internal counter
	EMIOS_0.CH[24].CCR.B.UCPRE=0; // Set channel prescaler to divide by 1
	EMIOS_0.CH[24].CCR.B.UCPEN = 1; // Enable prescaler; uses default divide by 1 
	//EMIOS_0.CH[24].CCR.B.FREN = 0;// Freeze channel counting when in debug mode 
	EMIOS_0.CH[24].CCR.B.EDPOL=1; //Edge Select rising edge
	EMIOS_0.CH[24].CADR.R=0xffff;

	SIU.PCR[60].R = 0x0102;  // Initialize pad for eMIOS channel Initialize pad for input

	SIU.PCR[59].R = 0x0102;  //PD11光编正反转
}


//*****************************************************************************************************************
//************************************************A/D模块************************************************    	  *
//*****************************************************************************************************************
void initADC(void)
{
	ADC.MCR.R = 0x00000100;          /* Conversion times for 80MHz ADClock */
	ADC.NCMR[1].R = 0x00000001;     /* Select ANS0 inputs for conversion */
	ADC.CTR[1].R = 0x00008606;      /* Conversion times for 32MHz ADClock */
	//ADC.MCR.B.NSTART=1;             /* Trigger normal conversions for ADC0 */
	SIU.PCR[24].R = 0x2100;         /* MPC56xxB: Initialize PB[8] as ANS0 */
}


//*****************************************************************************************************************
//************************************************调试模块************************************************    	  *
//*****************************************************************************************************************

/*************************蓝牙初始化***********************/
void initLINFlex_0_UART (void) 
{
	LINFLEX_1.LINCR1.B.INIT=1;  //进入初始化模式
	LINFLEX_1.LINCR1.R=0x00000011; 
	LINFLEX_1.LINIER.B.DRIE=1; //允许接收中断
	/*波特率算法baud=Fperiph_clk/(16*LFDIV)
	DIV_M=LFDIV整数部分
	DIV_F=LFDIV小数部分*16  */ 	
	LINFLEX_1.LINIBRR.B.DIV_M= 520;  	//波特率设置38400:80M-130+3 57600:80M-86+13 115200:80M-43+6  9600:80M-520+83
    	LINFLEX_1.LINFBRR.B.DIV_F = 83;		//38400:64M-104+3
    	LINFLEX_1.UARTCR.B.UART=1;
	LINFLEX_1.UARTCR.R=0x00000033;//8-bit data、UART mode
	LINFLEX_1.LINCR1.B.INIT=0; //退出初始化模式
	
	SIU.PCR[38].R = 0x0400;    /* MPC56xxB: Configure port C6 as LIN1TX */
   	 SIU.PCR[39].R = 0x0103;    /* MPC56xxB: Configure port C7 as LIN1RX */
  	INTC_InstallINTCInterruptHandler(LINFlex_RX_Interrupt,99,2); 
}
/*************************蓝牙接口函数**********************/
void LINFlex_TX(unsigned char data)
{
	LINFLEX_1.BDRL.B.DATA0=data;       //发送语句
	while(!LINFLEX_1.UARTSR.B.DTF){}
	LINFLEX_1.UARTSR.B.DTF=1;
}

void LINFlex_RX_Interrupt(void)
{
	while(!LINFLEX_1.UARTSR.B.DRF){}
	/*
	if(LINFLEX_1.BDRM.B.DATA4==0x55)
	{
		for(;;){SET_motor(0);}
	}
	*/
	LINFlex_TX(0x33);
	LINFLEX_1.UARTSR.B.DRF=1;      //清空标志位
}



/*********************显示屏初始化************************/
void initNokia5110(void)
{
	SIU.PCR[17].R = 0x0200;  
	SIU.PCR[42].R = 0x0200;  
	SIU.PCR[75].R = 0x0200;  
	SIU.PCR[74].R = 0x0200;  
	SIU.PCR[72].R = 0x0200;  
}
/*********************拨码开关、按键初始化************************/
void initKeys_Switchs(void)
{
	SIU.PCR[46].R = 0x0102;//C14_key1
	SIU.PCR[2].R = 0x0102; //A2_key2
	SIU.PCR[1].R = 0x0102;//A1_key3
	SIU.PCR[64].R = 0x0102;//E0_key4
	
	SIU.PCR[56].R = 0x0102;//D8_switch1
	SIU.PCR[54].R = 0x0102;//D6_switch2
	SIU.PCR[52].R = 0x0102;//D4_switch3
	SIU.PCR[50].R = 0x0102;//D2_switch4
}
/*********************LED、测试IO初始化************************/
void initTestIO(void)
{
	SIU.PCR[12].R = 0x0200;//A12_D0
	SIU.PCR[13].R = 0x0200;//A13_D1
	SIU.PCR[14].R = 0x0200;//A14_D2
	SIU.PCR[15].R = 0x0200;//A15_D3
	D0=1;
	D1=1;
	D2=1;
	D3=1;
	
	SIU.PCR[11].R = 0x0200;//A11测试
}
