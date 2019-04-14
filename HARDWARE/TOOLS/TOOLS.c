#include "TOOLS.h"

//整型数求幂
int IntPower(int base, int exp)
{
    int result=1;
    while (exp!=0)
    {
        result=result*base;
        exp--;
    }
    return result;
}

//一位数字(u8)转换为字符串
void Num2Char_1bit(u8 num, u8 *a)
{
	*(a)=num+'0';
	*(a+1)='\0';
}

//两位数字(u8)转换为字符串

void Num2Char_2bits(u8 num, u8 *a, u8 mode)
{
	int digit=2;
	int i=0;
	int temp;

	for (i=0;i<digit;i++)
	{
		temp=num/IntPower(10,digit-i-1);
		*(a+i)=temp+'0';
		
		num=num%IntPower(10,digit-i-1);
	}
	*(a+digit)='\0';
}

//三位数字(u16)转换为字符串(可隐藏高位的0)
//输入：
	//num-数值		a-存储转换结果的数组首地址
		//mode=0: 高位的数值0存储为'0'
		//mode=1：高位的数值0存储为' '，即不显示
void Num2Char_3bits(u16 num,u8 *a,u8 mode)
{
	int digit=3;
	int i=0;
	int temp;

	for (i=0;i<digit;i++)
	{
		temp=num/IntPower(10,digit-i-1);
		if (i==0&&mode==1&&temp==0)
			*(a+i)=' ';
		else if (i==1&&*a==' '&&mode==1&&temp==0)
			*(a+i)=' ';
		else 
			*(a+i)=temp+'0';
		
		num=num%IntPower(10,digit-i-1);
	}
	*(a+digit)='\0';
}



