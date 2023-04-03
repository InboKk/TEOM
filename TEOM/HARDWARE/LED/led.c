#include "led.h"
#include "timer.h"
#include "MeasFre.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK ��ӢSTM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��PC,PE�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //LED0-->PC8�˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOC8
 GPIO_SetBits(GPIOC,GPIO_Pin_8);						 //PC8 �����

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	    		 //LED1-->PE.5 �˿�����, �������
 GPIO_Init(GPIOC, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
 GPIO_SetBits(GPIOC,GPIO_Pin_7); 						 //PE.5 ����� 
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	    		 //LED1-->PE.5 �˿�����, �������
 GPIO_Init(GPIOC, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
 GPIO_SetBits(GPIOC,GPIO_Pin_6); 						 //PE.5 ����� 
}

float f;
u8 Get_Status(void)
{
	if(LED_True == 0)
	{
		f = (float)(18000000/((float)(TIM4->ARR)+1));
		return 1;
	}
	if(LED_False == 0)
	{
		f = 0.000;
		return 2;
	}
	if(LED_Pause == 0)
	{
		f = 0.000;		
		return 0;	
	}
	
	return 2;
}

