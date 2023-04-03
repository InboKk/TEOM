#ifndef __MeasFre_H
#define __MeasFre_H	
#include "sys.h"	  
typedef struct _PID
{
	double kp,ki,kd;   //���������֣�΢��ϵ��
	float Now_error,Last_Error;//��ǰ���ϴ����
	u16 Sum_error;
	float integral,Max_Integral;//���֡������޷�
	float Output,Max_Output;//���������޷�
}PID;

float PID_SingleCalc(PID *pid,float reference,float Set_val);

void PID_Init(PID *pid,double p,double i,double d,u16 maxI,u16 maxOut);
void PID_Clear(PID *pid);
void SetFre_Init(void);
void Wait_Preheat(u16 times);

float Calculate_HZ(float f);
#endif 
