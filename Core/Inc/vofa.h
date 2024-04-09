/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-05-28 11:34:32
 * @LastEditors: cui-zf 2998919571@qq.com
 * @LastEditTime: 2023-07-23 16:08:28
 * @FilePath: \MDK-ARMc:\Users\Cui\Desktop\AGV -mycore(1)\23.5.28_uart2\2\vofa\vofa.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef _VOFA_H_
#define _VOFA_H_

#include <main.h>
#include "usart.h"
#include "string.h"



typedef union
{
	float fdata;
	unsigned long ldata;
} FloatLongType;



void vodka_JustFloat_send(UART_HandleTypeDef *huart);
#endif
