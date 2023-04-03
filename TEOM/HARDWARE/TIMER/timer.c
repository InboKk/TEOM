#include "timer.h"
#include "dac.h"
#include "adc.h"
#include "usart.h"
#include "MeasFre.h"
#include "led.h"
#include "dma.h"
#include "sys.h"
#include "wdg.h"
#define HZ(x) (u16)(18000000/(x+1))  //��Ƶ�����276Hz

extern u16 ADC_ConvertedValue[2];
//��ʱ��4������������
void TIM4_Int_Init(u16 Hz)
{
	    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
			TIM_DeInit(TIM4); // ��ʼ��
      TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
      TIM_TimeBaseStructure.TIM_Period = HZ(Hz) ;                       
      TIM_TimeBaseStructure.TIM_Prescaler = 1;                 
      TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  
      TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	
      TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
      TIM_SelectOutputTrigger(TIM4, TIM_TRGOSource_Update);//����TIM2�������Ϊ����ģʽ
	

}
//��ʱ��5��PID��������

void TIM5_Int_Init(u32 arr,u16 pre)   //TIM3�Ĵ�������޷�����DAC ��ֻ�������TIM2 4 5 6 7  
{
	    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
			NVIC_InitTypeDef NVIC_InitStructure;
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	
			TIM_DeInit(TIM5); // ��ʼ��
      TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
      TIM_TimeBaseStructure.TIM_Period = arr ;                       
      TIM_TimeBaseStructure.TIM_Prescaler =pre;                 
      TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  
      TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
      TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	
      TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE); // ��������ж�
	
	    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
	
			
}

//extern Kalman kfp;  //���忨���������ṹ��
extern PID MeasFre;
extern u16 Kalman_data;

void TIM5_IRQHandler(void)
{

	float output;
    if(TIM_GetITStatus(TIM5,TIM_IT_Update)!=RESET)
    {
        TIM_ClearITPendingBit(TIM5,TIM_IT_Update);
			
    }	

		output = PID_SingleCalc(&MeasFre,2145,Kalman_data);  //����PID���� 1.729/3.295=2151.4
		
		if(output>0)  
		{
			TIM4->ARR = (TIM4->ARR + output);
		}
		else
		{
			TIM4->ARR = (TIM4->ARR + output);
		}		
		
}


//��ʱ��6���ϴ����ݶ�ʱ����

void TIM6_Int_Init(u32 arr,u16 pre)   //TIM3�Ĵ�������޷�����DAC ��ֻ�������TIM2 4 5 6 7  
{
	    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
			NVIC_InitTypeDef NVIC_InitStructure;
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	
			TIM_DeInit(TIM6); // ��ʼ��
      TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
      TIM_TimeBaseStructure.TIM_Period = arr ;                       
      TIM_TimeBaseStructure.TIM_Prescaler =pre;                 
      TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  
      TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
      TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
	
      TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE); // ��������ж�
	
	    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
	
			TIM_Cmd(TIM6, ENABLE);  //��ʱ��ʹ�ܿ���
			
}
u8 Status;
extern float f;
void TIM6_IRQHandler(void) //TIM3 �ж�
{

	u8 Check = 0;  //У��λ

		if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET) //��� TIM3 �����жϷ������
		{
				TIM_ClearITPendingBit(TIM6, TIM_IT_Update ); //��� TIM3 �����жϱ�־
				
				Status = Get_Status();
//				printf( "%d,%d,%.3f,%d\r\n",ADC_ConvertedValue[1],Kalman_data,f,Check);  //  ״̬λ/Ƶ��/0
				printf( "%d,%.3f,%d\r\n",Status,f,Check);  //  ״̬λ/Ƶ��/0
				IWDG_Feed();
		}
		
}

void TIM3_Int_Init(u32 arr,u16 pre)   //TIM3�Ĵ�������޷�����DAC ��ֻ�������TIM2 4 5 6 7  
{
	    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
			TIM_DeInit(TIM3); // ��ʼ��
      TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
      TIM_TimeBaseStructure.TIM_Period = arr ;                       
      TIM_TimeBaseStructure.TIM_Prescaler =pre;                 
      TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  
      TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
      TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
			
	    TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);//����TIM2�������Ϊ����ģʽ	



			TIM_Cmd(TIM3, ENABLE);	
}


//���ϵͳ�Ƿ������ڹ���Ƶ��
void TIM2_Int_Init(u32 arr,u16 pre)   //TIM3�Ĵ�������޷�����DAC ��ֻ�������TIM2 4 5 6 7  
{
	    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
			NVIC_InitTypeDef NVIC_InitStructure;
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
			TIM_DeInit(TIM2); // ��ʼ��
      TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
      TIM_TimeBaseStructure.TIM_Period = arr ;                       
      TIM_TimeBaseStructure.TIM_Prescaler =pre;                 
      TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  
      TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
      TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
      TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); // ��������ж�
	
	    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
	
//			TIM_Cmd(TIM2, ENABLE);  //��ʱ��ʹ�ܿ���
			
}

extern u8 flag;
void TIM2_IRQHandler(void) //TIM2 �ж�
{

		if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //��� TIM2 �����жϷ������
		{
				TIM_ClearITPendingBit(TIM2, TIM_IT_Update ); //��� TIM2 �����жϱ�־
				
//				if(ADC_ConvertedValue[0] < 3000)
//				{
//					
//					flag = 3;  //ϵͳ����ɨƵ
//				}
				
				IWDG_Feed();
		}
		
}
