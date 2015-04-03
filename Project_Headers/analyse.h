#ifndef __ANALYSE_H__
#define __ANALYSE_H__
#define byte unsigned char
#define word unsigned int

#define MAX(a,b)            (((a) > (b)) ?   (a)  : (b))
#define MIN(a,b)            (((a) < (b)) ?   (a)  : (b))
#define DISTANCE(a,b)       (((a) < (b)) ? (b-a)  : (a-b))
#define ABS(x)          ((x) > 0 ? (x) : 0-(x))


//左右线类型
#define UnExist	(0)
#define	Exist	(1)

//线型声明
//0-1:首次找线中间量
#define UnBegin	  (0)	//没开始找线
#define CrossLine (1)   //十字
//4- :最终赛道类型
#define NoLine    (2)   //没找到线
#define Straight  (3)   //直道
#define BigT	  (4)   //大弯
#define BigS 	  (5)   //大S
#define SmallS    (6)   //小S
#define ToT		  (7)	//直道入弯
#define UTurn	  (8)	//U型弯
#define UnRec	  (9)	//无法识别的赛道类型
#define Wrong	  (10)	//无效图像


//有效行变量
#define Far		(20)
#define Mid		(21)
#define Near	(22)

//#####################
//找线参数声明
#define Left      (0)   //Left
#define Right     (1)   //Right
#define Center	  (2)
#define Stop      (20)	//首次没找到线，二次找线前移的位置
#define CrsLen	  (10)	//十字二次找线前移位置
#define COLUMN1_2 (41)
#define COLUMN2_3 (55)
#define COLUMN1_3 (27)
#define ROW1_2	  (35)
#define REROW	  (55)  //首次没找到，二次找线的起始行

//变量的外部声明
extern byte BlackLine[2][ROWS];
extern byte StartRow[2];
extern byte EndRow[2];
extern byte LineType[2];	//左右线类型
extern byte RoadType;		//赛道类型
extern byte RoadStart;		//赛道起始行
extern byte RoadEnd;		//赛道终止行
extern byte RoadTurn;		//赛道终止行
extern signed char CenterLine[ROWS];

extern byte StartLine;
extern byte UpSlope;//上坡标志
extern byte DownSlope;//下坡标志
extern byte Slope;	//坡道标志
extern byte DottedLineFlags;//虚线标志

extern byte NearCross[2];
extern byte FarCross[2];

extern byte turn[10];


void FindBlackLine(void);
void Line_Init(byte lr);
void BlackLine_Init(byte lr,byte irowb,byte irowe);
void FindLine(byte lr);
byte NextPoint(byte lr,byte irow,byte flags);
void FillLine(byte lr,byte start,byte end);
void Pre_handle(byte lr);
void RebuildLine();
void Analyze_Cross();
void Rec_Cross();
void FindCross(byte lr);
byte CrossPoint(byte lr,byte irow,byte pre_point,byte flags,byte offset);
void FillCross(byte lr);         //修复十字
void ReBuildCross();
void SmoothLine(byte lr);
void AnalyzeRoadType();
void AnalyzeRoad();
byte JudgeStraight();
byte JudgeSmallS();
byte JudgeToT();
byte JudgeBigT();
byte JudgeUTurn();
void JudgeGeneral();
void GetTurnPoint();
void GetCenterLine();
void DetectSlope();
void DetectUpSlope();
void DetectDownSlope();
void DetectStartLine();
void DetectStart();
void ReBuildWeight();


//******************************偏移量计算****************************************//
void TargetOffset();
void UpSlopeOffset();
void DownSlopeOffset();

void StraightOffset();
void BigTOffset();
void BigSOffset();
void SmallSOffset();
void ToTOffset();
void UTurnOffset();
void ToToffset();
void GeneralOffset();
void RebuildOffset();


void NearOffset();
void MidOffset();
void FarOffset();
#endif


