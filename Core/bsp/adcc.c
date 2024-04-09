#include "adcc.h"
#include "adc.h"



#define N 100


int time =0;
int value_buf[N];
int sum = 0;
int curNum = 0;


float data[100];
float averageFilter(float in_data)
{
	float sum = 0;
	for(int i=0; i<10; i++)
	{
		data[i]=data[i+1];
		sum = sum + data[i];
	}
	data[10] = in_data;
	sum = sum + data[10];
	
	return(sum/10);
 
}


//float get_adc(ADC_HandleTypeDef *pin)
//{   
//    HAL_ADC_Start(pin);
//    int adc;
//    adc = HAL_ADC_GetValue(pin);
//	  
//	return adc* 3.6 / 4096 ;
//	
//    
//}

float get_adc(ADC_HandleTypeDef *pin)
{   
    HAL_ADC_Start(pin);
    int adc;
    adc = HAL_ADC_GetValue(pin);

		return adc* 3.6 / 4095 ;
	
	
    
}

int moveAverageFilter(int now)
{
    if (curNum < N)
    {
        value_buf[curNum] = now;
        sum += value_buf[curNum];
        curNum++;
        return sum / curNum;
    }
    else
    {
        sum -= sum / N;
        sum += now;
        return sum / N;
    }
}
