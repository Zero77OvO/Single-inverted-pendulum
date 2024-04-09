#include "interrupt.h"
#include "usart.h"
#include "adcc.h"
#include "adc.h"
#include "tim.h"
#include "pid.h"



float Kp_v =24.8;   //角度环P,D
float Kd_v =8.86 ;   

float Kp_x = 0.025; //位置环P,D  位置环都没调，逻辑对了，角度环闭好就能起来
float Kd_x = 0.000;


float Ki_v;
float err_v = 0;      //角度环参数
float last_err_v = 0;
float err_sum_v = 0;
float err_difference_v;



float err_x = 0;      //位置环参数
float last_err_x = 0;
float err_sum_x = 0;
float err_difference_x;



float v_out = 0;    //角度环输出
float x_out = 0;    //位置环输出





int target_location=30000; //编码器目标位置，我们是把30000作为编码器初始化位置，摆往左走为减，往右走为加
int target_angle=3456;     //电位器目标角度


int vertical_PID_value(float measure, float calcu)   //角度PID运算
{
    err_v = measure - calcu;
    err_sum_v += err_v;
    // xianfu	
	  if(err_sum_v > 1000)
		err_sum_v = 1000;
	  else if(err_sum_v < -1000)
		err_sum_v = -1000;
    err_difference_v = err_v - last_err_v;
    v_out = Kp_v * err_v + Kd_v * (err_v - last_err_v)+ Ki_v*err_sum_v;
    last_err_v = err_v;
    return v_out;
}


int location_PID_value(float measure, float calcu)   //位置PID运算
{
    err_x = measure - calcu;
    err_sum_x += err_x;
    // xianfu
    err_difference_x = err_x - last_err_x;
    x_out = Kp_x * err_x + Kd_x * (err_x - last_err_x);
    last_err_x = err_x;
	if(x_out>500){x_out=500;}
	if(x_out<-500){x_out=-500;}
	//HAL_Delay(10);
    return x_out;
}




