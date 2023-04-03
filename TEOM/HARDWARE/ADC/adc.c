 #include "adc.h"
 #include "delay.h"
		   
#define q  0.0252   //a = fp*2*pi*T = 10*2*3.14*0.001 = 0.0252  0.0628 15AD



void  Adc1_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_ADC1	, ENABLE );	  //使能ADC1通道时钟
 

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M

	//PA1 作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOC, &GPIO_InitStructure);	

	ADC_DeInit(ADC1);  //复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;	//多通道循环采集
	ADC_InitStructure.ADC_ContinuousConvMode =  DISABLE;	//模数转换工作在连续转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 2;	//顺序进行规则转换的ADC通道的数目，两个通道ADC14和ADC15
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_239Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 2, ADC_SampleTime_239Cycles5 );
	
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
	ADC_ExternalTrigConvCmd(ADC1,ENABLE);  //定时器3外部触发
	ADC_DMACmd(ADC1, ENABLE);	
	ADC_ResetCalibration(ADC1);	//使能复位校准  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	
	ADC_StartCalibration(ADC1);	 //开启AD校准
 
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
 
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能


}	

	
//获得ADC值
//ch:通道值 0~3
u16 Get_Adc(u8 ch)   
{
  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
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
Description : 卡尔曼滤波器参数初始化，主要调节Q R值
Input  :   
return  :  
*************************************************************/ 
Kalman kfp;   //定义kfp结构体
void Kalman_Init()
{
//	Kalman kfp;
	
	kfp.Last_P = 1;		 //不为0即可，其值只影响初始收敛速度	
	kfp.Now_P = 0;		
	kfp.out = 0;			
	kfp.Kg = 0;		
	kfp.Q = 0.02;   //从小到大调整
	kfp.R = 1.25;    //从大到小调整 20个AD偏差
	kfp.A = 1;
}

 /*************************************************************
Function  :  KalmanFilter
Description : 卡尔曼滤波器
Input  :   *kfp：卡尔曼结构体  ；input：输入参数（采集AD值）
return  :  卡尔曼滤波后输出参数值
*************************************************************/ 
float KalmanFilter(Kalman *kfp,float input)   //A = 1,B = 0；则先验估计值等于上一次的输出值，所以方程1不需要
{
   //预测协方差方程：k时刻系统估算协方差 = k-1时刻的系统协方差 + 过程噪声协方差
   kfp->Now_P = kfp->Last_P + kfp->Q;
   //卡尔曼增益方程：卡尔曼增益 = k时刻系统估算协方差 / （k时刻系统估算协方差 + 观测噪声协方差）
   kfp->Kg = kfp->Now_P / (kfp->Now_P + kfp->R);
   //更新最优值方程：k时刻状态变量的最优值 = 状态变量的预测值 + 卡尔曼增益 * （测量值 - 状态变量的预测值）
   kfp->out = kfp->out + kfp->Kg * (input -kfp->out);//因为这一次的预测值就是上一次的输出值
   //更新协方差方程: 本次的系统协方差付给 kfp->LastP 威下一次运算准备。
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
			
		  data = KalmanFilter(&kfp,ADC_ConvertedValue[1]);  //卡尔曼滤波采集AD值
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


u16 Yn_Now=0;  //现在输出量
u16 Yn_Last=0 ;   //上一次输出量
		
u16 FIR_Filter(u16 input)
{
		
		Yn_Now = q*input + (1-q)*Yn_Last;
		
		Yn_Last = Yn_Now;
	
		return Yn_Now;

}

















