#ifndef _Nokia5110_H
#define _Nokia5110_H


//位操作定义
#define LCD_RST  SIU.GPDO[17].R
#define LCD_CE   SIU.GPDO[42].R
#define LCD_DC   SIU.GPDO[75].R
#define SDIN     SIU.GPDO[74].R
#define SCLK     SIU.GPDO[72].R 

//函数声明
void LCD_Init(void);
void LCD_clear(void);


void ThreeLineNoVideo(void);
void CenterLineWithVideo();
void Video_Show();
void LCD_Show();
void Delay_us(unsigned int ut);
void Delay(unsigned int ut);




//库函数
void LCD_write_byte(unsigned char, unsigned char);
void LCD_set_XY(unsigned char , unsigned char );      

void LCD_write_char(unsigned char );
void LCD_Write_Num(unsigned char,unsigned char,unsigned int,unsigned char);
void LCD_write_english_string(unsigned char ,unsigned char ,char *); 


#endif
