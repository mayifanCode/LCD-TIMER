#include "beep.h"



u16 beep_count = 0;//����ʱ��
u8 beep_state = 0;//0��ʾ����
void BEEP_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��GPIOB�˿�ʱ��
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //BEEP-->PB.8 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //�ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);	 //���ݲ�����ʼ��GPIOB.8
 
 GPIO_ResetBits(GPIOB,GPIO_Pin_8);//���0���رշ��������

}
//����������
void beep_short(void)
{
	beep_count = 20;
	beep_state = 1;
	BEEP = 1;
}
//����������
void beep_long(void)
{
	beep_count = 500;
	beep_state = 1;
	BEEP = 1;
}