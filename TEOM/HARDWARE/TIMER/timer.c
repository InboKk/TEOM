#include "timer.h"
#include "dac.h"
#include "adc.h"
#include "usart.h"
#include "MeasFre.h"
#include "led.h"
#include "dma.h"
#include "sys.h"
#include "wdg.h"
#define HZ(x) (u16)(18000000/(x+1))  //分频，最低276Hz

extern u16 ADC_ConvertedValue[2];
//定时器4，方波发生。
void TIM4_Int_Init(u16 Hz)
{
	    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
			TIM_DeInit(TIM4); // 初始化
      TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
      TIM_TimeBaseStructure.TIM_Period = HZ(Hz) ;                       
      TIM_TimeBaseStructure.TIM_Prescaler = 1;                 
      TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  
      TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	
      TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
      TIM_SelectOutputTrigger(TIM4, TIM_TRGOSource_Update);//设置TIM2输出触发为更新模式
	

}
//定时器5，PID控制器。

void TIM5_Int_Init(u32 arr,u16 pre)   //TIM3的触发输出无法触发DAC ，只有软件，TIM2 4 5 6 7  
{
	    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
			NVIC_InitTypeDef NVIC_InitStructure;
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	
			TIM_DeInit(TIM5); // 初始化
      TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
      TIM_TimeBaseStructure.TIM_Period = arr ;                       
      TIM_TimeBaseStructure.TIM_Prescaler =pre;                 
      TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  
      TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
      TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	
      TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE); // 允许更新中断
	
	    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
	
			
}

//extern Kalman kfp;  //定义卡尔曼参数结构体
extern PID MeasFre;
extern u16 Kalman_data;

void TIM5_IRQHandler(void)
{

	float output;
    if(TIM_GetITStatus(TIM5,TIM_IT_Update)!=RESET)
    {
        TIM_ClearITPendingBit(TIM5,TIM_IT_Update);
			
    }	

		output = PID_SingleCalc(&MeasFre,2145,Kalman_data);  //增量PID计算 1.729/3.295=2151.4
		
		if(output>0)  
		{
			TIM4->ARR = (TIM4->ARR + output);
		}
		else
		{
			TIM4->ARR = (TIM4->ARR + output);
		}		
		
}


//定时器6，上传数据定时器。

void TIM6_Int_Init(u32 arr,u16 pre)   //TIM3的触发输出无法触发DAC ，只有软件，TIM2 4 5 6 7  
{
	    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
			NVIC_InitTypeDef NVIC_InitStructure;
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	
			TIM_DeInit(TIM6); // 初始化
      TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
      TIM_TimeBaseStructure.TIM_Period = arr ;                       
      TIM_TimeBaseStructure.TIM_Prescaler =pre;                 
      TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  
      TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
      TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
	
      TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE); // 允许更新中断
	
	    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
	
			TIM_Cmd(TIM6, ENABLE);  //定时器使能开启
			
}
u8 Status;
extern float f;
void TIM6_IRQHandler(void) //TIM3 中断
{

	u8 Check = 0;  //校验位

		if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET) //检查 TIM3 更新中断发生与否
		{
				TIM_ClearITPendingBit(TIM6, TIM_IT_Update ); //清除 TIM3 更新中断标志
				
				Status = Get_Status();
//				printf( "%d,%d,%.3f,%d\r\n",ADC_ConvertedValue[1],Kalman_data,f,Check);  //  状态位/频率/0
				printf( "%d,%.3f,%d\r\n",Status,f,Check);  //  状态位/频率/0
				IWDG_Feed();
		}
		
}

void TIM3_Int_Init(u32 arr,u16 pre)   //TIM3的触发输出无法触发DAC ，只有软件，TIM2 4 5 6 7  
{
	    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
			TIM_DeInit(TIM3); // 初始化
      TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
      TIM_TimeBaseStructure.TIM_Period = arr ;                       
      TIM_TimeBaseStructure.TIM_Prescaler =pre;                 
      TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  
      TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
      TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
			
	    TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);//设置TIM2输出触发为更新模式	



			TIM_Cmd(TIM3, ENABLE);	
}


//检测系统是否运行在共振频率
void TIM2_Int_Init(u32 arr,u16 pre)   //TIM3的触发输出无法触发DAC ，只有软件，TIM2 4 5 6 7  
{
	    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
			NVIC_InitTypeDef NVIC_InitStructure;
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
			TIM_DeInit(TIM2); // 初始化
      TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
      TIM_TimeBaseStructure.TIM_Period = arr ;                       
      TIM_TimeBaseStructure.TIM_Prescaler =pre;                 
      TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  
      TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
      TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
      TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); // 允许更新中断
	
	    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
	
//			TIM_Cmd(TIM2, ENABLE);  //定时器使能开启
			
}

extern u8 flag;
void TIM2_IRQHandler(void) //TIM2 中断
{

		if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //检查 TIM2 更新中断发生与否
		{
				TIM_ClearITPendingBit(TIM2, TIM_IT_Update ); //清除 TIM2 更新中断标志
				
//				if(ADC_ConvertedValue[0] < 3000)
//				{
//					
//					flag = 3;  //系统重新扫频
//				}
				
				IWDG_Feed();
		}
		
}
