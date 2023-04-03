#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"

void Adc1_Init(void);

u16  Get_Adc(u8 ch); 
u16 Get_Adc_Average(u8 ch,u8 times); 

typedef struct     //卡尔曼参数结构体
{
    float Last_P;//上次估算协方差 不可以为0 !
    float Now_P;//当前估算协方差
    float out;//卡尔曼滤波器输出
    float Kg;//卡尔曼增益
    float Q;//过程噪声协方差
    float R;//观测噪声协方差
		float A;//状态转移矩阵
}Kalman;

void Kalman_Init(void);   //卡尔曼参数初始化
float KalmanFilter(Kalman *kfp,float input);   //卡尔曼滤波器
void Wait_KalmanFilter(u8 times);

u16 FIR_Filter(u16 input);
u16 PhaseADC_Average(u8 times);
extern Kalman kfp;

#endif 
