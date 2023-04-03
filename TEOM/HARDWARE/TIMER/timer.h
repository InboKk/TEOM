#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
//#define HZ(x) (u16)(18000000/(x+1))  //分频，最低276Hz
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//定时器 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   


void TIM4_Int_Init(u16 Hz);  //方波
void TIM5_Int_Init(u32 arr,u16 pre);  //PID
void TIM6_Int_Init(u32 arr,u16 pre) ;  //数据上传定时器
void TIM3_Int_Init(u32 arr,u16 pre) ; //ADC采样定时器
void TIM2_Int_Init(u32 arr,u16 pre) ;  //检测Vrms定时器
#endif
