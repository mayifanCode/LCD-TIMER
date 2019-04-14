#include "lcd.h"
#include "TOOLS.h"
#include "sys.h"
#include "WATCH_FUCTION.h"
#include "TEXT.h"

extern u8 MODE;
u8 h=14;//Сʱ
u8 m=25;//����
u8 s=50;//����
u8 Count=0;//����ֵ
u8 h_string[3];//Сʱ�ַ�����ʽ
u8 m_string[3];//�����ַ�����ʽ
u8 s_string[3];//�����ַ�����ʽ
extern u8 Clock_State;//ʱ��״̬
extern u8 TWINKBIT;//λѡ

extern u8 timer_minutes;//���Լ�ʱ������
extern u8 timer_seconds;//���Լ�ʱ������
u8 timer_minutes_string[3];//�����ַ�����ʽ
u8 timer_seconds_string[3];//�����ַ�����ʽ
extern u16 words_point_x;//��ʾ�������Ͻ�x����
extern u16 words_point_y;//��ʾ�������Ͻ�y����
extern u8 test_state;//����״̬


//��ȡ�ַ�����ʱ��
void Get_Char(void)
{
   Num2Char_2bits(h,h_string,0);
   Num2Char_2bits(m,m_string,0);
   Num2Char_2bits(s,s_string,0);	
}
//��ȡ�ַ��������Լ�ʱ
void Get_Timer_Char(void)
{
	Num2Char_2bits(timer_minutes,timer_minutes_string,0);
	Num2Char_2bits(timer_seconds,timer_seconds_string,0);
}
//��ʾɨ�躯��
void Display_Scan(void)
{ 
	switch(MODE)//����ģʽ
	{
       case 0:
	    Get_Char();
	    LCD_ShowString(50,150,240,24,24,h_string);	
		LCD_ShowString(100,150,240,24,24,m_string);
		LCD_ShowString(150,150,240,24,24,s_string);
        if(Count==8)
		{
		     if(Clock_State==1)
		        {
					switch(TWINKBIT)
					{
						case 0:LCD_ShowString(50,150,200,24,24,"  ");break;
						case 1:LCD_ShowString(100,150,200,24,24,"  ");break;	
						case 2:LCD_ShowString(150,150,200,24,24,"  ");break;
						
					}	
				}
		}
        break;	   
	   case 1:
	    Get_Timer_Char();
	    Get_Char();
        change_timer_words_position();       
	    LCD_ShowString(65,50,240,16,16,h_string);	   
		LCD_ShowString(110,50,240,16,16,m_string);
		LCD_ShowString(155,50,240,16,16,s_string);	   
		LCD_ShowString(80,150,240,24,24,timer_minutes_string);
		LCD_ShowString(130,150,240,24,24,timer_seconds_string);
            switch(test_state) {
               case 0:
			        Show_Str(words_point_x,words_point_y,200,24,"",24,0);
            	   break;
               case 1:
			        Show_Str(words_point_x,words_point_y,200,24,"���Կ�ʼ",24,0);
			        LCD_Fill(words_point_x,words_point_y+24,words_point_x+200,words_point_y+48,BLACK);
            	   break;
               case 2:
			        Show_Str(words_point_x,words_point_y,200,24,"���Ծ�1",24,0);
            	   break;
               case 3:
			         Show_Str(words_point_x,words_point_y,200,24,"���Ծ�2",24,0);
            	   break;
               case 4:
			         Show_Str(words_point_x,words_point_y,200,24,"���Խ���",24,0);
                   break;	
		   }				
        break;	   
	}
	
	if(Count==10)
		Count=0;
	Count++;
}

 
			
	
	
	
	
	
	
