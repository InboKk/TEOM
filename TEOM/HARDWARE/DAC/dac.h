#ifndef __DAC_H
#define __DAC_H	 
#include "sys.h"	    
//#define DAC_DHR12RD_ADDRESS		(DAC_BASE + 0x20)		
#define HZ(x) (u16)(18000000/(x+1))  //��Ƶ�����276Hz
 //////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//DAC ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/8
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
								    
//u16 Fangbo[2] = {1300,0};
void Dac2_Init(void);

void MYDMA_Config(void) ;
#endif

















