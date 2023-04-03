 #include "MeasFre.h"
 #include "sys.h"   //库函数
 #include "dac.h"    //引入TIM使用
 #include "adc.h"
 #include "delay.h"
 #include "stdio.h"
 #include "usart.h"
 #include "led.h"
 #include "dma.h"
 
 #define LIMIT(x,min,max) (x)=(((x)<=(min))?(min):(((x)>=(max))?(max):(x)))


float PID_SingleCalc(PID *pid,float reference,float Set_val)
{

	pid->Now_error=reference - Set_val;//计算当前误差 
		
//	pid->integral = pid->Sum_error*pid->ki;//计算积分累加
//	LIMIT(pid->integral,-pid->Max_Integral,pid->Max_Integral);//积分限幅	
	pid->Output = (pid->Now_error - pid->Last_Error) * pid->kp + pid->ki* pid->Now_error;  //计算比例项和微分项

	LIMIT(pid->Output,-pid->Max_Output,pid->Max_Output);		//输出限幅

	pid->Last_Error = pid->Now_error;
	return pid->Output;
}
/*************************************************************
Function  :  PID_Init
Description : PID参数初始化
Input  :     结构参数
return  :   
*************************************************************/ 
void PID_Init(PID *pid,double p,double i,double d,u16 maxI,u16 maxOut)
{
	pid->kp=p;
	pid->ki=i;
	pid->kd=d;
	pid->Max_Integral=maxI;  //积分限幅
	pid->Max_Output=maxOut;   //输出限幅
}
/*************************************************************
Function  :  PID_Clear
Description : 清空PID历史参数
Input  :     结构参数
return  :   
*************************************************************/ 
void PID_Clear(PID *pid)
{
	pid->Now_error=0;
	pid->Last_Error=0;
	pid->integral=0;
	pid->Output=0;
	pid->Sum_error = 0;

}

/*************************************************************
Function  :  SetFre_Init
Description : 扫描起振频率(乘法器零点)
Input  :     起始频率
return  :   
*************************************************************/ 

extern u16 ADC_ConvertedValue[2];
void SetFre_Init()
{
	u16 Vrms[100];  //储存乘法器输出数据
	u8 i,j;
	u8 count = 0;
	u16 Max_Vrms = 0;

	for(i = 0;i <= 100;i++)   //循环采集乘法器数据 存入数组
	{
		TIM4->ARR = (TIM4->ARR - 14);
		delay_ms(500);
		Vrms[i] = ADC_ConvertedValue[0];

		
	}	
	for(j = 0;j <= 100;j++)  //找出最大值及其下标
	{
		if(Max_Vrms < Vrms[j])
		{
			Max_Vrms = Vrms[j];
			count = j;
		}
	}
	TIM4->ARR = (TIM4->ARR +1400 - 14*count );
	
		
}


void Wait_Preheat(u16 times)  //预热
{
	u16 i;
	for(i = 0;i <= times;i++)
	{
		delay_ms(1000); //1s等待
	}
}

float Calculate_HZ(float f)
{
		float f_average,f1 = 0;
		u8 i = 0;
		for(i = 0;i<=10;i++)
		{
			f1 = f;
			
			f_average += f1;
		}
		return f_average/10;
}
