#include "beep.h"



u16 beep_count = 0;//控制时长
u8 beep_state = 0;//0表示不响
void BEEP_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能GPIOB端口时钟
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //BEEP-->PB.8 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);	 //根据参数初始化GPIOB.8
 
 GPIO_ResetBits(GPIOB,GPIO_Pin_8);//输出0，关闭蜂鸣器输出

}
//蜂鸣器短鸣
void beep_short(void)
{
	beep_count = 20;
	beep_state = 1;
	BEEP = 1;
}
//蜂鸣器长鸣
void beep_long(void)
{
	beep_count = 500;
	beep_state = 1;
	BEEP = 1;
}