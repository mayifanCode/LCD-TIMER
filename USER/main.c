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

u8 MODE=0;   //MODE=0 CLOCK ;MODE=1 TIMER;MODE=2 开机关机

int main(void)
{
	//初始化参数
	u32 fontcnt;		  
	u8 i,j;
	u8 fontx[2];//gbk码
	u8 key,t;   	    	  		
	W25QXX_Init();				//初始化W25Q128
 	my_mem_init(SRAMIN);		//初始化内部内存池
	exfuns_init();				//为fatfs相关变量申请内存  
 	f_mount(fs[0],"0:",1); 		//挂载SD卡 
 	f_mount(fs[1],"1:",1); 		//挂载FLASH.	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	uart_init(9600);	 	//串口初始化为9600
	LED_Init();		  	 	//初始化与LED连接的硬件接口
    KEY_Init();          	//初始化与按键连接的硬件接口
	BEEP_Init();   //初始化蜂鸣器
	delay_init();  //初始化延时函数
	LCD_Init();    //初始化LCD
    LCD_Clear(BLACK);  //清平，设置为黑色背景
	LED0=0;					//点亮LED0
	LED1=1;	                 //关闭LED1
	TIM3_Int_Init(9999,7199);//10Khz的计数频率，计数到10000为1000ms		
	TIM4_Int_Init(9999,7199);//1s,计数器	
	TIM5_Int_Init(999,7199);//10Khz的计数频率，计数到1000为100ms	
	TIM6_Int_Init(99,7199);//10Khz的计数频率，计数到100为10ms	
	font_init();          //初始化字体
	SD_Init();             //初始化sd卡  
	Show_Str(65,50,200,24,"北京时间",24,0); //初始化屏幕显示内容
    LCD_ShowString(83,149,200,24,24,":");
	LCD_ShowString(133,149,200,24,24,":");
    //死循环	
	while(1)
	{
		
	}
	
}


