#include "interrupt.h"
#include "usart.h"
#include "adcc.h"
#include "adc.h"
#include "tim.h"
#include "pid.h"



float Kp_v =24.8;   //�ǶȻ�P,D
float Kd_v =8.86 ;   

float Kp_x = 0.025; //λ�û�P,D  λ�û���û�����߼����ˣ��ǶȻ��պþ�������
float Kd_x = 0.000;


float Ki_v;
float err_v = 0;      //�ǶȻ�����
float last_err_v = 0;
float err_sum_v = 0;
float err_difference_v;



float err_x = 0;      //λ�û�����
float last_err_x = 0;
float err_sum_x = 0;
float err_difference_x;



float v_out = 0;    //�ǶȻ����
float x_out = 0;    //λ�û����





int target_location=30000; //������Ŀ��λ�ã������ǰ�30000��Ϊ��������ʼ��λ�ã���������Ϊ����������Ϊ��
int target_angle=3456;     //��λ��Ŀ��Ƕ�


int vertical_PID_value(float measure, float calcu)   //�Ƕ�PID����
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


int location_PID_value(float measure, float calcu)   //λ��PID����
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




