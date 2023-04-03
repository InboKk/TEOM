 #include "adc.h"
 #include "delay.h"
		   
#define q  0.0252   //a = fp*2*pi*T = 10*2*3.14*0.001 = 0.0252  0.0628 15AD



void  Adc1_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_ADC1	, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
 

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M

	//PA1 ��Ϊģ��ͨ����������                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOC, &GPIO_InitStructure);	

	ADC_DeInit(ADC1);  //��λADC1,������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;	//��ͨ��ѭ���ɼ�
	ADC_InitStructure.ADC_ContinuousConvMode =  DISABLE;	//ģ��ת������������ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 2;	//˳����й���ת����ADCͨ������Ŀ������ͨ��ADC14��ADC15
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_239Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 2, ADC_SampleTime_239Cycles5 );
	
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
	ADC_ExternalTrigConvCmd(ADC1,ENABLE);  //��ʱ��3�ⲿ����
	ADC_DMACmd(ADC1, ENABLE);	
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC1);	 //����ADУ׼
 
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
 
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������


}	

	
//���ADCֵ
//ch:ͨ��ֵ 0~3
u16 Get_Adc(u8 ch)   
{
  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
}

u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 	 

 /*************************************************************
Function  :  Kalman_Init()
Description : �������˲���������ʼ������Ҫ����Q Rֵ
Input  :   
return  :  
*************************************************************/ 
Kalman kfp;   //����kfp�ṹ��
void Kalman_Init()
{
//	Kalman kfp;
	
	kfp.Last_P = 1;		 //��Ϊ0���ɣ���ֵֻӰ���ʼ�����ٶ�	
	kfp.Now_P = 0;		
	kfp.out = 0;			
	kfp.Kg = 0;		
	kfp.Q = 0.02;   //��С�������
	kfp.R = 1.25;    //�Ӵ�С���� 20��ADƫ��
	kfp.A = 1;
}

 /*************************************************************
Function  :  KalmanFilter
Description : �������˲���
Input  :   *kfp���������ṹ��  ��input������������ɼ�ADֵ��
return  :  �������˲����������ֵ
*************************************************************/ 
float KalmanFilter(Kalman *kfp,float input)   //A = 1,B = 0�����������ֵ������һ�ε����ֵ�����Է���1����Ҫ
{
   //Ԥ��Э����̣�kʱ��ϵͳ����Э���� = k-1ʱ�̵�ϵͳЭ���� + ��������Э����
   kfp->Now_P = kfp->Last_P + kfp->Q;
   //���������淽�̣����������� = kʱ��ϵͳ����Э���� / ��kʱ��ϵͳ����Э���� + �۲�����Э���
   kfp->Kg = kfp->Now_P / (kfp->Now_P + kfp->R);
   //��������ֵ���̣�kʱ��״̬����������ֵ = ״̬������Ԥ��ֵ + ���������� * ������ֵ - ״̬������Ԥ��ֵ��
   kfp->out = kfp->out + kfp->Kg * (input -kfp->out);//��Ϊ��һ�ε�Ԥ��ֵ������һ�ε����ֵ
   //����Э�����: ���ε�ϵͳЭ����� kfp->LastP ����һ������׼����
   kfp->Last_P = (1-kfp->Kg) * kfp->Now_P;
	 
//	 kfp->out = input;
   return kfp->out;
}
extern u16 ADC_ConvertedValue[2];

void Wait_KalmanFilter(u8 times)
{
		u16 data;
		u8 i;
		
		for(i = 0;i<=times;i++)
		{
			
		  data = KalmanFilter(&kfp,ADC_ConvertedValue[1]);  //�������˲��ɼ�ADֵ
			delay_ms(200);
		}
	
}

u16 PhaseADC_Average(u8 times)
{
		u8 t = 0;
		u16 data_Phase;
		for(t=0;t<times;t++)
		{
			data_Phase+=ADC_ConvertedValue[1];
			delay_ms(5);
		}	
		return data_Phase/times;		
}


u16 Yn_Now=0;  //���������
u16 Yn_Last=0 ;   //��һ�������
		
u16 FIR_Filter(u16 input)
{
		
		Yn_Now = q*input + (1-q)*Yn_Last;
		
		Yn_Last = Yn_Now;
	
		return Yn_Now;

}

















