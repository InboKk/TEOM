#ifndef __MeasFre_H
#define __MeasFre_H	
#include "sys.h"	  
typedef struct _PID
{
	double kp,ki,kd;   //比例，积分，微分系数
	float Now_error,Last_Error;//当前误差、上次误差
	u16 Sum_error;
	float integral,Max_Integral;//积分、积分限幅
	float Output,Max_Output;//输出、输出限幅
}PID;

float PID_SingleCalc(PID *pid,float reference,float Set_val);

void PID_Init(PID *pid,double p,double i,double d,u16 maxI,u16 maxOut);
void PID_Clear(PID *pid);
void SetFre_Init(void);
void Wait_Preheat(u16 times);

float Calculate_HZ(float f);
#endif 
