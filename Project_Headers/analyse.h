#ifndef __ANALYSE_H__
#define __ANALYSE_H__
#define byte unsigned char
#define word unsigned int

#define MAX(a,b)            (((a) > (b)) ?   (a)  : (b))
#define MIN(a,b)            (((a) < (b)) ?   (a)  : (b))
#define DISTANCE(a,b)       (((a) < (b)) ? (b-a)  : (a-b))
#define ABS(x)          ((x) > 0 ? (x) : 0-(x))


//����������
#define UnExist	(0)
#define	Exist	(1)

//��������
//0-1:�״������м���
#define UnBegin	  (0)	//û��ʼ����
#define CrossLine (1)   //ʮ��
//4- :������������
#define NoLine    (2)   //û�ҵ���
#define Straight  (3)   //ֱ��
#define BigT	  (4)   //����
#define BigS 	  (5)   //��S
#define SmallS    (6)   //СS
#define ToT		  (7)	//ֱ������
#define UTurn	  (8)	//U����
#define UnRec	  (9)	//�޷�ʶ�����������
#define Wrong	  (10)	//��Чͼ��


//��Ч�б���
#define Far		(20)
#define Mid		(21)
#define Near	(22)

//#####################
//���߲�������
#define Left      (0)   //Left
#define Right     (1)   //Right
#define Center	  (2)
#define Stop      (20)	//�״�û�ҵ��ߣ���������ǰ�Ƶ�λ��
#define CrsLen	  (10)	//ʮ�ֶ�������ǰ��λ��
#define COLUMN1_2 (41)
#define COLUMN2_3 (55)
#define COLUMN1_3 (27)
#define ROW1_2	  (35)
#define REROW	  (55)  //�״�û�ҵ����������ߵ���ʼ��

//�������ⲿ����
extern byte BlackLine[2][ROWS];
extern byte StartRow[2];
extern byte EndRow[2];
extern byte LineType[2];	//����������
extern byte RoadType;		//��������
extern byte RoadStart;		//������ʼ��
extern byte RoadEnd;		//������ֹ��
extern byte RoadTurn;		//������ֹ��
extern signed char CenterLine[ROWS];

extern byte StartLine;
extern byte UpSlope;//���±�־
extern byte DownSlope;//���±�־
extern byte Slope;	//�µ���־
extern byte DottedLineFlags;//���߱�־

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
void FillCross(byte lr);         //�޸�ʮ��
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


//******************************ƫ��������****************************************//
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


