#include "DISPLAY.h"
#include "WATCH_FUCTION.h"
#include "BEEP.h"
#include "lcd.h"
#include "led.h"
#include "stm32f10x.h"

u8 timer_minutes;//分钟
u8 timer_seconds;//秒钟

u8 timer_state=0; //0:暂停  1：走时
u8 order_count = 0;// 0:顺时计,累加  1：逆时计，累减
u8 timer_display_mode = 0;//显示模式 0：静止 1：循环左移 2：循环右移 3：循环上移 4：循环下移
u16 words_point_x=68;//字符左上角的横坐标
u16 words_point_y=225;//字符左上角的纵坐标
u8 test_state = 0;// 0 ：未开始； 1：考试开始的1分钟； 2：答试卷1； 3：答试卷2； 4：考试结束最后一分钟

u8 power_state = 0;//0:开机状态  1：关机状态
u16 power_count;  //开机和关机时间
u8 lock; //0:未锁住 1：锁住
//检查并调整考试状态
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

//计时加一
void timer_count (void)
{	
	//判断顺倒计
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



//暂停计时
void timer_stop(void)
{
	TIM_ITConfig(TIM4,TIM_IT_Update,DISABLE );
	timer_state = 0;
}
//启动计时
void timer_start(void)
{
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	timer_state = 1;	
}
//切换计时启停
void timer_switch(void)
{
	if(timer_state == 0) {
		timer_start();
	}else {
		timer_stop();
	}
}
//重置计时器
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
    LCD_Fill(0,175,240,320,BLACK);//遮蔽
}
//切换顺倒计模式
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
//切换显示跑马灯模式
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
//关机
void power_off(void)
{
	LED0 = 1;
	LED1 = 1;
	timer_reset();
	LCD_Clear(BLACK);
	LCD_LED = 0;
}

//开机
void power_on(void) 
{
    lock = 0;
	LCD_LED = 1;
}
//开关机切换
void power_switch(void) 
{
    if(power_state == 0) {
		lock = 1;
		LCD_Clear(BLACK);
        Show_Str(20,150,200,24,"正在关机，请等待",24,0);
		power_count = 500;
		power_state = 1;
	}else {
        power_on();
		Show_Str(20,150,200,24,"正在开机，请等待",24,0);		
		power_count = 500;
		power_state = 0;
	}
}
//修改标语位置，实现跑马灯
void change_timer_words_position(void)
{
	//不同显示模式不同处理
	switch(timer_display_mode) {
		case 0:
			
			break;
	    case 1:
			words_point_x -- ;
		    if(words_point_x == 0) {
				words_point_x = 180;
				LCD_Fill(0,175,240,320,BLACK);//到端点做一次遮蔽
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
