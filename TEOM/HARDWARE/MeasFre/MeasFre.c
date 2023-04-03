 #include "MeasFre.h"
 #include "sys.h"   //�⺯��
 #include "dac.h"    //����TIMʹ��
 #include "adc.h"
 #include "delay.h"
 #include "stdio.h"
 #include "usart.h"
 #include "led.h"
 #include "dma.h"
 
 #define LIMIT(x,min,max) (x)=(((x)<=(min))?(min):(((x)>=(max))?(max):(x)))


float PID_SingleCalc(PID *pid,float reference,float Set_val)
{

	pid->Now_error=reference - Set_val;//���㵱ǰ��� 
		
//	pid->integral = pid->Sum_error*pid->ki;//��������ۼ�
//	LIMIT(pid->integral,-pid->Max_Integral,pid->Max_Integral);//�����޷�	
	pid->Output = (pid->Now_error - pid->Last_Error) * pid->kp + pid->ki* pid->Now_error;  //����������΢����

	LIMIT(pid->Output,-pid->Max_Output,pid->Max_Output);		//����޷�

	pid->Last_Error = pid->Now_error;
	return pid->Output;
}
/*************************************************************
Function  :  PID_Init
Description : PID������ʼ��
Input  :     �ṹ����
return  :   
*************************************************************/ 
void PID_Init(PID *pid,double p,double i,double d,u16 maxI,u16 maxOut)
{
	pid->kp=p;
	pid->ki=i;
	pid->kd=d;
	pid->Max_Integral=maxI;  //�����޷�
	pid->Max_Output=maxOut;   //����޷�
}
/*************************************************************
Function  :  PID_Clear
Description : ���PID��ʷ����
Input  :     �ṹ����
return  :   
*************************************************************/ 
void PID_Clear(PID *pid)
{
	pid->Now_error=0;
	pid->Last_Error=0;
	pid->integral=0;
	pid->Output=0;
	pid->Sum_error = 0;

}

/*************************************************************
Function  :  SetFre_Init
Description : ɨ������Ƶ��(�˷������)
Input  :     ��ʼƵ��
return  :   
*************************************************************/ 

extern u16 ADC_ConvertedValue[2];
void SetFre_Init()
{
	u16 Vrms[100];  //����˷����������
	u8 i,j;
	u8 count = 0;
	u16 Max_Vrms = 0;

	for(i = 0;i <= 100;i++)   //ѭ���ɼ��˷������� ��������
	{
		TIM4->ARR = (TIM4->ARR - 14);
		delay_ms(500);
		Vrms[i] = ADC_ConvertedValue[0];

		
	}	
	for(j = 0;j <= 100;j++)  //�ҳ����ֵ�����±�
	{
		if(Max_Vrms < Vrms[j])
		{
			Max_Vrms = Vrms[j];
			count = j;
		}
	}
	TIM4->ARR = (TIM4->ARR +1400 - 14*count );
	
		
}


void Wait_Preheat(u16 times)  //Ԥ��
{
	u16 i;
	for(i = 0;i <= times;i++)
	{
		delay_ms(1000); //1s�ȴ�
	}
}

float Calculate_HZ(float f)
{
		float f_average,f1 = 0;
		u8 i = 0;
		for(i = 0;i<=10;i++)
		{
			f1 = f;
			
			f_average += f1;
		}
		return f_average/10;
}
