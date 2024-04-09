#include "interrupt.h"
#include "usart.h"
#include "adcc.h"
#include "adc.h"
#include "tim.h"
#include "pid.h"

int time_1s = 0;
int flag_stop = 0;
extern int flag_init;

extern float v_out;  //角度环pid输出
extern float x_out;  //位置环pid输出


int num_counter = 30000; //编码器的值

int location1;   //编码器位置1
int location2;   //编码器位置2
int location_end; //编码器位置差
int location;  //位置环PID返回值



int start_mode =-188;

int raw_now;   //电位器滤波前的值


int alarm_left = 0;
int alarm_right = 0;

int a = 0;

int raw_filter;//通过平滑滤波处理过后的角度值，具体函数在adcc里。没用平均值滤波是因为滤波处理太慢，角度采集不及时，pid调不了


extern int target_location; //位置目标值
extern int target_angle;    //角度目标值

//标志位
extern int c,b;
int t;
int t_angle;
int t_location;
int t_speed;

int stat=0;
int stat1=1;
int stat2=1;



//为了做题分题加的标志位//
extern int flag_jc12;
extern int flag_jc3;
extern int flag_fh;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	
	
	
	
	///////////////定时器4，25ms，只放了个标志位////////////
	  if (htim->Instance == TIM4)
    { 
			b++;
		
		}
		
		
		////////////定时器3，0.1ms////////////////
	  if (htim->Instance == TIM3)
		{ t_angle++;t_location++;t_speed++; //////三个标志位，用来判断后面三个函数进入周期的////
			
			
			raw_now=(int)(get_adc(&hadc1) * 1000);
			raw_filter = moveAverageFilter(raw_now); /////获取当前角度值/////
			
		
			
			////////PID//////
			if(location_end<-10&&location_end>10){stat=1;}
			
			if(t_angle==5){ v_out=0.8*vertical_PID_value((raw_filter-location), target_angle);t_angle=0;} //每0.5毫秒运算一次角度环，输出电机pwm
			
			
			
			///////上摆逻辑部分////////
			if(t_location==50){
				
				location1=__HAL_TIM_GET_COUNTER(&htim2);    //每5毫秒获取一次电机位置，并计算出两者差值，作为电机速度使用      
				
   			location_end=(location1-location2);   
				 
				location2=location1;                  //将旧的值赋给location2，存储
        
				if((location_end<2&&location_end>-2)){target_location=location1;} //当速度比较小时候，将当前位置值赋给位置环的目标值，目的是让摆在速度较低的时候稳定
				
				
				/////////////////////控制摆超限，使之回调/////////////////
				
				if(location1<29000){stat1 = 0;}           
				if(stat1==0){
					target_angle=3446;}                 //target_angle为目标角度值
				                                      //当角度环调的差不多时候，会出现摆朝一个方向摆，不能稳定的情况，这时候就需要调一点位置环，将位置环PID的输出作为角度返回到角度环中，即93行
				if(location1>29500&&location1<30500){ // 这时候摆会立一会，然后开始大幅度震荡。 106行就是为提高稳定性
				target_angle=3457;stat1=1;stat2=1;}   //在一个震荡周期中，选取合适的编码器位置，让它摆动超过这个值的时候，将其角度环的目标值往摆的运动反方向微调2度左右，即可实现回调，
				                                      //使得摆不会朝一个方向eng冲下去，两边都这样做一下，即可实现摆周期运动
				if(location1>31000){stat2 = 0;}        //定时器的周期越短，可能参数就越好调，因为精确
				if(stat2==0){
				target_angle=3466;}

	
				location= location_PID_value((num_counter), target_location); //位置环PID运算，输入值分别是 当前电机位置，目标电机位置
				
				t_location=0;               //标志位清0
				
				 				
			}
////////////////////////////PID运动控制//////////////////////////////
			      if (start_mode == 2||flag_jc3==1)
            { 
							
              if (flag_stop == 0 && flag_init == 0)
                {    
									if (v_out < 40 && v_out > 0)       //防电机死区
											v_out = 40;
										if (v_out > -40 && v_out < 0)
												v_out = -40;

								if(stat==0||stat==1){
																	
									 if (v_out > 0)
                    {
                        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
                        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
                        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, v_out);
                    }
                    if (v_out < 0)
                    {
                        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
                        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
                        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, -v_out);

										}

                }
            }
							
		     }
		 ////////////////////////////////////////////////////////////////////////
		
		}
		//定时器1，1ms,里面写了基础的12题
    if (htim->Instance == TIM1)
    { 
			 

			num_counter = __HAL_TIM_GET_COUNTER(&htim2); //获取当前电机编码器的值
			
			 // true_speed=30000-__HAL_TIM_GET_COUNTER(&htim2);
		  	
	/////////////////////////////////边界保护/////////////////////////////
        if (num_counter > 32000 && a == 1) 
        {
            alarm_right = 1;
            start_mode = 1;
        }

        if (num_counter < 27000 && a == 1)
        {
            alarm_left = 1;
            start_mode = 1;
        }
        if (num_counter > 29000 && num_counter < 31000)
        {
				
            alarm_left = 0;
            alarm_right = 0;
        } 
				/////////////////////////////////////////////////////////////////////////
				
				
				
           ////////////////运动部分///////////////////////////////////
        if (flag_init == 0 && flag_stop == 0)
        {
					
					
					/////////////////初始化摆，给摆一个启动速度////////////////////////
					if(start_mode==-1){
						c++;//标志位
					if(c==120){ HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
                     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
                    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 400);}
					if(c==240){
										HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
                     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
                    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 400);}
					if(c==360){start_mode=0;c=0;}
					
					
					}
					
										

					///////////////////////////////////////////第一阶段///超级起摆/////////////////////////////////
					
            if (start_mode == 0) 
            {
                if ((raw_filter > 100&& raw_filter < 200)||(raw_filter > 2900&& raw_filter < 3000)) //////////// 满足角度条件则进入第二阶段，低速起摆
                {
                    start_mode = 1;
                }
                //////////////////////////////////////////////////////////////////////////////////////
                if (raw_filter > 1600 && raw_filter < 3560) /////////////////////////杆在左侧
                {
                    if (alarm_left == 0) //////////////////////////////下落趋势向→→→→→→→→走
                    {
                        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
                        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
                        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 300);
                    }
                }
                else /////////////////杆在右侧
                {
                    if (alarm_right == 0) ////////////////////////////下落趋势向←←←←走
                    {
                        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
                        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
                        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 300);
                    }
                }
            }
						
						/////////////////////////////////第二阶段///低速起摆/////////////////////////////////
						
            if (start_mode == 1)
            {
							if (raw_filter > 3250 && raw_filter < 3450) ///////////////////满足条件则进入第三阶段，PID控制。
                {
                    start_mode = 2;                                  //PID部分在定时器3，定时器3周期为0.1ms，定时器1是1ms，可能周期越短越好调pid，越精确
                }

                if (raw_filter > 1600 && raw_filter < 3560)  /////////////////////////杆在左侧
                {
                    if (alarm_left == 0) //////////////////////////////下落趋势向→→→→→→→→走
                    {
                        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
                        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
                        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 170);
                    }
                }
                else /////////////////杆在右侧
                {
                    if (alarm_right == 0) ////////////////////////////下落趋势向←←←←走
                    {
                        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
                        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
                        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 170);
                    }
                }
            }
						
						
						///////////////////////////这一段同上，只是复制出来，展示做题用的/////////////////////////////////
						 if (flag_jc12 == 1)
            {

                if (raw_filter > 1600 && raw_filter < 3560)  /////////////////////////杆在左侧
                {
                    if (alarm_left == 0) //////////////////////////////下落趋势向→→→→→→→→走
                    {
                        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
                        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
                        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 170);
                    }
                }
                else /////////////////杆在右侧
                {
                    if (alarm_right == 0) ////////////////////////////下落趋势向←←←←走
                    {
                        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
                        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
                        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 170);
                    }
                }
            }
               ////////////////////////////////////////////////////////////////////////////////////////////////




///////////////////////////亮个灯先////////////////////////////////////////////////////
        if (++time_1s == 100)
        {
            HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
            time_1s = 0;
        }
        a = 1;
    }
}

	}

	
char rxdatas[30];
uint8_t rxdata;
unsigned char rx_pointer;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    rxdatas[rx_pointer++] = rxdata - 48;
	 HAL_UART_Receive_IT(&huart3, &rxdata, 1);

}
