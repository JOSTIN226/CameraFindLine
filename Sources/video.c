#include "includes.h"
//*************************************************************************
//*		 *************************采样变量*****************************   *
//*************************************************************************
//采样图像标号
word crow = 0;           //行数计数器
byte prow = 0;  	  	    //摄像头采样当前行
byte pcolumn = 0;        //摄像头当前列
//采样结束标志
byte fieldover = 0;                //一场采样结束标志符 
//采样二维数组    
volatile byte g_pix[ROWS][COLUMNS]; 
byte pix[ROWS][COLUMNS];

word selectRows[ROWS]=
{
//最小行:23
//最大行:257
//图像采样从近往远
23 ,39 ,52 ,65 ,76 ,89 ,96 ,105,113,121,
128,134,140,146,151,156,161,165,169,173,
177,180,183,186,189,192,195,198,200,202,
204,206,208,210,212,214,216,218,220,222,
224,225,226,227,228,229,230,231,232,233,
234,235,236,237,238,239,240,241,242,243,
244,245,246,247,248,249,250,251,252,253,
};

//*************************************************************************
//*   **************************中断函数****************************      *
//*************************************************************************
void FieldInputCapture(void) 
{	
    EMIOS_0.CH[3].CCR.B.FEN=0;  //关闭场中断 

	prow=0;crow=0;
	EMIOS_0.CH[7].CCR.B.FEN=1;//打开行中断
}

void RowInputCapture(void) 
{	
	EMIOS_0.CH[7].CSR.B.FLAG = 1;
	++crow;
	//采样开始
	//行消影 因为摄像头输出信号的前几行是不能用的，所以扔掉前ROWS_MIN行
	if(crow>selectRows[prow])
  	{
	    unsigned char i;
	    for(pcolumn=0;pcolumn<RowSAIC_DelayTime;++pcolumn) {}
	    /************************
	    	延时消隐区
	    ***********************************/ 
	   
	   	g_pix[ROW-prow][82]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][81]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][80]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][79]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][78]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][77]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][76]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][75]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][74]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][73]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][72]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][71]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][70]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	    																																																			
	   	g_pix[ROW-prow][69]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][68]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][67]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][66]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][65]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][64]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][63]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][62]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][61]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][60]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   																																																			 
	   	g_pix[ROW-prow][59]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][58]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][57]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][56]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][55]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][54]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][53]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][52]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][51]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][50]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");

	   	g_pix[ROW-prow][49]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][48]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][47]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][46]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][45]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][44]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][43]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][42]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][41]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][40]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	    	   		   		   		   		
	   	g_pix[ROW-prow][39]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][38]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][37]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][36]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][35]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][34]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][33]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][32]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][31]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][30]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	
	   	g_pix[ROW-prow][29]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][28]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][27]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][26]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][25]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][24]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][23]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][22]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][21]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][20]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   		   		   		   	
	   	g_pix[ROW-prow][19]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][18]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][17]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][16]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][15]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][14]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][13]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][12]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][11]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][10]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	
	   	g_pix[ROW-prow][9]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][8]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][7]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][6]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][5]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][4]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][3]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][2]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][1]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
	   	g_pix[ROW-prow][0]=SIU.GPDI[41].R; asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");	
	    ++prow;
		if(prow==ROWS)
		{
			EMIOS_0.CH[7].CCR.B.FEN=0; 	//关闭行中断
			fieldover=1; 
		}
	}
}

//*************************************************************************																							                        
//*   ********************无线发送图像******************************      *	
//*************************************************************************
void Send_CCD_Video(void) 
{
  unsigned char i,j;
  LINFlex_TX(0x55);
  LINFlex_TX(0xAA);
  LINFlex_TX(0xFF);
  for(i = 0;i < ROWS;i ++)
  {   
   for(j = 0;j < COLUMNS;j ++)
    LINFlex_TX(g_pix[i][j]*255);
  }
  LINFlex_TX(0xFF);
  LINFlex_TX(0xAA);
  LINFlex_TX(0x55);
}

void SetupCCD(void)
{   
  LINFlex_TX(0xAA);        
  LINFlex_TX(0xBB);        
  LINFlex_TX(0xFF);        
  LINFlex_TX(ROWS);        
  LINFlex_TX(COLUMNS);         
  LINFlex_TX(0xFF);        
  LINFlex_TX(0xBB);        
  LINFlex_TX(0xAA);        
}

void SetupBKL(void)
{
    unsigned char para1[10] = {"L"};//设置显示在上位机上的变量名
    unsigned char para2[10] = {"R"};  
    unsigned char para3[10] = {"target_a"};
    byte i;
       
    LINFlex_TX(0xCC);        
    LINFlex_TX(0xDD);        
    LINFlex_TX(0xFF);         
                                             
    LINFlex_TX(ROWS);        
    LINFlex_TX(COLUMNS);        
    LINFlex_TX(3);         // number of parameters
    
    LINFlex_TX(0);         // display position  0和1分别表示在左边窗口或者右边窗口显示
    LINFlex_TX(0);         // display position
    LINFlex_TX(1);
    
    for(i=0;i<10;++i)
    	LINFlex_TX(para1[i]);
    for(i=0;i<10;++i)
    	LINFlex_TX(para2[i]);
   	for(i=0;i<10;++i)
    	LINFlex_TX(para3[i]);
    	        
    LINFlex_TX(9);         // parameter color
    LINFlex_TX(5);
    LINFlex_TX(9);         // parameter color
    
    LINFlex_TX(0xFF);        
    LINFlex_TX(0xDD);        
    LINFlex_TX(0xCC);  
}

void Send_CCD_Blackline(void)
{
	byte i=0;
    LINFlex_TX(0x55);
    LINFlex_TX(0xBB);
    LINFlex_TX(0xFF);
    for(;i<ROWS;++i)
    	LINFlex_TX(BlackLine[0][i]);
    LINFlex_TX(0x00);
    LINFlex_TX(0x00);
    LINFlex_TX(0x00);
    LINFlex_TX(LineType[0]);//
    LINFlex_TX(0x00);
    LINFlex_TX(0x00);
    LINFlex_TX(0x00);
    LINFlex_TX(LineType[1]);//
    LINFlex_TX(0x00);
    LINFlex_TX(0x00);
    LINFlex_TX(0x00);
    LINFlex_TX(target_offset);//
    LINFlex_TX(0xFF);        
    LINFlex_TX(0xBB);        
    LINFlex_TX(0x55);              
}

