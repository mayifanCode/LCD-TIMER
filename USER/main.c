#include "stm32f10x.h"
#include "led.h"
#include "sys.h"
#include "WATCH_FUCTION.h"
#include "CLOCK_FUCTION.h"
#include "lcd.h"
#include "key.h"     
#include "timer.h"
#include "usart.h"
#include "delay.h"
#include "beep.h"		 	     
#include "malloc.h"
#include "sdio_sdcard.h"  
#include "w25qxx.h"    
#include "ff.h"  
#include "exfuns.h"   
#include "text.h"

u8 MODE=0;   //MODE=0 CLOCK ;MODE=1 TIMER;MODE=2 �����ػ�

int main(void)
{
	//��ʼ������
	u32 fontcnt;		  
	u8 i,j;
	u8 fontx[2];//gbk��
	u8 key,t;   	    	  		
	W25QXX_Init();				//��ʼ��W25Q128
 	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ��
	exfuns_init();				//Ϊfatfs��ر��������ڴ�  
 	f_mount(fs[0],"0:",1); 		//����SD�� 
 	f_mount(fs[1],"1:",1); 		//����FLASH.	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	uart_init(9600);	 	//���ڳ�ʼ��Ϊ9600
	LED_Init();		  	 	//��ʼ����LED���ӵ�Ӳ���ӿ�
    KEY_Init();          	//��ʼ���밴�����ӵ�Ӳ���ӿ�
	BEEP_Init();   //��ʼ��������
	delay_init();  //��ʼ����ʱ����
	LCD_Init();    //��ʼ��LCD
    LCD_Clear(BLACK);  //��ƽ������Ϊ��ɫ����
	LED0=0;					//����LED0
	LED1=1;	                 //�ر�LED1
	TIM3_Int_Init(9999,7199);//10Khz�ļ���Ƶ�ʣ�������10000Ϊ1000ms		
	TIM4_Int_Init(9999,7199);//1s,������	
	TIM5_Int_Init(999,7199);//10Khz�ļ���Ƶ�ʣ�������1000Ϊ100ms	
	TIM6_Int_Init(99,7199);//10Khz�ļ���Ƶ�ʣ�������100Ϊ10ms	
	font_init();          //��ʼ������
	SD_Init();             //��ʼ��sd��  
	Show_Str(65,50,200,24,"����ʱ��",24,0); //��ʼ����Ļ��ʾ����
    LCD_ShowString(83,149,200,24,24,":");
	LCD_ShowString(133,149,200,24,24,":");
    //��ѭ��	
	while(1)
	{
		
	}
	
}


