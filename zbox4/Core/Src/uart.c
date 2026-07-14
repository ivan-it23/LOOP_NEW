/*
 * uart.c
 *
 *  Created on: 13 апр. 2023 г.
 *      Author: user
 */


#include "main.h"
#include "stdio.h"
#include "string.h"


char RxBuffer[2] = {0,};
char RxBuffer2[2] = {0,};
char TxBuffer[256] = {0,};
char TxBuffer2[256] = {0,};
char rxStr[RX_BUFFER_SIZE], rxStr2[RX_BUFFER_SIZE]; //принимаемая строка
char cmdBuf[RX_BUFFER_SIZE], cmdBuf2[RX_BUFFER_SIZE]; //буфер для принятой команды
int cnt = 0;
//------------------------------------------------------------------------------
/*void SendString(char *str)
{
    while(*str)
    {
    	HAL_UART_Transmit(&huart1, (uint8_t*)str++, 1, 1000);
    }
}*/
//------------------------------------------------------------------------------
void SendString(char *str)
{
	HAL_UART_Transmit_IT(&huart1, (uint8_t*)str, strlen(str));
}
//------------------------------------------------------------------------------
void SendString2(char *str)
{
	HAL_UART_Transmit_IT(&huart2, (uint8_t*)str, strlen(str));
}
//------------------------------------------------------------------------------
volatile uint8_t n_bytes = 0; //байт принято
volatile uint8_t n_bytes2 = 0; //байт принято
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
uint8_t rxbyte, rxbyte2;
//static uint8_t n_bytes = 0; //байт принято
  if(huart == &huart1)
  {
	  rxbyte = RxBuffer[0];
	  rxStr[n_bytes] = rxbyte;
	  n_bytes++;

	  if(n_bytes >= 250) //переполнение
	  {
		HAL_TIM_Base_Stop_IT(&htim3);
	    n_bytes = 0;
	    HAL_UART_Receive_IT(&huart1, (uint8_t*)&RxBuffer, 1);
	    return;
	  }
	  if(n_bytes > 0)
	  {
		 TIM3->CNT = 0;
	  }
	  else //
	  {
		  HAL_TIM_Base_Start_IT(&htim3);
	  }

	  if(rxbyte == '\n') // \r\n - конец строки
	  {
		HAL_TIM_Base_Stop_IT(&htim3);
	    if(!bCmdFlag)
	    {
	      memcpy(cmdBuf, rxStr, n_bytes);
	      bCmdFlag = 1;
	    }
	    else
	     asm("nop"); //не успел обработать команду

	   n_bytes = 0; //сброс
	  }

	  HAL_UART_Receive_IT(&huart1, (uint8_t*)&RxBuffer, 1);
  }
  else if(huart == &huart2)
  {
	  rxbyte2 = RxBuffer2[0];
	  rxStr2[n_bytes2] = rxbyte2;
	  n_bytes2++;

	  if(n_bytes2 >= 250) //переполнение
	  {
		HAL_TIM_Base_Stop_IT(&htim15);
	    n_bytes2 = 0;
	    HAL_UART_Receive_IT(&huart2, (uint8_t*)&RxBuffer2, 1);
	    return;
	  }
	  if(n_bytes2 > 0)
	  {
		 TIM15->CNT = 0;
	  }
	  else //
	  {
		  HAL_TIM_Base_Start_IT(&htim15);
	  }

	  if(rxbyte2 == '\n') // \r\n - конец строки
	  {
		HAL_TIM_Base_Stop_IT(&htim15);
	    if(!bCmdFlag2)
	    {
	      memcpy(cmdBuf2, rxStr2, n_bytes2);
	      bCmdFlag2 = 1;
	    }
	    else
	     asm("nop"); //не успел обработать команду

	   n_bytes2 = 0; //сброс
	  }

	  HAL_UART_Receive_IT(&huart2, (uint8_t*)&RxBuffer2, 1);
  }
}

//------------------------------------------------------------------------------


