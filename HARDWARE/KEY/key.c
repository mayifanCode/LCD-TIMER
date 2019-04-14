#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "key.h"
#include "delay.h"
#include "CLOCK_FUCTION.h"
#include "WATCH_FUCTION.h"

u16 WKUP_STATE=0;  //WK_UP������״ֵ̬��ÿ��ɨ�赽���¼�1��δ������0
u16 KEY0_STATE=0;  //KEY0״ֵ̬
u16 KEY1_STATE=0;  //KEY1״ֵ̬
extern u8 MODE;    //ģʽ
extern u8 Clock_State; //ʱ�ӵ�״̬ 
extern u8 lock;  //������


								    
//������ʼ������
void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);//ʹ��PORTA,PORTEʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4|GPIO_Pin_3;//KEY0-KEY1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE4,3

	//��ʼ�� WK_UP-->GPIOA.0	  ��������
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0���ó����룬Ĭ������	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.0

}
//����������������ɨ��
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY0����
//2��KEY1����
//3��KEY3���� WK_UP
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY_UP!!
void KEY_Scan(void) 
{
	  if(WK_UP==0) //WK_UP�ɿ�
   {
       
	   if(WKUP_STATE==0) 
	   {
		   ;
	   }
	   else if(WKUP_STATE>0&&WKUP_STATE<200)
	   { 
		   if(lock == 0) {
		       MODE_SWITCH();//�̰��л�ģʽ
		   }
		   WKUP_STATE=0;//״ֵ̬��0
	   }
	   else if(WKUP_STATE>=200)
		   WKUP_STATE=0;//״ֵ̬��0
                  
	 }
	else if(WK_UP==1) //WK_UP����
	{
		if(WKUP_STATE==200 && MODE == 0)
		{
			switch(Clock_State) //�ж�״̬λ
			{
				case 0:
				Debug_Clock();//ģʽ0�³�������ʱ������ģʽ
                WKUP_STATE++;
				break;
				case 1:
			    Run_Clock(); //ģʽ0�³����뿪ʱ������ģʽ
                WKUP_STATE++;
				break;				
			}	 
		}else if(WKUP_STATE==200 && MODE == 2) {
			  power_switch();  //ģʽ2�³��������ػ�����
			  WKUP_STATE++;  //״̬��һ
		}else {
		      WKUP_STATE++;
		}
     }
	if(KEY1==1) //KEY1�ɿ�
	{
		switch(MODE) //�ж�ģʽ
		{
			case 0:			
				if(KEY1_STATE>0&&KEY1_STATE<200)
				   { 
					   Choose_Bit();//ģʽ0�¶̰�KEY1����λѡ
					   KEY1_STATE=0;
				   }
				   break;
			case 1:				
				if(KEY1_STATE>0&&KEY1_STATE<200) {
                       display_mode_change();//ģʽ1�¶̰�KEY1�л���ʾģʽ
					   KEY1_STATE = 0;
				}else if(KEY1_STATE >=  200) {
					   order_mode_switch();//ģʽ1�³���KEY1�л�˳������
					   KEY1_STATE = 0;
				}
						           
			case 2:
				   if(KEY1_STATE>0&&KEY1_STATE<200)
		
					   KEY1_STATE=0;
                   break;				
	    }		       
	 }	
	else if(KEY1==0) //KEY1����
	{
      KEY1_STATE++;
	}
	
	if(KEY0==1) //KEY0�ɿ�
	switch(MODE) //�ж�ģʽ
		{
			case 0:			
					if(KEY0_STATE>0&&KEY0_STATE<200)
				   { 
					   Be_Chose_Plus();//ģʽ0�±�ѡ��λ��һ
					   KEY0_STATE=0;
				   }
				   break;
			case 1:				
					if(KEY0_STATE>0&&KEY0_STATE<200)
				   { 
					   timer_switch();//ģʽ1�¶̰�KEY0���ƿ��Լ�ʱ��ͣ
					   KEY0_STATE=0;
				   }else if(KEY0_STATE>=200) {
					   timer_reset();//ģʽ1�³���KEY0���ü�ʱ
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
					Be_Chose_Plus();//ģʽ0�¶̰�KEY0��ѡλ��һ
					KEY0_STATE=170;//״̬�лأ�ʵ��������һ
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
