#include "DISPLAY.h"
#include "WATCH_FUCTION.h"
#include "BEEP.h"
#include "lcd.h"
#include "led.h"
#include "stm32f10x.h"

u8 timer_minutes;//����
u8 timer_seconds;//����

u8 timer_state=0; //0:��ͣ  1����ʱ
u8 order_count = 0;// 0:˳ʱ��,�ۼ�  1����ʱ�ƣ��ۼ�
u8 timer_display_mode = 0;//��ʾģʽ 0����ֹ 1��ѭ������ 2��ѭ������ 3��ѭ������ 4��ѭ������
u16 words_point_x=68;//�ַ����Ͻǵĺ�����
u16 words_point_y=225;//�ַ����Ͻǵ�������
u8 test_state = 0;// 0 ��δ��ʼ�� 1�����Կ�ʼ��1���ӣ� 2�����Ծ�1�� 3�����Ծ�2�� 4�����Խ������һ����

u8 power_state = 0;//0:����״̬  1���ػ�״̬
u16 power_count;  //�����͹ػ�ʱ��
u8 lock; //0:δ��ס 1����ס
//��鲢��������״̬
void check_test_state(void)
{
	int test_last;
	if(order_count == 0) {
		test_last = timer_minutes * 60 + timer_seconds;
	}else {
		test_last = 60*60 + 59 - timer_minutes * 60 - timer_seconds;
	}
	if(test_last == 0 && test_state != 0) {
		LCD_Fill(0,175,240,320,BLACK);
		test_state = 0;
	}else if(test_last > 0 && test_last <= 59 && test_state != 1) {
		beep_long();
		LCD_Fill(0,175,240,320,BLACK);
		test_state = 1;
	}else if(test_last > 59 && test_last <= 2399 && test_state != 2) {
		LCD_Fill(0,175,240,320,BLACK);
		test_state = 2;
	}else if(test_last > 2399 && test_last <= 3599 && test_state != 3) {
		LCD_Fill(0,175,240,320,BLACK);
		test_state = 3;
	}else if(test_last >3599 && test_last <= 3659 && test_state != 4) {
		LCD_Fill(0,175,240,320,BLACK);
		beep_long();
		test_state = 4;
	}
}

//��ʱ��һ
void timer_count (void)
{	
	//�ж�˳����
	if(order_count == 0) {
       if(timer_minutes == 60 && timer_seconds == 59) {
		   timer_minutes = 60;
		   timer_seconds = 59;
	   }else if(timer_minutes != 60 && timer_seconds == 59) {
		   timer_minutes ++;
		   timer_seconds = 0;
	   }else if(timer_seconds != 59) {
		   timer_seconds ++;
	   }	
	}else {
		if(timer_minutes ==0 && timer_seconds ==0) {
			timer_minutes =0;
			timer_seconds =0;
		}else if(timer_minutes != 0 && timer_seconds ==0) {
			timer_minutes --;
			timer_seconds = 59;
		}else if(timer_seconds != 0) {
			timer_seconds --;
		}
	}
    check_test_state();	
}



//��ͣ��ʱ
void timer_stop(void)
{
	TIM_ITConfig(TIM4,TIM_IT_Update,DISABLE );
	timer_state = 0;
}
//������ʱ
void timer_start(void)
{
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	timer_state = 1;	
}
//�л���ʱ��ͣ
void timer_switch(void)
{
	if(timer_state == 0) {
		timer_start();
	}else {
		timer_stop();
	}
}
//���ü�ʱ��
void timer_reset(void)
{
	TIM_ITConfig(TIM4,TIM_IT_Update,DISABLE);
	timer_state = 0;
	test_state = 0;
	if(order_count == 0) {
        timer_minutes = 0;
	    timer_seconds = 0;
	}else {
		timer_minutes = 60;
	    timer_seconds = 59;
	}
    LCD_Fill(0,175,240,320,BLACK);//�ڱ�
}
//�л�˳����ģʽ
void order_mode_switch(void)
{
	u16 count;
	u16 de_count;
	if(order_count == 0) {
		order_count = 1;
	}else {
		order_count = 0;
	}
	count = timer_minutes * 60 + timer_seconds;
	de_count = 60 * 60 + 59 - count;
    timer_minutes = de_count / 60;
	timer_seconds = de_count - (de_count / 60) * 60;
}
//�л���ʾ������ģʽ
void display_mode_change(void)
{
	timer_display_mode ++;
	if(timer_display_mode == 5) {
		timer_display_mode = 0;
	}
	LCD_Fill(0,175,240,320,BLACK);
	words_point_x = 68;
	words_point_y = 225;
}
//�ػ�
void power_off(void)
{
	LED0 = 1;
	LED1 = 1;
	timer_reset();
	LCD_Clear(BLACK);
	LCD_LED = 0;
}

//����
void power_on(void) 
{
    lock = 0;
	LCD_LED = 1;
}
//���ػ��л�
void power_switch(void) 
{
    if(power_state == 0) {
		lock = 1;
		LCD_Clear(BLACK);
        Show_Str(20,150,200,24,"���ڹػ�����ȴ�",24,0);
		power_count = 500;
		power_state = 1;
	}else {
        power_on();
		Show_Str(20,150,200,24,"���ڿ�������ȴ�",24,0);		
		power_count = 500;
		power_state = 0;
	}
}
//�޸ı���λ�ã�ʵ��������
void change_timer_words_position(void)
{
	//��ͬ��ʾģʽ��ͬ����
	switch(timer_display_mode) {
		case 0:
			
			break;
	    case 1:
			words_point_x -- ;
		    if(words_point_x == 0) {
				words_point_x = 180;
				LCD_Fill(0,175,240,320,BLACK);//���˵���һ���ڱ�
			break;
		case 2:
			words_point_x ++;
		    if(words_point_x > 180) {
				words_point_x = 0;
				LCD_Fill(0,175,240,320,BLACK);
			}
			break;
		case 3:
			words_point_y --;
		    if(words_point_y < 185) {
				words_point_y = 265;
				LCD_Fill(0,175,240,320,BLACK);
			}
			break;
		case 4:
			words_point_y ++;
		    if(words_point_y > 265) {
				words_point_y = 185;
				LCD_Fill(0,175,240,320,BLACK);
			}
			break;
		default:
			break;
	}
}



}