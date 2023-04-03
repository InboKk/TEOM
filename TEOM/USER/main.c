#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "dac.h"
#include "adc.h"
#include "timer.h"
#include "MeasFre.h"
#include "dma.h"
#include "wdg.h"
PID MeasFre;  //����PID�����ṹ��
extern u8 flag;  //��/ͣ��־λ

int main(void)
{ 		
	delay_init();		  //��ʼ����ʱ����
	LED_Init();		        //��ʼ��LED�˿�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	uart_init(9600);
	Adc1_Init();
	
	Kalman_Init(); //������������ʼ��
	PID_Init(&MeasFre,0,0.0652,0,100,80); 	//PID������ʼ��  0.0152 �������ȶ�
	PID_Clear(&MeasFre);  //PID��������
	Dac2_Init();  //DAC2��ʼ��
	TIM4_Int_Init(440);    //DAC����
	DMA_DAC_Init(); //����DMA�����������,ÿ�������Ӧ��ͬ��DMAͨ��

	TIM5_Int_Init(999,7199);  //PID��������0.25s
	TIM6_Int_Init(9999,7199);  //�����ϴ�����1s 	
	
	TIM3_Int_Init(799,71);  //AD��������λ1ms
//	TIM2_Int_Init(9999,863);  //ÿ�����Ӽ��һ��ϵͳ�Ƿ���λ
	DMA_ADC1_Init() ;  //ADC1/DMA1ѭ���ɼ�����
	
	IWDG_Init(10,4000) ; //4*2^10*3000/40 = 307s 5����
	LED_False = 0;  //ֹͣ����
	
	while(1)  
	{		

		if(flag == 1)  // ������־λ
		{
			flag = 0; //��־λ��0
//			printf("��ʼ����\r\n");
			LED_False = 1;  //ֹͣ���й�
			LED_Pause = 0;  //����״̬
			//ִ����������
			TIM_Cmd(TIM4, ENABLE);  //DAC\DMA\TIM4����������
			TIM4->ARR = 42857;   //����460HZ
			SetFre_Init();  //ɨ������Ƶ��
			TIM_Cmd(TIM5, ENABLE); //��ʼPID����
			Wait_Preheat(120);  //ϵͳԤ��300s
//			TIM_Cmd(TIM2, ENABLE);  //��ʼ���ϵͳ�Ƿ���λ
			LED_Pause = 1;  //����״̬��			
			LED_True = 0;   //����״̬��			
			

		}			
		else if(flag == 2)  //ֹͣ��־λ
		{
			flag = 0;  //��־λ��0
//			printf("ֹͣ����\r\n");
		//ִ��ֹͣ����
			LED_True = 1;   //����״̬��						
			LED_False = 0;  //ֹͣ״̬��
//			TIM_Cmd(TIM2, DISABLE);
			TIM_Cmd(TIM5, DISABLE);   //ֹͣPID����	
			TIM_Cmd(TIM4, DISABLE);   //ֹͣDAC����			
			

		}
		
		
	}
} 
 

