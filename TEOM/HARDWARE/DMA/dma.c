#include "dma.h"
#include "adc.h"
#define    ADC1_DR_Address ((u32)0x40012400+0x4c)

//DMA_InitTypeDef DMA_InitStructure;
u16 ADC_ConvertedValue[2] ;   //0-14;1-15  存放ADC循环采集的数据
    

//  DMA_DAC2 方波
u32 Fangbo[2] = {1241,0};  //0.6VPP
void DMA_DAC_Init() // cpar为外设地址，cmar为内存地址，cndtr为搬运的数据个数（对应于数组长度）
{
    DMA_InitTypeDef DMA_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE); // 重要，很多时候只顾着改DMA通道而忘记使能对应时钟，导致DMA不工作

    DMA_DeInit(DMA2_Channel4); // 初始化  DAC通道

    DMA_StructInit(&DMA_InitStructure); // 初始化
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST; // 方向：从内存到外设
    DMA_InitStructure.DMA_BufferSize = 2; // 指定每轮DMA需要搬运的数据个数
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; // 外设地址不变（固定为DAC寄存器地址）
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; // 内存地址自增（遍历数组）
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // 外设数据长度为16bit
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; // 内存数据长度为16bit
    DMA_InitStructure.DMA_Priority = DMA_Priority_High; // 设置为高优先级（使用单个DMA时无影响）
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; // 非内存到内存模式
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; // 循环
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&DAC->DHR12R2; // 外设起始地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)Fangbo; // 内存起始地址

    DMA_Init(DMA2_Channel4, &DMA_InitStructure);
    DMA_Cmd(DMA2_Channel4, ENABLE); // 使能
}


void DMA_ADC1_Init(void)
{
		DMA_InitTypeDef DMA_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
		DMA_DeInit(DMA1_Channel1);  								//复位
		DMA_StructInit(&DMA_InitStructure); // 初始化
	
    DMA_InitStructure.DMA_PeripheralBaseAddr=ADC1_DR_Address;	//外设地址，这里就是adc1的地址
    DMA_InitStructure.DMA_MemoryBaseAddr=(u32)&ADC_ConvertedValue;//存储器地址
    DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;//传输方向，外设-->内存

    DMA_InitStructure.DMA_BufferSize=2;	//存储数据大小
    DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;	//外设地址不增量
    DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;	//内存地址增量

    DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;   //外设数据宽度，半字16位
    DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord; //外设与内存数据宽度应为一样
    DMA_InitStructure.DMA_Mode=DMA_Mode_Circular;//DMA传输模式为循环传输

    DMA_InitStructure.DMA_Priority=DMA_Priority_Medium;	//通道优先级为中
    DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;	//关闭从存储器到存储器模式
    DMA_Init(DMA1_Channel1,&DMA_InitStructure);	

		DMA_ITConfig(DMA1_Channel1,DMA_IT_TC, ENABLE);		//使能传输完成中断

		NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		DMA_Cmd(DMA1_Channel1, ENABLE); // 使能

}
u16 Kalman_data;
extern Kalman kfp;  //定义卡尔曼参数结构体

void  DMA1_Channel1_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC1)!=RESET)
	{
		
//		Kalman_data = KalmanFilter(&kfp,ADC_ConvertedValue[1]);  //卡尔曼滤波采集AD值
		Kalman_data = FIR_Filter(ADC_ConvertedValue[1]);
		DMA_ClearITPendingBit(DMA1_IT_TC1);
//		ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
	}
}

























