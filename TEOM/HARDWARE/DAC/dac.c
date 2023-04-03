#include "dac.h"
#include "sys.h"

//#define DAC_DHR12R2	 0x40007414
void Dac2_Init()     // STM32F103 PA5_DAC2
{
    GPIO_InitTypeDef GPIO_InitStructure;
    DAC_InitTypeDef DAC_InitType;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // GPIOA时钟使能
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE); // DAC时钟使能

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; // 配置为模拟输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_SetBits(GPIOA, GPIO_Pin_5);
		GPIO_Init(GPIOA, &GPIO_InitStructure);
    DAC_DeInit(); // 初始化
    DAC_StructInit(&DAC_InitType); // 初始化
	
    DAC_InitType.DAC_Trigger = DAC_Trigger_T4_TRGO; // 使用触发功能，定时器4触发 ，2，4，5，6，7
	
    DAC_InitType.DAC_WaveGeneration = DAC_WaveGeneration_None; // 不使用自带的波形生成功能
    DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0; // 通过线性反馈移位寄存器生成伪噪声，仅当DAC_WaveGeneration配置为DAC_WaveGeneration_Noise时有效
    DAC_InitType.DAC_OutputBuffer = DAC_OutputBuffer_Enable; //使能输出缓存 
    DAC_Init(DAC_Channel_2, &DAC_InitType); // 初始化DAC

		DAC_Cmd(DAC_Channel_2, ENABLE); // 使能DAC
		DAC_DMACmd(DAC_Channel_2, ENABLE); //使能DAC通道2的DMA  
//		DAC_SetChannel2Data(DAC_Align_12b_R, 0); // 输出0V
}

