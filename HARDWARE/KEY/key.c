#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "key.h"
#include "delay.h"
#include "CLOCK_FUCTION.h"
#include "WATCH_FUCTION.h"

u16 WKUP_STATE=0;  //WK_UP按键的状态值，每次扫描到按下加1，未按下清0
u16 KEY0_STATE=0;  //KEY0状态值
u16 KEY1_STATE=0;  //KEY1状态值
extern u8 MODE;    //模式
extern u8 Clock_State; //时钟的状态 
extern u8 lock;  //键盘锁


								    
//按键初始化函数
void KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);//使能PORTA,PORTE时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4|GPIO_Pin_3;//KEY0-KEY1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE4,3

	//初始化 WK_UP-->GPIOA.0	  下拉输入
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0设置成输入，默认下拉	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.0

}
//按键处理函数，按键扫描
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEY0按下
//2，KEY1按下
//3，KEY3按下 WK_UP
//注意此函数有响应优先级,KEY0>KEY1>KEY_UP!!
void KEY_Scan(void) 
{
	  if(WK_UP==0) //WK_UP松开
   {
       
	   if(WKUP_STATE==0) 
	   {
		   ;
	   }
	   else if(WKUP_STATE>0&&WKUP_STATE<200)
	   { 
		   if(lock == 0) {
		       MODE_SWITCH();//短按切换模式
		   }
		   WKUP_STATE=0;//状态值清0
	   }
	   else if(WKUP_STATE>=200)
		   WKUP_STATE=0;//状态值清0
                  
	 }
	else if(WK_UP==1) //WK_UP按下
	{
		if(WKUP_STATE==200 && MODE == 0)
		{
			switch(Clock_State) //判断状态位
			{
				case 0:
				Debug_Clock();//模式0下长按进入时钟设置模式
                WKUP_STATE++;
				break;
				case 1:
			    Run_Clock(); //模式0下长按离开时钟设置模式
                WKUP_STATE++;
				break;				
			}	 
		}else if(WKUP_STATE==200 && MODE == 2) {
			  power_switch();  //模式2下长按，开关机操作
			  WKUP_STATE++;  //状态加一
		}else {
		      WKUP_STATE++;
		}
     }
	if(KEY1==1) //KEY1松开
	{
		switch(MODE) //判断模式
		{
			case 0:			
				if(KEY1_STATE>0&&KEY1_STATE<200)
				   { 
					   Choose_Bit();//模式0下短按KEY1进入位选
					   KEY1_STATE=0;
				   }
				   break;
			case 1:				
				if(KEY1_STATE>0&&KEY1_STATE<200) {
                       display_mode_change();//模式1下短按KEY1切换显示模式
					   KEY1_STATE = 0;
				}else if(KEY1_STATE >=  200) {
					   order_mode_switch();//模式1下长按KEY1切换顺，倒计
					   KEY1_STATE = 0;
				}
						           
			case 2:
				   if(KEY1_STATE>0&&KEY1_STATE<200)
		
					   KEY1_STATE=0;
                   break;				
	    }		       
	 }	
	else if(KEY1==0) //KEY1按下
	{
      KEY1_STATE++;
	}
	
	if(KEY0==1) //KEY0松开
	switch(MODE) //判断模式
		{
			case 0:			
					if(KEY0_STATE>0&&KEY0_STATE<200)
				   { 
					   Be_Chose_Plus();//模式0下被选的位加一
					   KEY0_STATE=0;
				   }
				   break;
			case 1:				
					if(KEY0_STATE>0&&KEY0_STATE<200)
				   { 
					   timer_switch();//模式1下短按KEY0控制考试计时启停
					   KEY0_STATE=0;
				   }else if(KEY0_STATE>=200) {
					   timer_reset();//模式1下长按KEY0重置计时
					   KEY0_STATE=0;
				   }
     				   
  			       break;
			case 2:
				   if(KEY0_STATE>0&&KEY0_STATE<200)
			
					   KEY0_STATE=0;
                   break;				   
	   }
	else if(KEY0==0)
	{
		if(MODE==0)		
		{
				if(KEY0_STATE==200)
				{   
					if(Clock_State==1)
					{
					Be_Chose_Plus();//模式0下短按KEY0被选位加一
					KEY0_STATE=170;//状态切回，实现连续加一
					}
                }
				else
				{
					KEY0_STATE++;
				}
		}
	    else
		{
	     KEY0_STATE++;
		}
    }
}
