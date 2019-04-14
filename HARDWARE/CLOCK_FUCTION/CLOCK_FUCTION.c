#include "DISPLAY.h"
#include "lcd.h"
#include "led.h"
#include "TEXT.h"
#include "beep.h"

extern u8 MODE;
extern u8 h;
extern u8 m;
extern u8 s;
u8 Clock_State=0;
u8 TWINKBIT=0;
//ģʽ�л�
void MODE_SWITCH(void)
{
	beep_short();//����
	if(MODE==2)
		MODE=0;
	else MODE++;
	LCD_Clear(BLACK);//����
   switch(MODE)//��ʾ���֣�����ģʽ
   {
	   case 0:
	    Show_Str(65,50,200,24,"����ʱ��",24,0);
		LCD_ShowString(83,149,200,24,24,"-");
		LCD_ShowString(133,149,200,24,24,"-");
	    LED0 = 0;
	    LED1 = 1;
        break;	   
	   case 1:
	    Show_Str(84,20,200,16,"����ʱ��",16,0);
	    LCD_ShowString(93,50,200,16,16,":");	   
	    LCD_ShowString(135,50,200,16,16,":");
	    Show_Str(68,110,200,24,"����ʱ��",24,0);
	    LCD_ShowString(110,150,200,24,24,":");
		LED0 = 1;
	    LED1 = 0; 
        break;	   
	   case 2:
        Show_Str(70,150,200,24,"�����ػ�",24,0);
		LED0 = 0;
	    LED1 = 0; 	   
        break;	   
   }
}	
	
	
	
	
	
	
//ʱ�ӿ�ʼ��ʱ	
void Clock_Run(void)
{	
   TIM_ITConfig(TIM3,TIM_IT_Update, ENABLE);
}	



//ʱ��ֹͣ��ʱ
void Clock_Stop(void)
{	
   TIM_ITConfig(TIM3,TIM_IT_Update, DISABLE);
}
//ʱ������
void Clock_Reset(void)
{
	h=0;
	m=0;
	s=0;
}	
//�޸�λѡ
void Choose_Bit(void)
{
   TWINKBIT++;
	if(TWINKBIT==3)
		TWINKBIT=0;
}

//����ʱ��
void Debug_Clock(void)
{
	Clock_Stop();
	Clock_Reset();
	Clock_State=1;
	
}
//����ʱ��
void Run_Clock(void)
{
	Clock_Run();
	Clock_State=0;
	
}
//��ѡλ��һ�����Ե���
void Be_Chose_Plus(void)
{
	if(Clock_State==1)		
	      switch(TWINKBIT)
			{
				case 0:
				h++;
				if(h == 25) {
					h = 0;
				}
				break;
				case 1:
				m++;
				if(m == 61) {
					m = 0;
				}
				break;
				case 2:
				s++;
				if(s == 61) {
					s = 0;
				}
				break;	
			}	
}
//��ѡλ��һ����ʱ����
void Sec_Plus(void)
{
	    if(h==23&&m==59&&s==59)
		{
		h=0;
		m=0;
		s=0;
		}
	    else if(h!=23&&m==59&&s==59)
		{
		h++;
        m=0;
		s=0;			
		}
		else if(m!=59&&s==59)
		{
		m++;
		s=0;			
		}
		else if(s!=59)
		{
		s++;			
		}	
}
	








