#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
//#define HZ(x) (u16)(18000000/(x+1))  //��Ƶ�����276Hz
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   


void TIM4_Int_Init(u16 Hz);  //����
void TIM5_Int_Init(u32 arr,u16 pre);  //PID
void TIM6_Int_Init(u32 arr,u16 pre) ;  //�����ϴ���ʱ��
void TIM3_Int_Init(u32 arr,u16 pre) ; //ADC������ʱ��
void TIM2_Int_Init(u32 arr,u16 pre) ;  //���Vrms��ʱ��
#endif
