//#include <main.h>
//#include "vofa.h"

//void Float_to_Byte(float f, unsigned char byte[])
//{
//	FloatLongType fl;
//	fl.fdata = f;
//	byte[0] = (unsigned char)fl.ldata;
//	byte[1] = (unsigned char)(fl.ldata >> 8);
//	byte[2] = (unsigned char)(fl.ldata >> 16);
//	byte[3] = (unsigned char)(fl.ldata >> 24);
//}

//void Send_Data(UART_HandleTypeDef *huart, float f)
//{
//	unsigned char byte[4] = {0};

//	Float_to_Byte(f, byte);
//	HAL_UART_Transmit(huart, (uint8_t *)&byte[0], 1, 0xffff);
//	while (HAL_UART_GetState(huart) == HAL_UART_STATE_BUSY_TX)
//		;
//	HAL_UART_Transmit(huart, (uint8_t *)&byte[1], 1, 0xffff);
//	while (HAL_UART_GetState(huart) == HAL_UART_STATE_BUSY_TX)
//		;
//	HAL_UART_Transmit(huart, (uint8_t *)&byte[2], 1, 0xffff);
//	while (HAL_UART_GetState(huart) == HAL_UART_STATE_BUSY_TX)
//		;
//	HAL_UART_Transmit(huart, (uint8_t *)&byte[3], 1, 0xffff);
//	while (HAL_UART_GetState(huart) == HAL_UART_STATE_BUSY_TX)
//		;
//}

//void Send_Tail(UART_HandleTypeDef *huart)
//{
//	unsigned char byte[4] = {0x00, 0x00, 0x80, 0x7f};

//	HAL_UART_Transmit(huart, (uint8_t *)&byte[0], 1, 0xffff);
//	while (HAL_UART_GetState(huart) == HAL_UART_STATE_BUSY_TX)
//		;
//	HAL_UART_Transmit(huart, (uint8_t *)&byte[1], 1, 0xffff);
//	while (HAL_UART_GetState(huart) == HAL_UART_STATE_BUSY_TX)
//		;
//	HAL_UART_Transmit(huart, (uint8_t *)&byte[2], 1, 0xffff);
//	while (HAL_UART_GetState(huart) == HAL_UART_STATE_BUSY_TX)
//		;
//	HAL_UART_Transmit(huart, (uint8_t *)&byte[3], 1, 0xffff);
//	while (HAL_UART_GetState(huart) == HAL_UART_STATE_BUSY_TX)
//		;
//}

//// Add Send_Date

////extern double A;
////extern double B;
//extern float Kp_v ;
//extern float Kd_v ;
//extern float Kp_x ;
//extern float Kd_x ;
//extern float v_out ;         // VOFA，其实还是挺重要的，后期调参很直观，某个参数大或小，什么状态，都可以用VOFA显示
//extern int location;
//extern int raw_now;
//extern int raw_filter;
//extern int  speed1;
//extern int  speed2;
//extern int  target_location;
//extern int  stat1;
//extern int  stat2;
//extern int  target_angle;


//extern int  speed_end;
//int mubiao=1126;

//void vodka_JustFloat_send(UART_HandleTypeDef *huart)
//{

//	


//	// Send_Data(huart, (float)ki_x);

//	 Send_Data(huart, (int)location);
//   Send_Data(huart, (int)target_angle);
//	 Send_Data(huart, (float)v_out);
//	 Send_Data(huart, (int)speed1);
//	 Send_Data(huart, (int)speed_end);
//	 Send_Data(huart, (int)target_location);
//	 Send_Data(huart, (int)stat1);
//	 Send_Data(huart, (int)stat2);
//	


//	Send_Tail(huart);
//}

///**example**/
//// while(1)
//// {
//// 	vodka_JustFloat_send(&huart1);
//// }

//#define RXSTRSIZE 256		  // ???????????
//uint8_t rx_string[RXSTRSIZE]; // ?????????????
//uint8_t rx_cnt = 0;			  // ?????????????
//uint8_t rx_buff;			  // ???????
//uint8_t RxBuff;
//unsigned char shujv[12];
//int vofa_i = 0, vofa_I = 0;
//double vofa_float;





//uint8_t RxState = 0; 
//char num;
//char num2;
//char num3;

//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//	uint8_t dat;

//		
//		// ?????????????ж???????
//		if (rx_cnt++ >= RXSTRSIZE - 1) // ????ж?
//		{
//			rx_cnt = 0;
//			memset(rx_string, 0x00, sizeof(rx_string));
//		}
//		else
//		{
//			dat = rx_buff; // ???????????

//			shujv[vofa_I++] = dat;

//			if (dat == 0x0a)
//			{

//				for (vofa_i = 2, vofa_float = 0; vofa_i < vofa_I - 1; vofa_i++)
//					vofa_float = 10 * vofa_float + (shujv[vofa_i] - '0');
//				vofa_I = 0;

//				if (shujv[1] == 0x3A)
//				{
//					switch (shujv[0])
//					{
//					// case 'S':my_dat.biaozhi =  shujv[2]  ; break;
//					case 'Q'://VOFA
//						raw_now= vofa_float; //keil
//						break;

//					case 'P':
//						Kp_v = vofa_float;
//						break;
////					case 'I':
////						ki_x = vofa_float;
////						break;
//					case 'D':
//						Kd_v = vofa_float;
//						break;
//					}
//				}
//			}
//			memset(rx_string, 0x00, sizeof(rx_string)); // ???????????
//			rx_cnt = 0;									// ????????
//		}
//	
//	

//	HAL_UART_Receive_IT(&huart3, (uint8_t *)&rx_buff, 1);  // ??????????ж????????????????
//}
