#ifndef _ADCC_H
#define _ADCC_H

#include "main.h"

float get_adc(ADC_HandleTypeDef *pin);
int moveAverageFilter(int now);

#endif
