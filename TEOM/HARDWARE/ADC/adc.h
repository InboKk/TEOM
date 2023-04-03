#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"

void Adc1_Init(void);

u16  Get_Adc(u8 ch); 
u16 Get_Adc_Average(u8 ch,u8 times); 

typedef struct     //�����������ṹ��
{
    float Last_P;//�ϴι���Э���� ������Ϊ0 !
    float Now_P;//��ǰ����Э����
    float out;//�������˲������
    float Kg;//����������
    float Q;//��������Э����
    float R;//�۲�����Э����
		float A;//״̬ת�ƾ���
}Kalman;

void Kalman_Init(void);   //������������ʼ��
float KalmanFilter(Kalman *kfp,float input);   //�������˲���
void Wait_KalmanFilter(u8 times);

u16 FIR_Filter(u16 input);
u16 PhaseADC_Average(u8 times);
extern Kalman kfp;

#endif 
