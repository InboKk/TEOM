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
PID MeasFre;  //定义PID参数结构体
extern u8 flag;  //启/停标志位

int main(void)
{ 		
	delay_init();		  //初始化延时函数
	LED_Init();		        //初始化LED端口
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	uart_init(9600);
	Adc1_Init();
	
	Kalman_Init(); //卡尔曼参数初始化
	PID_Init(&MeasFre,0,0.0652,0,100,80); 	//PID参数初始化  0.0152 不抽气稳定
	PID_Clear(&MeasFre);  //PID参数清零
	Dac2_Init();  //DAC2初始化
	TIM4_Int_Init(440);    //DAC方波
	DMA_DAC_Init(); //开启DMA传输输出方波,每个外设对应不同的DMA通道

	TIM5_Int_Init(999,7199);  //PID控制周期0.25s
	TIM6_Int_Init(9999,7199);  //数据上传周期1s 	
	
	TIM3_Int_Init(799,71);  //AD采样周期位1ms
//	TIM2_Int_Init(9999,863);  //每两分钟检测一次系统是否在位
	DMA_ADC1_Init() ;  //ADC1/DMA1循环采集数据
	
	IWDG_Init(10,4000) ; //4*2^10*3000/40 = 307s 5分钟
	LED_False = 0;  //停止运行
	
	while(1)  
	{		

		if(flag == 1)  // 启动标志位
		{
			flag = 0; //标志位置0
//			printf("开始启动\r\n");
			LED_False = 1;  //停止运行关
			LED_Pause = 0;  //过渡状态
			//执行启动程序
			TIM_Cmd(TIM4, ENABLE);  //DAC\DMA\TIM4发出方波、
			TIM4->ARR = 42857;   //起步在460HZ
			SetFre_Init();  //扫描起振频率
			TIM_Cmd(TIM5, ENABLE); //开始PID控制
			Wait_Preheat(120);  //系统预热300s
//			TIM_Cmd(TIM2, ENABLE);  //开始检测系统是否在位
			LED_Pause = 1;  //过度状态关			
			LED_True = 0;   //运行状态开			
			

		}			
		else if(flag == 2)  //停止标志位
		{
			flag = 0;  //标志位置0
//			printf("停止运行\r\n");
		//执行停止程序
			LED_True = 1;   //运行状态关						
			LED_False = 0;  //停止状态开
//			TIM_Cmd(TIM2, DISABLE);
			TIM_Cmd(TIM5, DISABLE);   //停止PID控制	
			TIM_Cmd(TIM4, DISABLE);   //停止DAC方波			
			

		}
		
		
	}
} 
 

