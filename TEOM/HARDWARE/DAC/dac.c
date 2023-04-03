#include "dac.h"
#include "sys.h"

//#define DAC_DHR12R2	 0x40007414
void Dac2_Init()     // STM32F103 PA5_DAC2
{
    GPIO_InitTypeDef GPIO_InitStructure;
    DAC_InitTypeDef DAC_InitType;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // GPIOAʱ��ʹ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE); // DACʱ��ʹ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; // ����Ϊģ������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_SetBits(GPIOA, GPIO_Pin_5);
		GPIO_Init(GPIOA, &GPIO_InitStructure);
    DAC_DeInit(); // ��ʼ��
    DAC_StructInit(&DAC_InitType); // ��ʼ��
	
    DAC_InitType.DAC_Trigger = DAC_Trigger_T4_TRGO; // ʹ�ô������ܣ���ʱ��4���� ��2��4��5��6��7
	
    DAC_InitType.DAC_WaveGeneration = DAC_WaveGeneration_None; // ��ʹ���Դ��Ĳ������ɹ���
    DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0; // ͨ�����Է�����λ�Ĵ�������α����������DAC_WaveGeneration����ΪDAC_WaveGeneration_Noiseʱ��Ч
    DAC_InitType.DAC_OutputBuffer = DAC_OutputBuffer_Enable; //ʹ��������� 
    DAC_Init(DAC_Channel_2, &DAC_InitType); // ��ʼ��DAC

		DAC_Cmd(DAC_Channel_2, ENABLE); // ʹ��DAC
		DAC_DMACmd(DAC_Channel_2, ENABLE); //ʹ��DACͨ��2��DMA  
//		DAC_SetChannel2Data(DAC_Align_12b_R, 0); // ���0V
}

