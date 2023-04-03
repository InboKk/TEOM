#include "dma.h"
#include "adc.h"
#define    ADC1_DR_Address ((u32)0x40012400+0x4c)

//DMA_InitTypeDef DMA_InitStructure;
u16 ADC_ConvertedValue[2] ;   //0-14;1-15  ���ADCѭ���ɼ�������
    

//  DMA_DAC2 ����
u32 Fangbo[2] = {1241,0};  //0.6VPP
void DMA_DAC_Init() // cparΪ�����ַ��cmarΪ�ڴ��ַ��cndtrΪ���˵����ݸ�������Ӧ�����鳤�ȣ�
{
    DMA_InitTypeDef DMA_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE); // ��Ҫ���ܶ�ʱ��ֻ���Ÿ�DMAͨ��������ʹ�ܶ�Ӧʱ�ӣ�����DMA������

    DMA_DeInit(DMA2_Channel4); // ��ʼ��  DACͨ��

    DMA_StructInit(&DMA_InitStructure); // ��ʼ��
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST; // ���򣺴��ڴ浽����
    DMA_InitStructure.DMA_BufferSize = 2; // ָ��ÿ��DMA��Ҫ���˵����ݸ���
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; // �����ַ���䣨�̶�ΪDAC�Ĵ�����ַ��
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; // �ڴ��ַ�������������飩
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // �������ݳ���Ϊ16bit
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; // �ڴ����ݳ���Ϊ16bit
    DMA_InitStructure.DMA_Priority = DMA_Priority_High; // ����Ϊ�����ȼ���ʹ�õ���DMAʱ��Ӱ�죩
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; // ���ڴ浽�ڴ�ģʽ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; // ѭ��
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&DAC->DHR12R2; // ������ʼ��ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)Fangbo; // �ڴ���ʼ��ַ

    DMA_Init(DMA2_Channel4, &DMA_InitStructure);
    DMA_Cmd(DMA2_Channel4, ENABLE); // ʹ��
}


void DMA_ADC1_Init(void)
{
		DMA_InitTypeDef DMA_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
		DMA_DeInit(DMA1_Channel1);  								//��λ
		DMA_StructInit(&DMA_InitStructure); // ��ʼ��
	
    DMA_InitStructure.DMA_PeripheralBaseAddr=ADC1_DR_Address;	//�����ַ���������adc1�ĵ�ַ
    DMA_InitStructure.DMA_MemoryBaseAddr=(u32)&ADC_ConvertedValue;//�洢����ַ
    DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;//���䷽������-->�ڴ�

    DMA_InitStructure.DMA_BufferSize=2;	//�洢���ݴ�С
    DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;	//�����ַ������
    DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;	//�ڴ��ַ����

    DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;   //�������ݿ�ȣ�����16λ
    DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord; //�������ڴ����ݿ��ӦΪһ��
    DMA_InitStructure.DMA_Mode=DMA_Mode_Circular;//DMA����ģʽΪѭ������

    DMA_InitStructure.DMA_Priority=DMA_Priority_Medium;	//ͨ�����ȼ�Ϊ��
    DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;	//�رմӴ洢�����洢��ģʽ
    DMA_Init(DMA1_Channel1,&DMA_InitStructure);	

		DMA_ITConfig(DMA1_Channel1,DMA_IT_TC, ENABLE);		//ʹ�ܴ�������ж�

		NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		DMA_Cmd(DMA1_Channel1, ENABLE); // ʹ��

}
u16 Kalman_data;
extern Kalman kfp;  //���忨���������ṹ��

void  DMA1_Channel1_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC1)!=RESET)
	{
		
//		Kalman_data = KalmanFilter(&kfp,ADC_ConvertedValue[1]);  //�������˲��ɼ�ADֵ
		Kalman_data = FIR_Filter(ADC_ConvertedValue[1]);
		DMA_ClearITPendingBit(DMA1_IT_TC1);
//		ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
	}
}

























