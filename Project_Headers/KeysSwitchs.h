#ifndef _KeysSwitchs_H
#define _KeysSwitchs_H

extern byte mode;
extern byte RowSAIC_DelayTime;
extern unsigned int CENTER;
extern unsigned int RIGHT;
extern unsigned int LEFT;


void ChooseMode(void);
void ChangeVideoDelayTime(void);
void ChangeCENTER(void);
void DelayKeys();


#endif
