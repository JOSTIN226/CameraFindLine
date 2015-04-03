#include "includes.h"
//找线变量定义
byte BlackLine[2][ROWS]; //左右线的数组
byte StartRow[2];        //线的起点
byte EndRow[2];          //线的终点
byte BreakRow[2];        //线的中断点
byte ContinueRow[2];     //线的续接点
byte SegNum[2];			 //黑线的段数
byte TurnRow[2];         //拐角点位置
byte LineType[2]={0,0};  //找到的线的类型
signed char CenterLine[ROWS]; //找到的中线数组
signed char dif[70];		  //二次差分量
//赛道类型有关量
byte RoadType=UnBegin;		//赛道类型
byte RoadStart=ROWS;		//赛道起始行
byte RoadEnd=0;		//赛道终止行
byte RoadTurn;		//赛道拐点
byte PastType[10];
byte MostL[3];
byte MostR[3];
byte left_d[3],right_d[3],mid_d[3];//0:左线	1:右线	2:中线部分

byte turn[10],count;
byte TurnPoint=0;
//虚线类型识别
unsigned int blackcount=0;
byte DottedLineFlags=0;
byte DotRow=0,DotCol=0;
byte DotUseful;
byte Line[70];
//十字类型判断
byte NearCross[2];
byte FarCross[2];
byte CrossFlags;
//起始线识别参数
byte StartLine=0;//起始线标志(1:表示检测到起始线)
byte startline_fnum=0;
byte startline_unfnum=0;
byte startline_delay=0;
byte startcount=0;
byte StartFlags=0;
byte StartList[15]=
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0	};
unsigned char StartDelay=0;
//坡道识别参数
byte up_fnum=0;
byte up_unfnum=0;
byte down_fnum=0;
byte down_unfnum=0;
byte slope_flags=0;
byte UpSlope=0;//上坡标志
byte DownSlope=0;//下坡标志
byte slope_delay=0;
byte detect_slope_delay=0;
byte detected=0;
byte Slope=0;	//0:正常	1:上坡和坡上	2:下坡
byte PastUpSlope=0;
byte PPastUpSlope=0;
byte PPPastUpSlope=0;
byte PastDownSlope=0;
byte PPastDownSlope=0;
byte Offset[ROWS]=	//畸变修复偏移量
{
51,51,51,52,52,52,52,52,53,53,
53,53,54,54,54,54,54,55,55,55,
55,55,56,56,56,56,56,57,57,57,
57,58,58,59,59,59,60,60,60,61,
61,61,62,62,62,63,63,64,64,65,
65,66,66,67,67,68,68,69,70,71,
72,73,74,75,76,77,78,79,80,81,
};
//*****************************************************************************************************************
//*	     *************************有效行权值分配******************************************************* 	      *
//*****************************************************************************************************************
byte StraightWeight[ROWS]=		//直道偏移量权值
{//远
10,10,10,10,10,9,9,9,9,9,//1
8,8,8,8,8,7,7,7,7,7,//2
6,6,6,6,6,5,5,5,5,5,//3
4,4,4,4,4,4,4,4,4,4,//4
3,3,3,3,3,3,3,3,3,3,//5
2,2,2,2,2,2,2,2,2,2,//6
1,1,1,1,1,1,1,1,1,1,//7
};//近
byte UTurnWeight[ROWS]=		//偏移量权值
{
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//0-9
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//10-19
92,92,92,91,91,90,90,89,89,88,//20-29
87,86,85,83,81,79,77,74,71,67,//30-39
63,58,53,47,42,38,34,30,27,24,//40-49
21,18,16,14,12,10, 9, 8, 7, 6,//50-59
 5 ,4 ,4 ,3 ,3 ,2 ,2 ,1 ,1 ,1  //60-69
};
byte ToTWeightMode0[ROWS]=//15-29
{//远
215,214,213,212,211,210,209,208,207,206,//0-15无用
204,202,200,198,196,194,192,190,188,185,//15-19
182,179,176,173,169,165,161,157,153,148,//20-29
143,138,133,128,122,116,109,101, 94, 87,//30-39
 81, 75, 69, 64, 59, 54, 49, 45, 41, 37,//40-49	
 33, 29, 26, 23, 20, 17, 14, 12, 10,  8,//50-59
  6,  5,  4,  3,  3,  2,  2,  1,  1,  1//60-69
};//近
byte ToTWeightMode1[ROWS]=//15-29
{//远
160,160,159,158,157,156,154,153,152,151,//0-15无用
150,149,148,147,146,145,143,141,139,137,//15-19
135,133,131,129,127,125,123,120,107,104,//20-29
101, 98, 95, 92, 88, 84, 80, 75, 70, 64,//30-39
 59, 54, 50, 46, 42, 39, 36, 33, 30, 27,//40-49	
 24, 22, 20, 18, 16, 14, 12, 10,  8,  7,//50-59
  6,  5,  4,  3,  3,  2,  2,  1,  1,  1//60-69
};//近
byte ToTWeight[ROWS]=//15-29
{//远
100,99,99,98,98,97,96,95,94,93,//0-15无用
92,91,90,89,88,87,86,85,83,81,//15-19
79,77,75,72,69,66,62,58,54,50,//20-29
47,44,41,38,35,33,31,29,27,25,//30-39
23,21,20,19,18,17,16,15,14,13,//40-49	
12, 1, 10, 9, 8, 7, 6, 6, 5, 5,//50-59
4, 4, 3, 3, 2, 2, 2, 1, 1, 1  //60-69
};//近
byte FarWeightMode0[ROWS]=//0-14
{
1 ,2 ,3 ,4 ,4 ,5 ,5 ,6 ,6 ,7 ,//0-9
7 ,8 ,8 ,9 ,9 ,10,10,11,12,14,//10-19
16,18,20,22,24,27,30,33,37,40,//20-29
43,42,41,40,39,37,35,33,30,27,//30-39
24,22,19,16,14,13,12,11,10,9,//40-49
8, 8, 7 ,7 ,6 ,6 ,5 ,5 ,4 ,4 ,//50-59
3 ,3 ,3 ,2 ,2 ,2 ,1 ,1 ,1 ,1  //60-69
};
byte FarWeight[ROWS]=//0-14
{
1 ,2 ,3 ,4 ,4 ,5 ,5 ,6 ,6 ,7 ,//0-9
7 ,8 ,8 ,9 ,9 ,10,10,11,11,12,//10-19
12,13,13,14,14,15,16,17,19,21,//20-29
24,26,28,29,31,33,35,33,30,27,//30-39
24,22,19,16,14,13,12,11,10,9,//40-49
8, 8, 7 ,7 ,6 ,6 ,5 ,5 ,4 ,4 ,//50-59
3 ,3 ,3 ,2 ,2 ,2 ,1 ,1 ,1 ,1  //60-69
};
byte FarWeightTem[ROWS]=//0-14
{
1 ,2 ,3 ,4 ,4 ,5 ,5 ,6 ,6 ,7 ,//0-9
7 ,8 ,8 ,9 ,9 ,10,10,11,11,12,//10-19
12,13,13,14,14,15,16,17,19,21,//20-29
24,26,28,29,30,31,32,30,28,26,//30-39
24,22,19,16,14,13,12,11,10,9,//40-49
8, 8, 7 ,7 ,6 ,6 ,5 ,5 ,4 ,4 ,//50-59
3 ,3 ,3 ,2 ,2 ,2 ,1 ,1 ,1 ,1  //60-69
};
/*byte MidWeight[ROWS]=//15-29
{//远
0,0,0,0,0,0,0,0,0,0,0 ,0 ,0 ,0 ,0,//0-15无用
20,20,20,20,20,               //15-19
25,25,25,25,25,30,30,30,30,30,//20-29
35,35,35,35,35,30,30,30,30,30,//30-39
25,25,25,25,25,20,20,20,20,20,//40-49	
15,15,15,15,15,10,10,10,10,10,//50-59
5 ,5 ,5 ,5 ,5 ,1 ,1 ,1 ,1 ,1  //60-69
};//近*/
byte NearWeightMode0[ROWS]=//30-69
{//远
0 , 0, 0, 0, 0, 0, 0, 0, 0, 0,//0-9
0 , 0, 0, 0, 0, 1, 6,13,19,25,//10-19
32,40,49,57,64,63,62,61,60,59,//20-29
57,55,53,50,47,43,39,36,33,30,//30-39
28,26,24,22,20,18,16,15,14,13,//40-49
12,11,10, 9, 8, 7, 6, 6, 5, 5,//50-59
 4, 4, 3, 3, 2, 2, 2, 1, 1, 1,//60-69
};//近
byte NearWeightMode1[ROWS]=//30-69
{//远
0 , 0, 0, 0, 0, 0, 0, 0, 0, 0,//0-9
0 , 0, 0, 0, 0, 1, 6,13,19,25,//10-19
32,40,49,57,64,63,62,61,60,59,//20-29
57,55,53,50,47,43,39,36,33,30,//30-39
28,26,24,22,20,18,16,15,14,13,//40-49
12,11,10, 9, 8, 7, 6, 6, 5, 5,//50-59
 4, 4, 3, 3, 2, 2, 2, 1, 1, 1,//60-69
};//近
byte NearWeight[ROWS]=//30-69
{//远
0 , 0, 0, 0, 0, 0, 0, 0, 0, 0,//0-9
0 , 0, 0, 0, 0, 1, 2, 5, 8,12,//10-19
17,22,28,33,37,40,38,36,35,34,//20-29
33,32,31,29,27,25,24,23,22,21,//30-39
20,19,18,17,16,15,14,13,12,11,//40-49
10, 9, 8, 8, 7, 7, 6, 6, 5, 5,//50-59
 4, 4, 3, 3, 2, 2, 2, 1, 1, 1,//60-69
};//近
byte SlopeWeight[ROWS]=
{
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//0-9
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//10-19
69,69,69,69,69,69,68,68,68,66,//20-29
64,62,60,58,56,54,52,50,48,46,//30-39
44,42,40,38,20,19,18,17,16,15,//40-49
6,6,6,6,6,5,5,5, 5, 5,//50-59
5 ,4 ,4 ,4 ,3 ,3 ,3 ,1 ,1 ,1  //60-69
};
/*
** ###################################################################
**     CCD 视频寻找黑线算法
**	   输入 g_pix[][]           // 采集的灰度数组
**	   输出 g_black_line[]		// 显示的黑线数组 
**		    BlackLine[][]		//左右黑线
** ###################################################################
*/
void FindBlackLine(void) {
    byte lr=0,i;
	LineType[0]=LineType[1]=0;
	RoadType=UnBegin;
	RoadStart=ROWS;
	RoadEnd=0;
	for(lr=0;lr<2;lr++) {           //找左右线（0：左线，1：右线）
	   	Line_Init(lr);				//初始化左右线变量
		FindLine(lr);				//首次找线
   		Pre_handle(lr);				//判断此次找线的起点和终点
	}
	Analyze_Cross();				//分析是不是十字线，若是：则重新初始化有关信息
	Rec_Cross();					//再次分析十字赛道
	for(lr=0;lr<2;lr++){
		if(LineType[lr]==CrossLine){
			FindCross(lr);			//针对十字，进行二次找线
			FillCross(lr);			//填充十字中间线
		}
	}
	ReBuildCross();					//修复十字赛道
	GetCenterLine();				//获取中线
	SmoothLine(Center);				//中心线平滑
	
	AnalyzeRoadType();				//分析赛道类型

//	GetTurnPoint();					//再次判断赛道类型
	DetectSlope();					//检测坡道
	DetectStartLine();				//检测起始线
//	DetectStart();
//	LINFlex_TX(StartFlags);
	
//	ReBuildWeight();
	TargetOffset();					//目标控制量
}
//*   *********************找线初始化*********************************    *
void Line_Init(byte lr) {
    StartRow[lr]=ROWS;
    BreakRow[lr]=0;
    ContinueRow[lr]=ROWS;
    EndRow[lr]=0;
	TurnRow[lr]=ROWS;
	SegNum[lr]=0;
	MostL[lr]=0;
	MostR[lr]=0;
	BlackLine_Init(lr,ROW,0);
}
void BlackLine_Init(byte lr,byte irowb,byte irowe)
{
	byte irow=irowe;
	for(;irow<=irowb;++irow)
	   BlackLine[lr][irow]=COLUMNS;
}
//*    ************************找线主程序***************************
byte find_num,pre_break,seg_num,pre_flags;//全局变量变成局部变量
void FindLine(byte lr){
    byte flags=0,irow=0,irowb=ROW,irowe=0;
  	find_num=0;	 seg_num=0;	 pre_break=ROWS;	 pre_flags=0;
    for(irow=irowb;irow<ROWS;--irow){		//左上角的坐标为（0，0）
	  flags=NextPoint(lr,irow,pre_flags);
  	  if(flags==0){         		 //没有找到线
  		if(irow<=40)	break;
  	  }
  	  else if(flags==1){      		 //找到一个点
		if(pre_flags==2)	
		{
			if(ABS(BlackLine[lr][irow]-BlackLine[lr][pre_break])>25)
			{
				BlackLine[lr][irow]=COLUMNS;
				flags=2;
			}
			else
			{
		      	++find_num;
				pre_flags=1;			
			}
		}
		else {
	      	++find_num;
			pre_flags=1;
		}
  	  }
 	  if(flags==2){             //之前找到，现在没找到
		  if(pre_flags==1){
			  if(find_num<3)
				BlackLine_Init(lr,irow+find_num,irow+1);
			  else if(find_num<7&&seg_num==0&&irow<45)
				BlackLine_Init(lr,irow+find_num,irow+1);
			  else {
				  if(seg_num>0)	
					  FillLine(lr,pre_break+1,irow-1+find_num);
				  seg_num++;	pre_break=irow+1;
			  }
			  find_num=0;
			  if(seg_num==0)	pre_flags=0;
			  else	pre_flags=2;
		  }
		  else{
			  if(irow>50){		
				  if(pre_break-irow>10+seg_num)	break;			
			  }
			  else if(irow>40){		
				  if(pre_break-irow>8+seg_num)	break;			
			  }
			  else if(irow>30){		
				  if(pre_break-irow>6+seg_num)	break;	
			  }
			  else{
				  if(seg_num>3&&pre_break-irow>seg_num+7)		break;
				  if(seg_num<4&&pre_break-irow>4)		break;
			  }
		  }
  	  }
  	}
	if(irow>ROW&&seg_num>0&&find_num>1)
		FillLine(lr,pre_break+1,irow-1+find_num);
	SegNum[lr]=seg_num>0?seg_num:1;
}
//*    ************************找下一行黑点********************    *
byte offset;						  				 //搜索宽度
byte NextPoint(byte lr,byte irow,byte flags)
{
    byte pre_point,search_point,edge_point,width=3,start_point;			
    signed char	search_direction=1-2*lr;         		 //左右搜索方向（-1：向左，1：向右)
    signed char	compare_direction=search_direction;  //左右比较方向 (-1：向左，1：向右)
  	byte j=0,k=0,l=0;					  				 //循环控制变量
	if(flags==0){             //之前一直没有找到点
  		if(lr==1){  
			if(irow>ROW-5){		offset=COLUMN2_3;	pre_point=COLUMN1_3;}
			else if(irow>=55){	offset=COLUMN1_2;	pre_point=COLUMN1_2+(ROW-irow);	}
			else if(irow>=40){	offset=15;	pre_point=COLUMN-15;	}
			else{	offset=8;	pre_point=COLUMN-8;	}
  		}
		else{
			if(irow>ROW-5){		offset=COLUMN2_3; 	pre_point=COLUMN2_3;		}
			else if(irow>=55){		offset=COLUMN1_2;	pre_point=COLUMN1_2-(ROW-irow);	}
			else if(irow>=40){	offset=15;	pre_point=15;	}
			else{	offset=8;	pre_point=8;	}
		}
		search_direction=2*lr-1;
  	}
	else if(flags==1){		  //前一行找到有效点
		pre_point=BlackLine[lr][irow+1];
		if(find_num==1)
			offset=7;
		else if(irow>=45)	offset=ABS(pre_point-BlackLine[lr][irow+2])+6;
		else if(irow>=30)	offset=ABS(pre_point-BlackLine[lr][irow+2])+4;
		else if(irow>=20)	offset=ABS(pre_point-BlackLine[lr][irow+2])+3;
		else 
		{
			offset=ABS(pre_point-BlackLine[lr][irow+2])+2;
			if(offset>3)	offset=3;
			if((lr==0&&BlackLine[lr][irow+2]<BlackLine[lr][irow+1])||
				(lr==1&&BlackLine[lr][irow+2]>BlackLine[lr][irow+1]))
				offset+=2;
		}
	}
	else{					//前一行没找到有效点，但之前找到过（针对白色底色中虚线）
		start_point=pre_break;
		while(BlackLine[lr][start_point]!=COLUMNS&&start_point<ROWS)
			start_point++;
		start_point=start_point-2;
		pre_point=BlackLine[lr][start_point]-(BlackLine[lr][start_point]-BlackLine[lr][pre_break])*(start_point-irow)/(start_point-pre_break);
		if(irow>45){	if(offset<8)	offset+=2;	else offset=8;}
		else if(irow>30){	if(offset<6)	offset+=1;	else offset=6;}
		else if(irow>20){	if(offset<3)	offset+=1;	else offset=3;}
		else	offset=1;
	}
	offset+=seg_num;
	search_point=pre_point-search_direction;
	edge_point=search_point+compare_direction*width;//
  	for(j=0;j<2;++j){		//j表示左右两个找线方向                             
  		 for(k=0;k<=offset;++k){	//搜索偏移量											
  			search_point+=search_direction;  //计算当前搜索点位置
			edge_point+=search_direction;
  			if(edge_point>COLUMN||search_point>COLUMN)
  		  		break;
			if(g_pix[irow][search_point])
				continue;
    		for(l=1;l<=width;++l) 			
    		    if(!g_pix[irow][search_point+l*compare_direction])       
    				break;
			if(l>width){ 
  	  			BlackLine[lr][irow]=search_point;
    				return 1;
			}
 		 }
		if(flags==0)	break;
 		search_direction=-search_direction;     
		search_point=pre_point;
		edge_point=search_point+compare_direction*width;
	}
  	BlackLine[lr][irow]=COLUMNS;
	if(flags==0)	return 0;
	else       	  	return 2;
}
void FillLine(byte lr,byte start,byte end)
{
	byte deltar=start-end,irow;
	signed char deltac=BlackLine[lr][start]-BlackLine[lr][end];
	for(irow=end+1;irow<start;irow++)
		BlackLine[lr][irow]=BlackLine[lr][end]+deltac*(irow-end)/deltar;
}
void Pre_handle(byte lr)
{
	byte irowb=ROW;		//起点
	byte irowe=0;		//终点
	for(;irowe<irowb;++irowe)			//寻找终点
		if(BlackLine[lr][irowe]!=COLUMNS)
			break;
	for(;irowb>irowe;--irowb)				//寻找起点
		if(BlackLine[lr][irowb]!=COLUMNS)
			break;
	if(irowb>irowe){					//首次找线
		StartRow[lr]=irowb;
		EndRow[lr]=irowe;
	}
}
void RebuildLine()
{
	byte irow,irowb,irowe;
	if(StartRow[0]<EndRow[1]+5)
	{
		if(ABS(BlackLine[0][StartRow[0]]-BlackLine[1][EndRow[1]])<5)
		{
			for(irow=EndRow[0];irow<=StartRow[0];irow++)
				BlackLine[0][irow]=COLUMNS;
			StartRow[0]=ROWS;
			EndRow[0]=0;
		}

	}
	if(StartRow[1]<EndRow[0]+5)
	{
		if(ABS(BlackLine[0][EndRow[0]]-BlackLine[1][StartRow[1]]))
		{
			for(irow=EndRow[1];irow<=StartRow[1];irow++)
				BlackLine[1][irow]=COLUMNS;
			StartRow[1]=ROWS;
			EndRow[1]=0;
		}
	}
}
void Analyze_Cross()
{
	byte i,irow,irowb,irowe,lr,dis,dis2,num=0,flags=0,maxr,maxc;
	if(EndRow[0]==0&&EndRow[1]==0&&StartRow[0]!=ROWS&&StartRow[1]!=ROWS&&ABS(StartRow[0]-StartRow[1])<25)	return;		//针对虚线，长直道
	for(lr=0;lr<2;lr++)
	{
		if(StartRow[lr]==ROWS)	continue;
		if(SegNum[lr]>2)		continue;
		for(irow=0;irow<ROW;irow++)
			dif[irow]=0;
		for(irow=StartRow[lr];irow>EndRow[lr];irow--)
			dif[irow]=BlackLine[lr][irow-1]-BlackLine[lr][irow];
		for(irow=EndRow[lr]+1;irow<StartRow[lr];irow++)
			dif[irow]=ABS(dif[irow]-dif[irow+1]);
		dif[StartRow[lr]]=0;
		irowb=StartRow[lr]-2;irowe=EndRow[lr]+2;
		maxc=lr*COLUMN;
		for(irow=irowb;irow>=irowe;irow--)
		{
			if(irow<15)	break;
			if((BlackLine[lr][irow]>maxc&&lr==0)||(BlackLine[lr][irow]<maxc&&lr==1)){
				maxc=BlackLine[lr][irow];
				maxr=irow;
			}
			if(dif[irow]>3&&(BlackLine[lr][irow]-BlackLine[lr][irow-2])*(BlackLine[lr][irow]-BlackLine[lr][irow+2])>0){
				LineType[lr]=CrossLine;
				TurnRow[lr]=irow;flags=1;num++;
			}
			else if(dif[irow]>3&&(BlackLine[lr][irow]-BlackLine[lr][irow-2])*(BlackLine[lr][irow]-BlackLine[lr][irow+2])>0){
				if(num==0&&(BlackLine[lr][irow-1]-BlackLine[lr][irow-2])*(BlackLine[lr][irow+1]-BlackLine[lr][irow+2])>0){
					LineType[lr]=CrossLine;
					TurnRow[lr]=irow;	flags=1;
				}
				num++;
			}
			else if(dif[irow]>=2&&flags==0){
				for(i=0;i<2;i++)
					if((!lr&&!(BlackLine[lr][irow-i]>=BlackLine[lr][irow-i-1]&&BlackLine[lr][irow+i]>=BlackLine[lr][irow+i+1]))
					||(lr&&!(BlackLine[lr][irow-i]<=BlackLine[lr][irow-i-1]&&BlackLine[lr][irow+i]<=BlackLine[lr][irow+i+1])))
					break;
				if(i==2&&!(BlackLine[lr][irow]==BlackLine[lr][irow-2]||BlackLine[lr][irow]==BlackLine[lr][irow+2])){
					dis=MIN(3,irow-EndRow[lr]);		dis=MIN(dis,StartRow[lr]-irow);
					dis2=MIN(5,irow-EndRow[lr]);		dis2=MIN(dis2,StartRow[lr]-irow);
					if(lr&&BlackLine[lr][irow-dis]+BlackLine[lr][irow+dis]-2*BlackLine[lr][irow]>5
						&&BlackLine[lr][irow-dis2]+BlackLine[lr][irow+dis2]-2*BlackLine[lr][irow]>7){
						LineType[lr]=CrossLine;
						TurnRow[lr]=irow;
						flags=1;
					}
					else if(!lr&&2*BlackLine[lr][irow]-BlackLine[lr][irow-dis]-BlackLine[lr][irow+dis]>5
						&&2*BlackLine[lr][irow]-BlackLine[lr][irow-dis2]-BlackLine[lr][irow+dis2]>7){
						LineType[lr]=CrossLine;
						TurnRow[lr]=irow;
						flags=1;
					}
				}
			}
			else if(dif[irow]==1&&dif[irow-1]==1&&flags==0){
				if(lr&&!(BlackLine[lr][irow]<BlackLine[lr][irow+1]&&BlackLine[lr][irow-1]<BlackLine[lr][irow-2]))	continue;
				else if(!lr&&!(BlackLine[lr][irow]>BlackLine[lr][irow+1]&&BlackLine[lr][irow-1]>BlackLine[lr][irow-2]))	continue;
				dis=MIN(3,irow-1-EndRow[lr]);		dis=MIN(dis,StartRow[lr]-irow);
				dis2=MIN(5,irow-1-EndRow[lr]);		dis2=MIN(dis2,StartRow[lr]-irow);
				if(lr&&BlackLine[lr][irow-dis-1]+BlackLine[lr][irow+dis]-BlackLine[lr][irow-1]-BlackLine[lr][irow]>5
					&&BlackLine[lr][irow-dis2-1]+BlackLine[lr][irow+dis2]-BlackLine[lr][irow-1]-BlackLine[lr][irow]>7){
					LineType[lr]=CrossLine;
					TurnRow[lr]=irow;
					flags=1;
				}
				else if(!lr&&BlackLine[lr][irow]+BlackLine[lr][irow-1]-BlackLine[lr][irow-dis-1]-BlackLine[lr][irow+dis]>5
					&&BlackLine[lr][irow]+BlackLine[lr][irow-1]-BlackLine[lr][irow-dis2-1]-BlackLine[lr][irow+dis2]>5){
					LineType[lr]=CrossLine;
					TurnRow[lr]=irow;
					flags=1;
				}
			}
		}
		if(num>1){				//对应黑色赛道下的虚线小S弯
			RoadType=SmallS;
			LineType[lr]=UnBegin;
			TurnRow[lr]=ROWS;
		}
		if(LineType[lr]==CrossLine&&TurnRow[lr]!=ROWS)
		{
			BlackLine_Init(lr,TurnRow[lr]-1,EndRow[lr]);
			EndRow[lr]=TurnRow[lr];
		}
		if(TurnRow[lr]==ROWS&&EndRow[lr]>25&&EndRow[lr]<ROW-4&&(ABS(EndRow[0]-EndRow[1])<18||TurnRow[1-lr]!=ROWS)){		//
			if(maxr-EndRow[lr]<5||ABS(BlackLine[lr][maxr]-BlackLine[lr][EndRow[lr]])<4){
				if((lr==0&&BlackLine[lr][EndRow[lr]]<40&&BlackLine[lr][EndRow[lr]]>5)
				||(lr==1&&BlackLine[lr][EndRow[lr]]<78&&BlackLine[lr][EndRow[lr]]>35))
				LineType[lr]=CrossLine;}
			else{
				if((lr==0&&BlackLine[lr][EndRow[lr]]<50&&BlackLine[lr][EndRow[lr]]>10)
				||(lr==1&&BlackLine[lr][EndRow[lr]]<73&&BlackLine[lr][EndRow[lr]]>30))
				LineType[lr]=CrossLine;TurnRow[lr]=maxr;}
		}
	}
	if(StartRow[1]<5+EndRow[0]&&BlackLine[0][EndRow[0]]>BlackLine[1][EndRow[1]]+10)//十字找线出错
		Line_Init(1);
	if(StartRow[0]<5+EndRow[1]&&BlackLine[0][EndRow[0]]>BlackLine[1][EndRow[1]]+10)
		Line_Init(0);
	if(LineType[0]==CrossLine&&LineType[1]==UnBegin)
	{
		if(BlackLine[1][StartRow[1]]-BlackLine[1][EndRow[1]]>30&&StartRow[1]-EndRow[1]<35){
			LineType[0]=UnBegin;TurnRow[0]=ROWS;}
	}
	if(LineType[1]==CrossLine&&LineType[0]==UnBegin)
	{
		if(BlackLine[0][EndRow[0]]-BlackLine[0][StartRow[0]]>30&&StartRow[0]-EndRow[0]<35){
			LineType[1]=UnBegin;TurnRow[1]=ROWS;}
	}
}
void Rec_Cross()
{
	byte irow,irowb,irowe,countnum=0,turnrow=0;
	if(StartRow[0]<=EndRow[1]||StartRow[1]<=EndRow[1])return;
	if(LineType[0]!=CrossLine&&LineType[1]!=CrossLine){
		if(EndRow[0]<EndRow[1]-1&&EndRow[0]>15){//左边线远
			irowb=EndRow[1]-1;
			irowe=EndRow[0]+1;
			for(irow=irowb-3;irow>irowe+2&&irow<ROWS;irow--)
			{
				if(BlackLine[0][irow+3]+1<BlackLine[0][irow]&&
					BlackLine[0][irow+2]<BlackLine[0][irow]&&
					BlackLine[0][irow+1]<=BlackLine[0][irow]&&
					BlackLine[0][irow-3]+1<BlackLine[0][irow]&&
					BlackLine[0][irow-2]<BlackLine[0][irow]&&
					BlackLine[0][irow-1]<=BlackLine[0][irow]
					)
				{
					turnrow=irow;
					TurnRow[0]=irow;
					LineType[0]=CrossLine;
					BlackLine_Init(0,TurnRow[0],EndRow[0]);
					EndRow[0]=TurnRow[0]+1;
					break;
				}
			}
		}
		else if(EndRow[1]<EndRow[0]-1&&EndRow[1]>15){
			irowb=EndRow[0]-1;
			irowe=EndRow[1]+1;
			for(irow=irowb-3;irow>irowe+2&&irow<ROWS;irow--)
			{
				if(BlackLine[1][irow+3]+1>BlackLine[1][irow]&&
					BlackLine[1][irow+2]>BlackLine[1][irow]&&
					BlackLine[1][irow+1]>=BlackLine[1][irow]&&
					BlackLine[1][irow-3]+1>BlackLine[1][irow]&&
					BlackLine[1][irow-2]>BlackLine[1][irow]&&
					BlackLine[1][irow-1]>=BlackLine[1][irow]
					)
				{
					turnrow=irow;
					TurnRow[1]=irow;
					LineType[1]=CrossLine;
					BlackLine_Init(1,TurnRow[1]-1,EndRow[1]);
					EndRow[1]=TurnRow[1];
					break;
				}
			}
		}
	}
	if(LineType[0]==CrossLine){	//左线判断为十字
		if(LineType[1]==CrossLine){
		}//右线也判断为十字
		else{					//右线判断不是十字
			if(BlackLine[0][EndRow[0]]-BlackLine[0][(EndRow[0]+StartRow[0])/2]>1)
			{
				for(irow=StartRow[1]-1;irow>EndRow[1]+1;irow--){
					if(BlackLine[1][irow]<BlackLine[1][irow-1])
						break;
				}
				if(irow==EndRow[1]+1&&
					BlackLine[1][(EndRow[1]+StartRow[1])/2]<BlackLine[1][StartRow[1]]
					&&BlackLine[1][EndRow[1]]<BlackLine[1][(EndRow[1]+StartRow[1])/2])
					LineType[1]=CrossLine;
			}
			if(LineType[1]!=CrossLine){
				countnum=0;
				for(irow=EndRow[1]+1;irow<StartRow[1];irow++){
					if(ABS(BlackLine[1][EndRow[1]+1]-BlackLine[1][irow])<2)
						countnum++;
					else
						break;
				}
				if(countnum>10)
					LineType[1]=CrossLine;
			}
		}
	}
	else if(LineType[1]==CrossLine){
		if(LineType[0]==CrossLine){
		}//左线也判断为十字
		else{					//左线判断不是十字
			if(BlackLine[1][(EndRow[1]+StartRow[1])/2]-BlackLine[1][EndRow[1]]>1)
			{
				for(irow=StartRow[0]-1;irow>EndRow[0]+1;irow--){
					if(BlackLine[0][irow]>BlackLine[0][irow-1])
						break;
				}
				if(irow==EndRow[0]+1&&
					BlackLine[0][(EndRow[0]+StartRow[0])/2]>BlackLine[0][StartRow[0]]
					&&BlackLine[0][EndRow[0]]>BlackLine[0][(EndRow[0]+StartRow[0])/2])
					LineType[0]=CrossLine;
			}
			if(LineType[0]!=CrossLine){
				countnum=0;
				for(irow=EndRow[0]+1;irow<StartRow[0];irow++){
					if(ABS(BlackLine[0][EndRow[0]+1]-BlackLine[0][irow])<2)
						countnum++;
					else
						break;
				}
				if(countnum>10)
					LineType[0]=CrossLine;
			}
		}
	}
	else{
		if(EndRow[0]<10&&EndRow[1]<10&&ABS(EndRow[1]-EndRow[0])<3){
			irowb=MIN(StartRow[0],StartRow[1]);
			irowe=MAX(EndRow[0],EndRow[1]);
			for(irow=irowb;irow>irowe;irow--){
				if(BlackLine[1][irow-1]-BlackLine[0][irow-1]>=
					BlackLine[1][irow]-BlackLine[0][irow]-1)
					break;
			}
			if(irow==irowe)	{
				LineType[0]=CrossLine;LineType[1]=CrossLine;
			}
		}
	}
}
void FindCross(byte lr)
{
  	byte irowb=EndRow[lr]-CrsLen,irowe=0,irow=0,pre_point,flags=0,offset=10,num=0,pre_break=0,midrow;
	byte sflags=0;
	byte start_point=0;
  	midrow=(StartRow[lr]+EndRow[lr]+1)/2;
	pre_point=BlackLine[lr][EndRow[lr]+1]-(CrsLen+1)*
		(BlackLine[lr][midrow]-BlackLine[lr][EndRow[lr]])/(midrow-EndRow[lr]);
	if(TurnRow[lr]==ROWS){
		irowb-=3;offset-=5;pre_point+=5*(2*lr-1);}
    for(irow=irowb;irow<ROWS;--irow){		//左上角的坐标为（0，0）
	  flags=CrossPoint(lr,irow,pre_point,flags,offset);
	  if(flags==0){
	 	pre_point=BlackLine[lr][EndRow[lr]+1]-(EndRow[lr]+1-irow)*
			(BlackLine[lr][midrow]-BlackLine[lr][EndRow[lr]])/(midrow-EndRow[lr]);
		num++;
		if(num==6)	offset+=5;
		if(num>=6)	pre_point+=5*(2*lr-1);
		if(num==18||irow<10)
			break;
	  }
	  else if(flags==1){
		if(sflags==0)
		{
			if(ABS(BlackLine[lr][irow]-BlackLine[lr][EndRow[lr]+1])>25)
			{
				BlackLine[lr][irow]=COLUMNS;
				flags=2;
			}
			else
			{
		  		pre_point=BlackLine[lr][irow];
				if(irow>20)	offset=5;
				else	offset=2;
				num=0;
				pre_break=0;
				sflags=1;
			}
		}
		else
		{
		  	pre_point=BlackLine[lr][irow];
			if(irow>20)	offset=5;
			else	offset=2;
			num=0;
			pre_break=0;
		}
	  }
	  if(flags==2){
		  if(pre_break==0){	
			  pre_break=irow+1;
		  	  start_point=pre_break;
	    	  while(BlackLine[lr][start_point]!=COLUMNS&&start_point<ROWS)
				start_point++;
			  start_point=start_point-1;
			  if(start_point-pre_break<2)
			  {
				  BlackLine_Init(lr,start_point,pre_break);
				  pre_break=0;
			  }
		  }
		  if(start_point-pre_point>=2)
		  {
			  pre_point=BlackLine[lr][start_point]-(BlackLine[lr][start_point]-BlackLine[lr][pre_break])*(start_point-irow)/(start_point-pre_break);
			  offset++;
			  num++;
			  if(irow<20)	offset=2;
			  if(num==8||irow<10)	break;
		  }
		  else
		  {
	 		pre_point=BlackLine[lr][EndRow[lr]+1]-(EndRow[lr]+1-irow)*
				(BlackLine[lr][midrow]-BlackLine[lr][EndRow[lr]])/(midrow-EndRow[lr]);
			num++;
			if(num==6)	offset+=5;
			if(num>=6)	pre_point+=5*(2*lr-1);
			if(num==18||irow<10)
				break;
		  }
	  }
	}
}
byte CrossPoint(byte lr,byte irow,byte pre_point,byte flags,byte offset)
{
    byte search_point,edge_point,width=2;			
    signed char	search_direction=1-2*lr;         		 //左右搜索方向（-1：向左，1：向右)
    signed char	compare_direction=search_direction;  //左右比较方向 (-1：向左，1：向右)
  	byte j=0,k=0,l=0;	//循环控制变量
	if(pre_point>=COLUMN)	pre_point=COLUMN-1;
	search_point=pre_point-search_direction;
	edge_point=search_point+compare_direction*width;//
  	for(j=0;j<2;++j){		//j表示左右两个找线方向                             
  		 for(k=0;k<=offset;++k){	//搜索偏移量											
  			search_point+=search_direction;  //计算当前搜索点位置
			edge_point+=search_direction;
  			if(edge_point>COLUMN||search_point>COLUMN)
  		  		break;
			if(g_pix[irow][search_point])
				continue;
    		for(l=1;l<=width;++l) 			
    		    if(!g_pix[irow][search_point+l*compare_direction])       
    				break;
			if(l>width){ 
  	  			BlackLine[lr][irow]=search_point;
    				return 1;
			}
 		 }
 		search_direction=-search_direction;     
		search_point=pre_point;
		edge_point=search_point+compare_direction*width;
	}
  	BlackLine[lr][irow]=COLUMNS;
	if(flags==0)	return 0;
	else       	  	return 2;
}
void FillCross(byte lr)         
{									//先确定填充范围
	byte irowb=EndRow[lr]-CrsLen;		//起点
	byte irowe=0;		//终点
	for(;irowe<irowb;++irowe)			//寻找终点
		if(BlackLine[lr][irowe]!=COLUMNS)
			break;
	if(irowe==irowb)
		return;
	for(;irowb>irowe;--irowb)				//寻找起点
		if(BlackLine[lr][irowb]!=COLUMNS)
			break;
	if(irowb-irowe<1)
		FillLine(lr,EndRow[lr]+1,irowb);
	else if(irowb-irowe<5)FillLine(lr,EndRow[lr]+1,irowb-1);
	else if(irowb-irowe<7)FillLine(lr,EndRow[lr]+1,irowb-2);
	else if(irowb-irowe<9)FillLine(lr,EndRow[lr]+1,irowb-3);
	else  FillLine(lr,EndRow[lr]+1,irowb-4);
	EndRow[lr]=irowe;
}
void ReBuildCross()
{
	byte irow,irowe,irowb,start=0,end,lastw=0;
	byte col,fnum=0,unum=0,sedge;
	if(StartRow[0]==ROWS)	EndRow[0]=ROWS;
	if(StartRow[1]==ROWS)	EndRow[1]=ROWS;
	if(EndRow[1]>50&&EndRow[0]<30)	//右边没找到
	{
		irowe=EndRow[0];
		irowb=StartRow[0];
		for(irow=irowb;irow>irowe;irow--)
		{
			sedge=2*(Offset[irow]-COLUMN1_2);
			for(col=5;col<sedge;col++)
			{
				if(BlackLine[0][irow]+col>COLUMN)
				{
					col=sedge;break;
				}
				if(!g_pix[irow][BlackLine[0][irow]+col]&&(ABS(col-lastw)<5||start==0))
				{
					if(start==0)	start=irow;
					lastw=col;
					end=irow;
					fnum++;
					if(fnum>2)
						unum=0;
					BlackLine[1][irow]=BlackLine[0][irow]+col;break;
				}
			}
			if(col==sedge)
			{
				unum++;
				if(start==0)
				{
					if(unum>10)		break;
				}
				else if(unum>1)		break;
			}

		}
		if(start!=0)
		{
			if(StartRow[1]==ROWS)
			{
				StartRow[1]=start;
				EndRow[1]=end;
			}
			else
			{
				FillLine(1,EndRow[1],start);
				EndRow[1]=end;
			}
		}
	}
	else if(EndRow[1]<30&&EndRow[0]>50)//左边没找到
	{
		irowe=EndRow[1];
		irowb=StartRow[1];
		for(irow=irowb;irow>irowe;irow--)
		{
			sedge=2*(Offset[irow]-COLUMN1_2);
			for(col=5;col<sedge;col++)
			{
				if(BlackLine[1][irow]-col<1)
				{
					col=sedge;break;
				}
				if(!g_pix[irow][BlackLine[1][irow]-col]&&(ABS(col-lastw)<5||start==0))
				{
					if(start==0)	start=irow;
					fnum++;
					if(fnum>2)	unum=0;
					end=irow;
					BlackLine[0][irow]=BlackLine[1][irow]-col;break;
				}
			}
			if(col==sedge)
			{
				unum++;
				if(start==0)
				{
					if(unum>10)		break;
				}
				else if(unum>1)		break;
			}
		}
		if(start!=0)
		{
			if(StartRow[0]==ROWS)
			{
				StartRow[0]=start;
				EndRow[0]=end;
			}
			else
			{
				FillLine(0,EndRow[0],start);
				EndRow[0]=end;
			}
		}
	}
}
void GetCenterLine()
{
	byte lrb=0,lre=0,lrb1,lre1,irow,tem;
	byte substart,subend;
	signed char dis;
	for(irow=0;irow<ROWS;++irow)
		CenterLine[irow]=-1;
	if(EndRow[0]>55&&EndRow[1]>55)
	{
		RoadType=NoLine;
		return;
	}
    if(StartRow[0]==ROWS||(EndRow[0]>55&&EndRow[1]<30)){				//左边没线
		if(EndRow[1]<35){
			for(irow=EndRow[1];irow<=StartRow[1];irow++)
				CenterLine[irow]=BlackLine[1][irow]-(Offset[irow]-COLUMN1_2);//+5
		}
		else{
			for(irow=StartRow[1];irow>=EndRow[1];irow--)
				CenterLine[irow]=BlackLine[1][irow]-(Offset[irow]-COLUMN1_2)-(StartRow[1]-irow)/2;//+5
		}
		RoadStart=StartRow[1];
		RoadEnd=EndRow[1];
	}
	else if(StartRow[1]==ROWS||(EndRow[1]>55&&EndRow[0]<30)){			//右边没线
		if(EndRow[0]<35){
			for(irow=EndRow[0];irow<=StartRow[0];irow++)
				CenterLine[irow]=BlackLine[0][irow]+(Offset[irow]-COLUMN1_2);//+5
		}
		else{
			for(irow=StartRow[0];irow>=EndRow[0];irow--)
				CenterLine[irow]=BlackLine[0][irow]+(Offset[irow]-COLUMN1_2)+(StartRow[0]-irow)/2;//+5
		}
		RoadStart=StartRow[0];
		RoadEnd=EndRow[0];
	}
	else if(StartRow[0]<EndRow[1]){//左边在前
		RoadStart=StartRow[1];
		RoadEnd=EndRow[0];
		if(RoadEnd<=35){
			for(irow=EndRow[1];irow<=StartRow[1];++irow)//近处,单边平移
				CenterLine[irow]=BlackLine[1][irow]-(Offset[irow]-COLUMN1_2);//
			for(irow=EndRow[1];irow>=StartRow[0];irow--)//中间间隔处，趋势延伸
				CenterLine[irow]=CenterLine[EndRow[1]]+(EndRow[1]-irow)//此处可优化
				*(BlackLine[1][EndRow[1]]-BlackLine[1][StartRow[1]])/(StartRow[1]-EndRow[1]);
			//远处，趋势平移
			dis=CenterLine[StartRow[0]]-BlackLine[0][StartRow[0]]-(Offset[StartRow[0]]-COLUMN1_2);
			for(irow=StartRow[0];irow>=EndRow[0]&&irow<ROWS;irow--)
				CenterLine[irow]=BlackLine[0][irow]+(Offset[irow]-COLUMN1_2)+dis;
		}
		else{
			for(irow=EndRow[1];irow<=StartRow[1];++irow)//近处
				CenterLine[irow]=BlackLine[1][irow]-(Offset[irow]-COLUMN1_2)+(irow-EndRow[1])/2;//
			for(irow=EndRow[1];irow>=StartRow[0];irow--)//远处
				CenterLine[irow]=CenterLine[EndRow[1]]+(EndRow[1]-irow)//此处可优化
				*(BlackLine[1][EndRow[1]]-BlackLine[1][StartRow[1]])/(StartRow[1]-EndRow[1]);
			dis=CenterLine[StartRow[0]]-BlackLine[0][StartRow[0]]-(Offset[StartRow[0]]-COLUMN1_2);
			for(irow=StartRow[0];irow>=EndRow[0]&&irow<ROWS;irow--)
				CenterLine[irow]=BlackLine[0][irow]+(Offset[irow]-COLUMN1_2)+dis+(StartRow[0]-irow);
		}
	}
	else if(StartRow[1]<EndRow[0]){//右边在前
		RoadStart=StartRow[0];
		RoadEnd=EndRow[1];
		if(RoadEnd<=33){
			for(irow=EndRow[0];irow<=StartRow[0];++irow)
				CenterLine[irow]=BlackLine[0][irow]+(Offset[irow]-COLUMN1_2);//
			for(irow=EndRow[0];irow>=StartRow[1];irow--)
				CenterLine[irow]=CenterLine[EndRow[0]]+(EndRow[0]-irow)//此处可优化
				*(CenterLine[EndRow[0]]-CenterLine[StartRow[0]])/(StartRow[0]-EndRow[0]);
			dis=CenterLine[StartRow[1]]-BlackLine[1][StartRow[1]]+(Offset[StartRow[1]]-COLUMN1_2);
			for(irow=StartRow[1];irow>=EndRow[1]&&irow<ROWS;irow--)
				CenterLine[irow]=BlackLine[1][irow]-(Offset[irow]-COLUMN1_2)+dis;
		}
		else{
			//以近处为准，远处平移
			for(irow=EndRow[0];irow<=StartRow[0];++irow)
				CenterLine[irow]=BlackLine[0][irow]+(Offset[irow]-COLUMN1_2)-(irow-EndRow[0])/2;//
			for(irow=EndRow[0];irow>=StartRow[1];irow--)
				CenterLine[irow]=CenterLine[EndRow[0]]+(EndRow[0]-irow)//此处可优化
				*(CenterLine[EndRow[0]]-CenterLine[StartRow[0]])/(StartRow[0]-EndRow[0]);
			dis=CenterLine[StartRow[1]]-BlackLine[1][StartRow[1]]+(Offset[StartRow[1]]-COLUMN1_2);
			for(irow=StartRow[1];irow>=EndRow[1]&&irow<ROWS;irow--)
				CenterLine[irow]=BlackLine[1][irow]-(Offset[irow]-COLUMN1_2)+dis-(StartRow[1]-irow);
		}
	}
	else{
		if(StartRow[0]<StartRow[1])
			lrb=1;
		if(EndRow[0]>EndRow[1])
			lre=1;
		RoadStart=StartRow[lrb];
		RoadEnd=EndRow[lre];
		substart=StartRow[(lrb+1)%2];
		subend=EndRow[(lre+1)%2];
		//中间：均值
		for(irow=subend;irow<=substart;++irow)
			CenterLine[irow]=(BlackLine[0][irow]+BlackLine[1][irow])/2;//+tem;
		lrb1=2*lrb-1;
		lre1=2*lre-1;
		dis=CenterLine[subend]-BlackLine[lre][subend];
		dis=dis+lre1*Offset[subend];
		//远处，趋势平移
		if(RoadEnd<=35){
			for(irow=subend-1;irow>=RoadEnd&&irow<ROWS;--irow)
				CenterLine[irow]=BlackLine[lre][irow]-lre1*Offset[irow]+dis;
		}
		else{
			for(irow=subend-1;irow>=RoadEnd&&irow<ROWS;--irow)
				CenterLine[irow]=BlackLine[lre][irow]-lre1*Offset[irow]+dis+(1-2*lre)*(subend-irow);
		}
		dis=CenterLine[substart]-BlackLine[lrb][substart];
		dis=dis+lrb1*Offset[substart];
		//近处，趋势平移
		for(irow=substart+1;irow<=RoadStart;++irow)
			CenterLine[irow]=BlackLine[lrb][irow]-lrb1*Offset[irow]+dis+(2*lrb-1)*(irow-substart)/2;
		//急弯，整体平移
		if(substart-subend<5){
			for(irow=RoadEnd;irow<=RoadStart;irow++)
				CenterLine[irow]+=(2*lrb-1)*7;
		}
	}
}
//*************************************************************************************
void SmoothLine(byte lr)
{
	byte irow,irowe,irowb;
	if(lr==Center){
		irowb=RoadStart;
		irowe=RoadEnd;
		for(irow=irowb-1;irow>irowe;--irow)
			CenterLine[irow]=(CenterLine[irow+1]+CenterLine[irow-1])/2;
		CenterLine[irowb]=2*CenterLine[irowb-1]-CenterLine[irowb-2];
		CenterLine[irowe]=2*CenterLine[irowe+1]-CenterLine[irowe+2];
		if(BlackLine[lr][irowb]>200)	BlackLine[lr][irowb]=0;
		if(BlackLine[lr][irowe]>200)	BlackLine[lr][irowe]=0;
	}
	else{
		irowb=StartRow[lr];
		irowe=EndRow[lr];
		for(irow=irowb-1;irow>irowe;--irow)
			BlackLine[lr][irow]=(BlackLine[lr][irow+1]+BlackLine[lr][irow-1])/2;
		BlackLine[lr][irowb]=2*BlackLine[lr][irowb-1]-BlackLine[lr][irowb-2];
		BlackLine[lr][irowe]=2*BlackLine[lr][irowe+1]-BlackLine[lr][irowe+2];
		if(BlackLine[lr][irowb]>200)	BlackLine[lr][irowb]=0;
		if(BlackLine[lr][irowe]>200)	BlackLine[lr][irowe]=0;
	}
}
void AnalyzeRoadType()
{
	AnalyzeRoad();

	if(RoadType!=NoLine)
	{
		if(!JudgeStraight())
		{
			if(!JudgeSmallS())
			{
				if(!JudgeToT())
				{
				//	if(!JudgeBigT())
				//	{
						if(!JudgeUTurn())
						{
							JudgeGeneral();
						}
				//	}
				}
			}
		}
	}
}
void AnalyzeRoad()
{
	byte i,j,k,irow,irowb,irowe,flags;

	if((StartRow[0]==ROWS&&StartRow[1]==ROWS)||(EndRow[0]>60&&EndRow[1]>60))
	{
		RoadType=NoLine;
		return;
	}
	if(StartRow[0]-EndRow[0]<5)
		LineType[0]=UnExist;
	else
		LineType[0]=Exist;
	if(StartRow[1]-EndRow[1]<5)
		LineType[1]=UnExist;
	else
		LineType[1]=Exist;
	//计算三根线的向左、向右、向中
	left_d[0]=0;	right_d[0]=0;	mid_d[0]=0;		
	left_d[1]=0;	right_d[1]=0;	mid_d[1]=0;		
	left_d[2]=0;	right_d[2]=0;	mid_d[2]=0;	
	for(irow=RoadStart;irow>RoadEnd;irow--)
		if(CenterLine[irow]==CenterLine[irow-1])
			mid_d[2]++;
		else if(CenterLine[irow]>CenterLine[irow-1])
			left_d[2]++;
		else
			right_d[2]++;
	if(LineType[0]==Exist)
	{
		for(irow=StartRow[0];irow>EndRow[0];irow--)
			if(BlackLine[0][irow]==BlackLine[0][irow-1])
				mid_d[0]++;
			else if(BlackLine[0][irow]>BlackLine[0][irow-1])
				left_d[0]++;
			else
				right_d[0]++;
	}
	if(LineType[1]==Exist)
	{
		for(irow=StartRow[1];irow>EndRow[1];irow--)
			if(BlackLine[1][irow]==BlackLine[1][irow-1])
				mid_d[1]++;
			else if(BlackLine[1][irow]>BlackLine[1][irow-1])
				left_d[1]++;
			else
				right_d[1]++;
	}
	
	//统计起点、拐点、终点
	for(i=0;i<10;i++)
		turn[i]=0;
	count=1;
	turn[0]=RoadStart;
	irow=RoadStart;
	irowe=RoadEnd;
	while(CenterLine[irow]==CenterLine[irow-1]&&irow>irowe)
		irow--;
	if(CenterLine[irow]>CenterLine[irow-1])//向左
		flags=1;		//1:表示向左
	else
		flags=0;
	while(irow>irowe)
	{
		if(flags==1)
		{
			if(CenterLine[irow]<CenterLine[irow-1])
			{
				flags=0;
				turn[count]=irow;
				count++;
			}
		}
		else
		{
			if(CenterLine[irow]>CenterLine[irow-1])
			{
				flags=1;
				turn[count]=irow;
				count++;
			}
		}
		irow--;
	}
	turn[count]=RoadEnd;
	count++;
}
byte JudgeStraight()
{
	byte irowe;
	irowe=MAX(EndRow[0],EndRow[1]);
	if(EndRow[0]<25&&EndRow[1]<25)	//包含直道、小S
	{
		if(ABS((BlackLine[0][irowe]+BlackLine[1][irowe])/2-COLUMN1_2)<15)//远处赛道中心在市场中心
		{
			if(left_d[0]<3&&right_d[1]<3&&SegNum[0]==1&&SegNum[1]==1)//直道类型
			{
				RoadType=Straight;
				return 1;
			}
		}
	}
	return 0;
}
byte JudgeSmallS()
{
	byte i;
	byte left_m=83,right_m=0;
	for(i=RoadEnd;i<=RoadStart;i++)
	{
		if(CenterLine[i]<left_m)
			left_m=CenterLine[i];
		if(CenterLine[i]>right_m)
			right_m=CenterLine[i];
	}
	if(right_m-left_m>=20)
		return 0;
	if(LineType[0]==Exist&&LineType[1]==Exist)
	{
		if(EndRow[0]>EndRow[1])//右线长
		{
			if(right_d[1]<2)
				return 0;
		}
		else	//左线长
		{
			if(left_d[0]<2)
				return 0;
		}
	}
	else if(LineType[0]==Exist)		//左线存在，左线长
	{
		if(left_d[0]<2)
			return 0;
	}
	else
	{
		if(right_d[1]<2)
			return 0;
	}
	if(count-1<3)
		return 0;
	for(i=0;i<count-1;i++)
		if(turn[i]-turn[i+1]>37)
			return 0;
	RoadType=SmallS;
	return 1;
}
byte JudgeToT()
{
	byte irow;
	if(LineType[0]==UnExist&&LineType[1]==UnExist)
		return 0;
	if(count<3)	//没有拐点
	{
		irow=RoadStart;
		if(EndRow[0]<EndRow[1])		//左线长
		{
			while(CenterLine[irow]-CenterLine[RoadStart]<7&&irow>RoadEnd)
			{
				irow--;
			}
			if(irow==RoadEnd)
				return 0;
			if(irow<45)
			{
				RoadType=ToT;
				return 1;
			}
			else
				return 0;
		}
		else			//右线长
		{
			while(CenterLine[RoadStart]-CenterLine[irow]<7&&irow>RoadEnd)
			{
				irow--;
			}
			if(irow==RoadEnd)
				return 0;
			if(irow<45)
			{
				RoadType=ToT;
				return 1;
			}
			else
				return 0;
		}
	}
	else
	{
		if(turn[1]>45)
			return 0;
		else if(ABS(CenterLine[RoadStart]-CenterLine[turn[1]])<6)
		{
			RoadType=ToT;
			return 1;
		}
		else
		{
			irow=turn[1];
			while(ABS(CenterLine[irow]-CenterLine[turn[1]])<6&&irow<RoadStart)
			{
				irow++;
			}
			if(irow==RoadStart)
			{
				RoadType=ToT;
				return 1;
			}
			if(irow-turn[1]>30)
			{
				RoadType=ToT;
				return 1;
			}
			return 0;
		}
	}
}
byte JudgeBigT()
{
	byte i,j,k,irow=RoadStart;
	byte mid=0,left=0,right=0;
	if(LineType[0]==UnExist||LineType[1]==UnExist)
		return 0;
	if(RoadEnd<17){
		if(EndRow[0]<EndRow[1])		//左线长
		{
			while(irow>RoadEnd)
			{
				if(CenterLine[irow]<CenterLine[irow-1])
					break;
				irow--;
			}
			while(irow<RoadStart)
			{
				if(CenterLine[irow]!=CenterLine[irow+1])
					break;
				irow++;
			}
			if(irow<=63&&irow!=RoadEnd)
				return 0;
			if(irow==RoadEnd)
			{
				RoadType=BigT;
				return 1;
			}
			for(i=irow;i>RoadEnd;i--)
				if(CenterLine[i]==CenterLine[i-1])
					mid++;
				else if(CenterLine[i]>CenterLine[i-1])
					left++;
				else
					right++;
			if(left>2)
				return 0;
		}
		else		//右线长
		{
			while(irow>RoadEnd)
			{
				if(CenterLine[irow]>CenterLine[irow-1])
					break;
				irow--;
			}
			while(irow<RoadStart)
			{
				if(CenterLine[irow]!=CenterLine[irow+1])
					break;
				irow++;
			}
			if(irow<=63&&irow!=RoadEnd)
				return 0;
			if(irow==RoadEnd)
			{
				RoadType=BigT;
				return 1;
			}
			for(i=irow;i>RoadEnd;i--)
				if(CenterLine[i]==CenterLine[i-1])
					mid++;
				else if(CenterLine[i]>CenterLine[i-1])
					left++;
				else
					right++;
			if(right>2)
				return 0;
		}
		RoadType=BigT;
		return 1;
	}
	else
		return 0;
}
byte JudgeUTurn()
{
	byte irow,iturn;
	signed char tem;
	if(RoadEnd>=28)
	{
		if(EndRow[0]<EndRow[1]||StartRow[1]==ROWS)		//左线长
		{
			if(BlackLine[0][EndRow[0]]<=COLUMN1_2)
				return 0;
			irow=StartRow[0];
			while(irow>EndRow[0])
			{
				if(BlackLine[0][irow]>BlackLine[0][irow-1])
					break;
				irow--;
			}
			if(irow==EndRow[0])
			{
				RoadType=UTurn;
				return 1;
			}
		}
		else				//右线长
		{
			if(BlackLine[1][EndRow[1]]>=COLUMN1_2)
				return 0;
			irow=StartRow[1];
			while(irow>EndRow[1])
			{
				if(BlackLine[1][irow]<BlackLine[1][irow-1])
					break;
				irow--;
			}
			if(irow==EndRow[1])
			{
				RoadType=UTurn;
				return 1;
			}
		}
	}
	if(count>2&&CenterLine[RoadStart]-CenterLine[turn[1]]>35)		//近处向左
	{
		iturn=turn[1];
		while(iturn<RoadStart)
		{
			if(CenterLine[iturn]!=CenterLine[iturn+1])
				break;
			iturn++;
		}
		if(BlackLine[1][iturn]>COLUMN1_2-10||iturn<26)
			return 0;
		irow=StartRow[1];
		while(irow>iturn)
		{
			if(BlackLine[1][irow]<BlackLine[1][irow-1])
				break;
			irow--;
		}
		if(irow==iturn)
		{
			RoadType=UTurn;
			RoadEnd=turn[1];
			EndRow[1]=turn[1];
	    	if(StartRow[0]==ROWS)	tem=EndRow[1]/8+(8-(StartRow[1]-EndRow[1])/8);
	    	else
	    	{
	    		tem=EndRow[0]>60?(EndRow[0]-60)/2:(60-EndRow[0])/10;
	    		tem+=6-(StartRow[1]-EndRow[1])/8;	
	    	}
			for(irow=RoadEnd;irow<=RoadStart;irow++)
				CenterLine[irow]=CenterLine[irow]-2*tem;
			return 1;
		}
		else	return 0;
	}
	else if(count>2&&CenterLine[turn[1]]-CenterLine[RoadStart]>35)	//近处向右
	{
		iturn=turn[1];
		while(iturn<RoadStart)
		{
			if(CenterLine[iturn]!=CenterLine[iturn+1])
				break;
			iturn++;
		}
		if(BlackLine[0][iturn]<COLUMN1_2+10||iturn<26)
			return 0;
		irow=StartRow[0];
		while(irow>iturn)
		{
			if(BlackLine[0][irow]>BlackLine[0][irow-1])
				break;
			irow--;
		}
		if(irow==iturn)
		{
			RoadType=UTurn;
			RoadEnd=turn[1];
			EndRow[0]=turn[1];
			if(StartRow[1]==ROWS)	tem=EndRow[0]/8+(8-(StartRow[0]-EndRow[0])/8);
			else
			{
				tem=EndRow[1]>60?(EndRow[1]-60):(60-EndRow[1])/10;
				tem+=6-(StartRow[0]-EndRow[0])/8;	
			}
			for(irow=EndRow[0];irow<=StartRow[0];irow++)
				CenterLine[irow]=CenterLine[irow]+2*tem;
			return 1;
		}
		else	return 0;
	}
	return 0;
}
void JudgeGeneral()
{
	if(RoadEnd<15)
	{
		RoadType=Far;
	}
	else if(RoadEnd<30)
	{
		RoadType=Mid;	
	}
	else
	{
		RoadType=Near;
	}
}


void GetTurnPoint()
{
	byte irow,irowb,irowe,countnum,point,pcount=0,ppoint;
	if(RoadType==ToT)
	{
		irowb=RoadStart;	irowe=RoadEnd;
		point=irowe;	countnum=1;
		for(irow=irowe+1;irow<=irowb;irow++){
			if(CenterLine[irow]==CenterLine[irow-1])
				countnum++;
			else if(countnum>pcount){
				if(countnum>=5){
					RoadTurn=point;	break;
				}
				if(countnum<pcount){
					ppoint=point;	pcount=countnum;
					countnum=1;	point=irow;
				}
			}
		}
		RoadTurn=ppoint;
	}
	else if(RoadType==BigS)
	{
		if(EndRow[0]<EndRow[1]){
			for(irow=RoadStart;irow>RoadEnd;irow--)
				if(CenterLine[irow]<CenterLine[irow-1])
					break;
		}
		else{
			for(irow=RoadStart;irow>RoadEnd;irow--)
				if(CenterLine[irow]>CenterLine[irow-1])
					break;
		}
		RoadTurn=irow;
	}
}

void DetectSlope()
{
	if(Slope==0)	//正常赛道
	{
		if(detected==0)	//检测坡道前
		{
			DetectUpSlope();//检测上坡标志
			if(UpSlope==1&&PastUpSlope==1&&PPastUpSlope==1&&PPPastUpSlope==1)//连续三场上坡标志认为检测到坡道
			{
				Slope=1;
				detected=1;			
			}
			else	//记录上坡标志
			{
				PPPastUpSlope=PPastUpSlope;
				PPastUpSlope=PastUpSlope;
				PastUpSlope=UpSlope;
			}		
		}
		else	//下坡后直道
		{
			detect_slope_delay++;
			if(detect_slope_delay==35)//延时40场开始下一次坡道检测
			{
				detect_slope_delay=0;
				detected=0;
			}
		}
	}
	else if(Slope==1)	//检测到上坡后
	{
		slope_delay++;
		if(slope_delay<42)
		{
			DetectDownSlope();	//检测下坡标志
			if(PPastDownSlope==1&&PastDownSlope==1&&DownSlope==0)	//连续3场下坡标志认为到了下坡
			{
				Slope=2;
				PastUpSlope=0;
				PPastDownSlope=0;
			}
			else	//记录下坡标志
			{
				PPastDownSlope=PastDownSlope;
				PastDownSlope=DownSlope;	
			}
		}
		else
		{
			Slope=0;
			PastUpSlope=0;
			PPastUpSlope=0;
			PPPastUpSlope=0;
			PastDownSlope=0;
			PPastDownSlope=0;
			slope_delay=0;
		}
	}
	else if(Slope==2)	//下坡
	{
		detect_slope_delay++;
		if(detect_slope_delay>18)	//20场之后自动认为离开坡道
		{
			Slope=0;
			PastUpSlope=0;
			PPastUpSlope=0;
			PPPastUpSlope=0;
			PastDownSlope=0;
			PPastDownSlope=0;
			slope_delay=0;
		}
		else if(detect_slope_delay>8)	//延时10场检测离开坡道
		{
			DetectUpSlope();	//离开坡道时有上坡标志
			if(PastUpSlope==1&&UpSlope==0)	//连续两场检测到上坡标志
			{
				Slope=0;	//离开坡道
				PastUpSlope=0;
				PPastUpSlope=0;
				PPPastUpSlope=0;
				PastDownSlope=0;
				PPastDownSlope=0;
				slope_delay=0;
			}
			else	PastUpSlope=UpSlope;
		}

	}
}
void DetectUpSlope()
{
	byte irowend,fupslope=0;
	//把无效线和小S踢出去
	if(RoadType==SmallS||RoadType==NoLine)
	{
		UpSlope=0;
		return;
	}
	//把虚线小S踢出去
	if((SegNum[0]+SegNum[1])>3||left_d[0]>=5||right_d[1]>=5)
	{
		UpSlope=0;
		return;
	}
	if(RoadEnd>15)
	{
		UpSlope=0;
		return;
	}
	if(StartRow[0]==ROWS||StartRow[1]==ROWS)//左线或右线没找到线
	{
		UpSlope=0;
		return;
	}
	if(EndRow[0]<2&&EndRow[1]<2)
	{
		irowend=MAX(EndRow[1],EndRow[0]);
		if(BlackLine[1][irowend]-BlackLine[0][irowend]>28)
			fupslope=1;
	}
	if(fupslope==1)
		UpSlope=1;
	else
		UpSlope=0;
}
void DetectDownSlope()
{
	byte irowe,irow,irowb,fdownslope=0,countnum=0;
	irowe=MAX(EndRow[0],EndRow[1]);
	irowb=MIN(StartRow[0],StartRow[1]);
	if(irowb<=40)
		fdownslope=0;
	else if(irowe<10)
	{
		for(irow=irowe;irow<=30;irow++)
		{
			if(BlackLine[1][irow]-BlackLine[0][irow]<15+irow/3)
				countnum++;
		}
		if(countnum>8)
			fdownslope=1;
	}
	else if(irowe<20)
	{
		if(ABS(CenterLine[irowe]-COLUMN1_2)<6&&ABS(CenterLine[irowb]-COLUMN1_2)<6)
			fdownslope=1;
	}
	if(fdownslope==1)
		DownSlope=1;
	else
		DownSlope=0;
}

void DetectStartLine()
{
	byte i;
	if(StartDelay<250)
	{
		StartDelay++;
		StartFlags=0;
	}
	else
		DetectStart();
	for(i=1;i<10;i++)
		StartList[i]=StartList[i-1];
	StartList[0]=StartFlags;
	startcount=0;
	for(i=0;i<10;i++)
	{
		if(StartList[i]==1)
			startcount++;
	}
	if(startcount>=2)	StartLine=1;	
}

void DetectStart()
{
	byte irow,icolumn,irowb,irowe,width,temrow1,temrow2;
	byte lstart=0,lend=0,rstart=0,rend=0;
	StartFlags=0;
	if(RoadType!=Straight)	//直道上检测起始线
		return;	
	if(Slope!=0)	//坡道不检测起始线
		return;
	irowe=MAX(EndRow[0],EndRow[1]);
	irowe=MAX(irowe,25);
	irowb=MIN(StartRow[0],StartRow[1]);
	if(irowb<=irowe)	
		return;	
	for(irow=irowb-1;irow>=irowe;irow--)
	{
		width=(BlackLine[1][irow]-CenterLine[irow])/2+CenterLine[irow];
		temrow1=irow;	//上一黑点行数
		for(icolumn=CenterLine[irow];icolumn<BlackLine[1][irow];icolumn++)//右侧起始线
		{
			if(icolumn==width&&rstart==0)	//
				break;
			if(!g_pix[temrow1][icolumn])	//黑点
			{
				if(rstart==0)	//首次找到黑点
					rstart=icolumn;
			}
			else if(!g_pix[temrow1-1][icolumn])			
				temrow1--;
			else if(!g_pix[temrow1+1][icolumn])
				temrow1++;
			else
				rend=icolumn-1;
		}
		width=CenterLine[irow]-(CenterLine[irow]-BlackLine[0][irow])/2;
		temrow2=irow;	//上一黑点行数
		for(icolumn=CenterLine[irow];icolumn>BlackLine[0][irow];icolumn--)//左侧起始线
		{
			if(icolumn==width&&lstart==0)	//
				break;
			if(!g_pix[temrow2][icolumn])	//黑点
			{
				if(lstart==0)	//首次找到黑点
					lstart=icolumn;
			}
			else if(!g_pix[temrow2-1][icolumn])			
				temrow2--;
			else if(!g_pix[temrow2+1][icolumn])
				temrow2++;
			else
				lend=icolumn-1;
		}
		if(rstart&&lstart)
			break;
	}
	if(irow<irowe)
		return;	
	width=BlackLine[1][temrow1]-BlackLine[0][temrow2];
	if((rstart-lstart)>width/7&&(rend-rstart)>width/7&&(lstart-lend)>width/7)
		StartFlags=1;
	else
		StartFlags=0;
}
void ReBuildWeight()		//修复直道入弯
{
	byte irow;
	if(RoadType==Far){
		if(count==3)
		{
			TurnPoint=turn[1];
		}
		else if(count==4){
			TurnPoint=(turn[0]*turn[1]+turn[2]*turn[2]-turn[2]*turn[3]-turn[1]*turn[1])/(turn[0]+turn[2]-turn[3]-turn[1]);
		}
		if(count==3||count==4)
		{
			for(irow=RoadEnd;irow<TurnPoint;irow++)
			{
				FarWeight[irow]=32*(irow-RoadEnd)/(TurnPoint-RoadEnd);
			}
			for(irow=TurnPoint;irow<=RoadStart;irow++)
			{
				FarWeight[irow]=32*(RoadStart-irow)/(RoadStart-TurnPoint);
			}
		}
		else
		{
			for(irow=RoadEnd;irow<=RoadStart;irow++)
				FarWeight[irow]=FarWeightTem[irow];
		}
	}
}
//*****************************************************************************************************************
//*	 *************************通过有效行分类计算offset******************************************************* 	  *
//*****************************************************************************************************************
void TargetOffset()
{	
	if(Slope==1)	UpSlopeOffset();
	else if(Slope==2) UpSlopeOffset();
	
	else if(RoadType==Straight||RoadType==SmallS) StraightOffset();
	else if(RoadType==UTurn) UTurnOffset();
	else if(RoadType==ToT)	ToToffset();
	else if(RoadType==Near||RoadType==Mid) NearOffset();
	//else if(RoadType==Mid) MidOffset();
	else FarOffset();
	
	if(target_offset>43) target_offset=43;
	else if(target_offset<-43) target_offset=-43;
}

void UpSlopeOffset()	//上坡偏移量计算
{
	byte irow,irowe;
	signed long sum_weight=0;
	target_offset=0;
	irowe=RoadEnd<45?45:RoadEnd;
	for(irow=irowe;irow<=(RoadStart-5);++irow){
		target_offset+=(CenterLine[irow]-COLUMN1_2)*SlopeWeight[irow];
		sum_weight+=SlopeWeight[irow];
	}
	target_offset/=sum_weight;
}

void StraightOffset()	//直道偏移量计算
{
	byte irow,irowe;
	signed long sum_weight=0;
	target_offset=0;
	for(irow=RoadEnd;irow<=RoadStart;++irow){
		target_offset+=(CenterLine[irow]-COLUMN1_2)*StraightWeight[irow];
		sum_weight+=StraightWeight[irow];
	}
	target_offset/=sum_weight;
}

void UTurnOffset()	//180度弯偏移量计算
{
	byte irow,irowe;
	signed long sum_weight=0;
	target_offset=0;
	//irowe=RoadEnd<41?40:RoadEnd;
	for(irow=RoadEnd;irow<=RoadStart;++irow){
		target_offset+=(CenterLine[irow]-COLUMN1_2)*UTurnWeight[irow];
		sum_weight+=UTurnWeight[irow];
	}
	target_offset/=sum_weight;
}

void ToToffset()
{
	byte irow,irowe;
	signed long sum_weight=0;
	target_offset=0;
	//irowe=RoadStart>65?65:RoadStart;
	if(mode==0)
	{
		for(irow=RoadEnd;irow<=RoadStart;++irow){
			target_offset+=(CenterLine[irow]-COLUMN1_2)*ToTWeightMode0[irow];
			sum_weight+=ToTWeightMode0[irow];
		}
	}
	else if(mode==1)
	{
		for(irow=RoadEnd;irow<=RoadStart;++irow){
			target_offset+=(CenterLine[irow]-COLUMN1_2)*ToTWeightMode1[irow];
			sum_weight+=ToTWeightMode1[irow];
		}
	}
	else
	{
		for(irow=RoadEnd;irow<=RoadStart;++irow){
			target_offset+=(CenterLine[irow]-COLUMN1_2)*ToTWeight[irow];
			sum_weight+=ToTWeight[irow];
		}
	}
	target_offset/=sum_weight;
}



void FarOffset()
{
	byte irow,irowe;
	signed long sum_weight=0;
	target_offset=0;
//	if(count!=3)
//		irowe=RoadStart>39?39:RoadStart;
	if(mode==0)
	{
		for(irow=RoadEnd;irow<=RoadStart;++irow){
			target_offset+=(CenterLine[irow]-COLUMN1_2)*FarWeightMode0[irow];
			sum_weight+=FarWeightMode0[irow];
		}
	}
	else
	{
		for(irow=RoadEnd;irow<=RoadStart;++irow){
			target_offset+=(CenterLine[irow]-COLUMN1_2)*FarWeight[irow];
			sum_weight+=FarWeight[irow];
		}
	}
	target_offset/=sum_weight;
}

/*void MidOffset()
{
	byte irow,irowe;
	signed long sum_weight=0;
	target_offset=0;
	if(count!=3)
		irowe=RoadStart>45?45:RoadStart;
	for(irow=RoadEnd;irow<=RoadStart;++irow){
		target_offset+=(CenterLine[irow]-COLUMN1_2)*MidWeight[irow];
		sum_weight+=MidWeight[irow];
	}
	target_offset/=sum_weight;
}*/

void NearOffset()
{
	byte irow,irowe;
	signed long sum_weight=0;
	target_offset=0;
	//irowe=RoadStart>65?65:RoadStart;
	if(mode==0)
	{
		for(irow=RoadEnd;irow<=RoadStart;++irow){
			target_offset+=(CenterLine[irow]-COLUMN1_2)*NearWeightMode0[irow];
			sum_weight+=NearWeightMode0[irow];
		}
	}
	else if(mode==1)
	{
		for(irow=RoadEnd;irow<=RoadStart;++irow){
			target_offset+=(CenterLine[irow]-COLUMN1_2)*NearWeightMode1[irow];
			sum_weight+=NearWeightMode1[irow];
		}
	}
	else
	{
		for(irow=RoadEnd;irow<=RoadStart;++irow){
			target_offset+=(CenterLine[irow]-COLUMN1_2)*NearWeight[irow];
			sum_weight+=NearWeight[irow];
		}
	}
	target_offset/=sum_weight;
}