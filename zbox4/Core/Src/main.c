/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define NUMZONDES 5//4 или 5, 4 сдвиигает емкости
#define VREF 332 //%100
#define VLOW 290
#define IDLETIME 30 //в минутах
#define FLASH_ADDR_SETTINGS 0x0800F618 //адрес куда будем писать Z (256 байт)
#include "string.h"
#include "stdio.h"
#include <complex.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim15;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
volatile uint8_t bCmdFlag = 0, bCmdFlag2 = 0;
volatile uint64_t iOFFTime = IDLETIME*60*1000;//ввремя выключения в мс
volatile uint64_t Now = 0;
int iRes = 0, iCap = 0, iEnable = 0;
uint8_t args;
char str[64];
char str2[64];
int iZIndex;
float fRe, fIm;
typedef struct
{
	float Re;
	float Im;
}TZ;

typedef struct
{
	TZ Z[32];
	uint32_t SGN;
}TSettings;

TSettings Settings;

typedef struct
{
	float Re;
	float Im;
} complex_float;

complex_float cplxf(float complex src)
{
	complex_float res;
	res.Re = crealf(src);
	res.Im = cimagf(src);
	return res;
}
//-------------------------------------------------------------------------------------------
float complex CMPLXF(float re, float im)
{
  return re+im*I;
}

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM3_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM15_Init(void);
static void MX_ADC_Init(void);
static void MX_TIM6_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//-------------------------------------------------------------------------------------------
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM3) //100ms на завершение команды
	{
		HAL_TIM_Base_Stop_IT(&htim3);
		n_bytes = 0;
	}
	if(htim->Instance == TIM15) //100ms на завершение команды
	{
		HAL_TIM_Base_Stop_IT(&htim15);
		n_bytes2 = 0;
	}
	if(htim->Instance == TIM6)
	{
		Now++;
	}
	//HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_8);
}
//-------------------------------------------------------------------------------------------
int SetR(int R)
{
	HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, 0);
	HAL_GPIO_WritePin(R2_GPIO_Port, R2_Pin, 0);
	HAL_GPIO_WritePin(R3_GPIO_Port, R3_Pin, 0);
	HAL_GPIO_WritePin(R4_GPIO_Port, R4_Pin, 0);
	HAL_GPIO_WritePin(R5_GPIO_Port, R5_Pin, 0);
	HAL_GPIO_WritePin(R6_GPIO_Port, R6_Pin, 0);
	HAL_GPIO_WritePin(R7_GPIO_Port, R7_Pin, 0);
	HAL_GPIO_WritePin(R8_GPIO_Port, R8_Pin, 0);

	if(R == 0)
		return 1; //ok
	else if(R == 1)
		HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, 1);
	else if(R == 2)
		HAL_GPIO_WritePin(R2_GPIO_Port, R2_Pin, 1);
	else if(R == 3)
		HAL_GPIO_WritePin(R3_GPIO_Port, R3_Pin, 1);
	else if(R == 4)
		HAL_GPIO_WritePin(R4_GPIO_Port, R4_Pin, 1);
	else if(R == 5)
		HAL_GPIO_WritePin(R5_GPIO_Port, R5_Pin, 1);
	else if(R == 6)
		HAL_GPIO_WritePin(R6_GPIO_Port, R6_Pin, 1);
	else if(R == 7)
		HAL_GPIO_WritePin(R7_GPIO_Port, R7_Pin, 1);
	else if(R == 8)
		HAL_GPIO_WritePin(R8_GPIO_Port, R8_Pin, 1);
	else
		return 0; //error

	return 1; //ok
}
//-------------------------------------------------------------------------------------------
int SetC(int Cap)
{
	int C = Cap;
	HAL_GPIO_WritePin(C1_GPIO_Port, C1_Pin, 0);
	HAL_GPIO_WritePin(C2_GPIO_Port, C2_Pin, 0);
	HAL_GPIO_WritePin(C3_GPIO_Port, C3_Pin, 0);
	HAL_GPIO_WritePin(C4_GPIO_Port, C4_Pin, 0);
	HAL_GPIO_WritePin(C5_GPIO_Port, C5_Pin, 0);
	HAL_GPIO_WritePin(C6_GPIO_Port, C6_Pin, 0);
	HAL_GPIO_WritePin(C7_GPIO_Port, C7_Pin, 0);
	HAL_GPIO_WritePin(C8_GPIO_Port, C8_Pin, 0);
	HAL_GPIO_WritePin(C9_GPIO_Port, C9_Pin, 0);
	HAL_GPIO_WritePin(C10_GPIO_Port, C10_Pin, 0);


	if(NUMZONDES==4)
	{
		if(C>4)
			C=C+1;
	}

	if(C == 0)
		return 1; //ok
	else if(C == 1)
		HAL_GPIO_WritePin(C1_GPIO_Port, C1_Pin, 1);
	else if(C == 2)
		HAL_GPIO_WritePin(C2_GPIO_Port, C2_Pin, 1);
	else if(C == 3)
		HAL_GPIO_WritePin(C3_GPIO_Port, C3_Pin, 1);
	else if(C == 4)
		HAL_GPIO_WritePin(C4_GPIO_Port, C4_Pin, 1);
	else if(C == 5)
		HAL_GPIO_WritePin(C5_GPIO_Port, C5_Pin, 1);
	else if(C == 6)
		HAL_GPIO_WritePin(C6_GPIO_Port, C6_Pin, 1);
	else if(C == 7)
		HAL_GPIO_WritePin(C7_GPIO_Port, C7_Pin, 1);
	else if(C == 8)
		HAL_GPIO_WritePin(C8_GPIO_Port, C8_Pin, 1);
	else if(C == 9)
		HAL_GPIO_WritePin(C9_GPIO_Port, C9_Pin, 1);
	else if(C == 10)
		HAL_GPIO_WritePin(C10_GPIO_Port, C10_Pin, 1);
	else
		return 0; //error

	return 1; //ok
}
//-------------------------------------------------------------------------------------------
void LoadSettings(void)
{
	uint32_t addr = FLASH_ADDR_SETTINGS;
	uint32_t sz = sizeof(Settings);
	uint32_t *dataPtr = (uint32_t *)&Settings;
	for (int i = 0; i < sz / 4; i++)
	{
		dataPtr[i] = *(__IO uint32_t*)addr;
		addr += 4;
	}
}
//-------------------------------------------------------------------------------------------
uint8_t SaveSettings(void)
{
	uint32_t addr = FLASH_ADDR_SETTINGS;
	HAL_StatusTypeDef status;
	uint32_t sz = sizeof(Settings);          // замеряем размер структуры
	FLASH_EraseInitTypeDef FlashErase;                     // структура для функции стирания страницы
	uint32_t pageError = 0;                                // переменная для записи информации об ошибках в процессе стирания

	__disable_irq();                                       // запрещаем прерывания
	HAL_FLASH_Unlock();
	FlashErase.TypeErase = FLASH_TYPEERASE_PAGES;          // стирать постранично
	FlashErase.PageAddress = addr;                         // адрес страницы для стирания
	FlashErase.NbPages = sz/1024 + 1;         // считаем количество страниц, чтобы наш массив поместился
	if (HAL_FLASHEx_Erase(&FlashErase, &pageError) != HAL_OK)   // вызов функции стирания
	{
		HAL_FLASH_Lock();                                  // если не смог стереть, то закрыть память и вернуть ошибку
		return HAL_ERROR;
	}

    uint32_t *dataPtr = (uint32_t *)&Settings;          // создаем указатель на нашу структуру и записываем ее кусочками по 32 бита
	for(int i = 0; i < sz/4; i++)            // 4 байта = 32 бита
    {
		status += HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr, dataPtr[i]);
        addr += 4;                                         // сдвигаем адрес на 4 байта
    }
	__enable_irq();                                        // включаем прерывания обратно
	return status;
}
//-------------------------------------------------------------------------------------------
void CycleC()
{
	uint8_t R = 0;
	uint8_t C = 0;
	static uint8_t i = 0;

	if(NUMZONDES==4) //с пропуском
	{
		if(i++ < 22)
		{
			if(i == 0)
			{
				R = 0;
				C = 0;
			}
			else if(i == 1)
			{
				R = 1;
				C = 1;
			}
			else if(i == 2)
			{
				R = 1;
				C = 2;
			}
			else if(i == 3)
			{
				R = 1;
				C = 3;
			}
			else if(i == 4)
			{
				R = 1;
				C = 4;
			}
			else if(i == 5)
			{
				R = 2;
				C = 4;
			}
			else if(i == 6)
			{
				R = 3;
				C = 4;
			}
			else if(i == 7)
			{
				R = 4;
				C = 4;
			}
			else if(i == 8)
			{
				R = 5;
				C = 4;
			}
			else if(i == 9)
			{
				R = 6;
				C = 4;
			}
			else if(i == 10)
			{
				R = 7;
				C = 4;
			}
			else if(i == 11)
			{
				R = 8;
				C = 4;
			}

			else if(i == 12) //частота 2
			{
				R = 1;
				C = 5;
			}
			else if(i == 13)
			{
				R = 1;
				C = 6;
			}
			else if(i == 14)
			{
				R = 1;
				C = 7;
			}
			else if(i == 15)
			{
				R = 1;
				C = 8;
			}
			else if(i == 16)
			{
				R = 2;
				C = 8;
			}
			else if(i == 17)
			{
				R = 3;
				C = 8;
			}
			else if(i == 18)
			{
				R = 4;
				C = 8;
			}
			else if(i == 19)
			{
				R = 5;
				C = 8;
			}
			else if(i == 20)
			{
				R = 6;
				C = 8;
			}
			else if(i == 21)
			{
				R = 7;
				C = 8;
			}
			else if(i == 22)
			{
				R = 8;
				C = 8;
			}

		}
		else
		{
			R = 0;
			C = 0;
			i = 0;
		}

	}
	else //без пропуска
	{
		if(i++ < 24)
		{
			if(i == 0)
			{
				R = 0;
				C = 0;
			}
			else if(i == 1)
			{
				R = 1;
				C = 1;
			}
			else if(i == 2)
			{
				R = 1;
				C = 2;
			}
			else if(i == 3)
			{
				R = 1;
				C = 3;
			}
			else if(i == 4)
			{
				R = 1;
				C = 4;
			}
			else if(i == 5)
			{
				R = 1;
				C = 5;
			}
			else if(i == 6)
			{
				R = 2;
				C = 4;
			}
			else if(i == 7)
			{
				R = 3;
				C = 4;
			}
			else if(i == 8)
			{
				R = 4;
				C = 4;
			}
			else if(i == 9)
			{
				R = 5;
				C = 4;
			}
			else if(i == 10)
			{
				R = 6;
				C = 4;
			}
			else if(i == 11)
			{
				R = 7;
				C = 4;
			}

			else if(i == 12)
			{
				R = 8;
				C = 4;
			}
			else if(i == 13) //частота 2
			{
				R = 1;
				C = 6;
			}
			else if(i == 14)
			{
				R = 1;
				C = 7;
			}
			else if(i == 15)
			{
				R = 1;
				C = 8;
			}
			else if(i == 16)
			{
				R = 1;
				C = 9;
			}
			else if(i == 17)
			{
				R = 1;
				C = 10;
			}
			else if(i == 18)
			{
				R = 2;
				C = 9;
			}
			else if(i == 19)
			{
				R = 3;
				C = 9;
			}
			else if(i == 20)
			{
				R = 4;
				C = 9;
			}
			else if(i == 21)
			{
				R = 5;
				C = 9;
			}
			else if(i == 22)
			{
				R = 6;
				C = 9;
			}
			else if(i == 23)
			{
				R = 7;
				C = 9;
			}
			else if(i == 24)
			{
				R = 8;
				C = 9;
			}

		}
		else
		{
			R = 0;
			C = 0;
			i = 0;
		}
	}

	SetC(C);
	SetR(R);
}
//-------------------------------------------------------------------------------------------
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	uint32_t vbat;
	uint8_t bPressed = 0;
	uint8_t bCycled = 1;
	uint64_t ButtonTime = 0;
	uint32_t adc_value = 0;

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_USART2_UART_Init();
  MX_TIM15_Init();
  MX_ADC_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */


  HAL_TIM_Base_Start(&htim1);
  HAL_TIM_Base_Start_IT(&htim3);
  HAL_TIM_Base_Start_IT(&htim6);
  HAL_TIM_Base_Start_IT(&htim15);

  SetR(0);
  SetC(0);

  HAL_UART_Receive_IT(&huart1, (uint8_t*)&RxBuffer, 1); //по 1 байту
  HAL_UART_Receive_IT(&huart2, (uint8_t*)&RxBuffer2, 1); //по 1 байту

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

LoadSettings();

  iOFFTime = IDLETIME*60*1000;

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	adc_value = 0;
	for(uint8_t i=0;i<16;i++)
	{
		HAL_ADC_Start(&hadc);
		HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY);
		adc_value += HAL_ADC_GetValue(&hadc);
		HAL_ADC_Stop(&hadc);
	}
	adc_value = adc_value>>4;

	vbat = (adc_value*VREF)>>11;//%2

	if(Now > iOFFTime) //Авто-выкл. после 30 мин простоя
		HAL_GPIO_WritePin(OFF_GPIO_Port, OFF_Pin, 0);
	else
		HAL_GPIO_WritePin(OFF_GPIO_Port, OFF_Pin, 1);

	if(vbat < VLOW) //светодиод LOW BAT
		HAL_GPIO_WritePin(LED_LOW_GPIO_Port, LED_LOW_Pin, 1);
	else
		HAL_GPIO_WritePin(LED_LOW_GPIO_Port, LED_LOW_Pin, 0);

	//кнопка
	if(Now > 500) //время на вкл. питания
	{
		if(adc_value > 3600)
		{
			if(!bPressed)
			{
				bPressed = 1;
				ButtonTime = Now;
			}
			else //было нажато
			{
				if(ButtonTime+50 < Now && !bCycled) //еще раз проверяем (анти-дребезг)
				{
					CycleC();
					iOFFTime = Now + IDLETIME*60*1000;
					ButtonTime = Now+200;
					bCycled = 1;
				}
			}

		}
		else
		{
			bPressed = 0;
			bCycled = 0;
		}
	}

		//обработка команд
	    if(bCmdFlag) //RS-485
	    {
	    	bCmdFlag = 0;
			iOFFTime = Now + IDLETIME*60*1000;
			  if(cmdBuf[0] == 'V' && cmdBuf[1] == 'B' && cmdBuf[2] == 'A' && cmdBuf[3] == 'T') //VBAT
			  {
				  sprintf(str, "VBAT=%d\r\n", (uint16_t)vbat);
				  SendString(str);

				memset(cmdBuf, 0x00, RX_BUFFER_SIZE);
			  }
			  else if(cmdBuf[0] == 'O' && cmdBuf[1] == 'F' && cmdBuf[2] == 'F') //OFF
			  {
				HAL_GPIO_WritePin(OFF_GPIO_Port, OFF_Pin, 0);
				memset(cmdBuf, 0x00, RX_BUFFER_SIZE);
			  }
			  else if(cmdBuf[0] == 'R' && cmdBuf[1] == 'E' && cmdBuf[2] == 'S' && cmdBuf[3] == ' ') //RES
			  {
				args = sscanf(cmdBuf, "RES %d\r\n", &iRes);
				if(args == 1 && iRes >= 0 && iRes < 9)
				{
				  SetR(iRes);
				  sprintf(str, "RES=%d OK!\r\n", iRes);
				  SendString(str);
				}
				else
				{
				  SendString("Error!\r\n");
				}
				memset(cmdBuf, 0x00, RX_BUFFER_SIZE);
			  }
			  else if(cmdBuf[0] == 'C' && cmdBuf[1] == 'A' && cmdBuf[2] == 'P' && cmdBuf[3] == ' ') //CAP
			  {
				args = sscanf(cmdBuf, "CAP %d\r\n", &iCap);
				if(args == 1 && iCap >= 0 && iCap < 11)
				{
				  SetC(iCap);
				  sprintf(str, "CAP=%d OK!\r\n", iCap);
				  SendString(str);
				}
				else
				{
				  SendString("Error!\r\n");
				}
				memset(cmdBuf, 0x00, RX_BUFFER_SIZE);
			  }
			  else if(cmdBuf[0] == 'Z' && cmdBuf[1] == 'W' && cmdBuf[2] == 'R' && cmdBuf[3] == 'I' && cmdBuf[4] == 'T' && cmdBuf[5] == 'E') //ZWRITE
			  {
				args = sscanf(cmdBuf, "ZWRITE %d %f %f\r\n", &iZIndex, &fRe, &fIm);
				if(args == 3 && iZIndex >= 0 && iZIndex < 32)
				{
				  LoadSettings();
				  Settings.Z[iZIndex].Re = fRe;
				  Settings.Z[iZIndex].Im = fIm;
				  SaveSettings();
				  sprintf(str, "WRITE Z%d OK!\r\n", iZIndex);
				  SendString(str);
				}
				else
				{
				  SendString("Error!\r\n");
				}
				memset(cmdBuf, 0x00, RX_BUFFER_SIZE);
			  }
			  else if(cmdBuf[0] == 'Z' && cmdBuf[1] == 'R' && cmdBuf[2] == 'E' && cmdBuf[3] == 'A' && cmdBuf[4] == 'D') //ZREAD
			  {
				args = sscanf(cmdBuf, "ZREAD %d\r\n", &iZIndex);
				if(args == 1 && iZIndex >= 0 && iZIndex < 32)
				{
				  LoadSettings();
				  sprintf(str, "Z%d %f %f\r\n", iZIndex, Settings.Z[iZIndex].Re, Settings.Z[iZIndex].Im);
				  SendString(str);
				}
				else
				{
				  SendString("Error!\r\n");
				}
				memset(cmdBuf, 0x00, RX_BUFFER_SIZE);
			  }
			  else if(cmdBuf[0] == 'Z' && cmdBuf[1] == 'E' && cmdBuf[2] == 'R' && cmdBuf[3] == 'A' && cmdBuf[4] == 'S' && cmdBuf[5] == 'E') //ZERASE
			  {
				for(int i=0;i<32;i++)
				{
					Settings.Z[i].Im = 0;
					Settings.Z[i].Re = 0;
				}
				SaveSettings();
				SendString("ERASE OK!\r\n");
				memset(cmdBuf, 0x00, RX_BUFFER_SIZE);
			  }
			  else if(cmdBuf[0] == 'S' && cmdBuf[1] == 'G' && cmdBuf[2] == 'R' && cmdBuf[3] == 'E' && cmdBuf[4] == 'A' && cmdBuf[5] == 'D') //SGREAD
			  {
				  sprintf(str, "SG=%lu\r\n", Settings.SGN);
				  SendString(str);
			  }
			  else if(cmdBuf[0] == 'S' && cmdBuf[1] == 'G' && cmdBuf[2] == 'W' && cmdBuf[3] == 'R' && cmdBuf[4] == 'I' && cmdBuf[5] == 'T' && cmdBuf[6] == 'E') //SGWRITE
			  {
				  args = sscanf(cmdBuf, "SGWRITE %lu\r\n", &Settings.SGN);
				  SaveSettings();
				  sprintf(str, "WRITE SGN=%lu OK!\r\n", Settings.SGN);
				  SendString(str);
			  }
			  else
			  {
				memset(cmdBuf, 0x00, RX_BUFFER_SIZE);
				SendString("Unknown command!\r\n");
			  }
	    }
	    if(bCmdFlag2) //ВТ
	    {
	    	iOFFTime = Now + IDLETIME*60*1000;
				 if(cmdBuf2[0] == 'V' && cmdBuf2[1] == 'B' && cmdBuf2[2] == 'A' && cmdBuf2[3] == 'T') //VBAT
				 {
					  sprintf(str2, "VBAT=%d\r\n", (uint16_t)vbat);
					  SendString2(str2);

					memset(cmdBuf2, 0x00, RX_BUFFER_SIZE);
					bCmdFlag = 0;
				 }
				 else if(cmdBuf2[0] == 'O' && cmdBuf2[1] == 'F' && cmdBuf2[2] == 'F') //OFF
				 {
					HAL_GPIO_WritePin(OFF_GPIO_Port, OFF_Pin, 0);
					memset(cmdBuf2, 0x00, RX_BUFFER_SIZE);
					bCmdFlag = 0;
				 }
				 else if(cmdBuf2[0] == 'R' && cmdBuf2[1] == 'E' && cmdBuf2[2] == 'S' && cmdBuf2[3] == ' ') //RES
	    	      {
	    	    	args = sscanf(cmdBuf2, "RES %d\r\n", &iRes);
	    			if(args == 1 && iRes >= 0 && iRes < 9)
	    			{
	    			  SetR(iRes);
	    			  sprintf(str2, "RES=%d OK!\r\n", iRes);
	    			  SendString2(str2);
	    			}
	    			else
	    			{
	    			  SendString2("Error!\r\n");
	    			}
	    			memset(cmdBuf2, 0x00, RX_BUFFER_SIZE);
	    			bCmdFlag2 = 0;
	    	      }
	    	      else if(cmdBuf2[0] == 'C' && cmdBuf2[1] == 'A' && cmdBuf2[2] == 'P' && cmdBuf2[3] == ' ') //CAP
	    		  {
	    			args = sscanf(cmdBuf2, "CAP %d\r\n", &iCap);
	    			if(args == 1 && iCap >= 0 && iCap < 11)
	    			{
	    			  SetC(iCap);
	    			  sprintf(str2, "CAP=%d OK!\r\n", iCap);
	    			  SendString2(str2);
	    			}
	    			else
	    			{
	    			  SendString2("Error!\r\n");
	    			}
	    			memset(cmdBuf2, 0x00, RX_BUFFER_SIZE);
	    			bCmdFlag2 = 0;
	    		  }
	    	      else if(cmdBuf2[0] == 'Z' && cmdBuf2[1] == 'W' && cmdBuf2[2] == 'R' && cmdBuf2[3] == 'I' && cmdBuf2[4] == 'T' && cmdBuf2[5] == 'E') //ZWRITE
	    		  {
	    			args = sscanf(cmdBuf2, "ZWRITE %d %f %f\r\n", &iZIndex, &fRe, &fIm);
	    			if(args == 3 && iZIndex >= 0 && iZIndex < 32)
	    			{
	    			  LoadSettings();
	    			  Settings.Z[iZIndex].Re = fRe;
	    			  Settings.Z[iZIndex].Im = fIm;
	    			  SaveSettings();
	    			  sprintf(str2, "WRITE Z%d OK!\r\n", iZIndex);
	    			  SendString2(str2);
	    			}
	    			else
	    			{
	    			  SendString2("Error!\r\n");
	    			}
	    			memset(cmdBuf2, 0x00, RX_BUFFER_SIZE);
	    			bCmdFlag2 = 0;
	    		  }
	    	      else if(cmdBuf2[0] == 'Z' && cmdBuf2[1] == 'R' && cmdBuf2[2] == 'E' && cmdBuf2[3] == 'A' && cmdBuf2[4] == 'D') //ZREAD
	    		  {
	    			args = sscanf(cmdBuf2, "ZREAD %d\r\n", &iZIndex);
	    			if(args == 1 && iZIndex >= 0 && iZIndex < 32)
	    			{
	    			  LoadSettings();
	    			  sprintf(str2, "Z%d %f %f\r\n", iZIndex, Settings.Z[iZIndex].Re, Settings.Z[iZIndex].Im);
	    			  SendString2(str2);
	    			}
	    			else
	    			{
	    			  SendString2("Error!\r\n");
	    			}
	    			memset(cmdBuf2, 0x00, RX_BUFFER_SIZE);
	    			bCmdFlag2 = 0;
	    		  }
	    	      else if(cmdBuf2[0] == 'Z' && cmdBuf2[1] == 'E' && cmdBuf2[2] == 'R' && cmdBuf2[3] == 'A' && cmdBuf2[4] == 'S' && cmdBuf2[5] == 'E') //ZERASE
	    		  {
	    	    	for(int i=0;i<32;i++)
	    	    	{
	    	    		Settings.Z[i].Im = 0;
	    	    		Settings.Z[i].Re = 0;
	    	    	}
	    	    	SaveSettings();
	    	    	SendString2("ERASE OK!\r\n");
	    			memset(cmdBuf2, 0x00, RX_BUFFER_SIZE);
	    			bCmdFlag2 = 0;
	    		  }
	    	      else
	    	      {
	    	        memset(cmdBuf2, 0x00, RX_BUFFER_SIZE);
	    	        bCmdFlag2 = 0;
	    	        SendString2("Unknown command!\r\n");
	    	      }
	    	    }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI14|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL3;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC_Init(void)
{

  /* USER CODE BEGIN ADC_Init 0 */

  /* USER CODE END ADC_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC_Init 1 */

  /* USER CODE END ADC_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc.Instance = ADC1;
  hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;
  hadc.Init.ContinuousConvMode = DISABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.DMAContinuousRequests = DISABLE;
  hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel to be converted.
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC_Init 2 */

  /* USER CODE END ADC_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 480;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 1000;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 480;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 10000-1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 4800;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 10-1;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * @brief TIM15 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM15_Init(void)
{

  /* USER CODE BEGIN TIM15_Init 0 */

  /* USER CODE END TIM15_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM15_Init 1 */

  /* USER CODE END TIM15_Init 1 */
  htim15.Instance = TIM15;
  htim15.Init.Prescaler = 480;
  htim15.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim15.Init.Period = 10000-1;
  htim15.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim15.Init.RepetitionCounter = 0;
  htim15.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim15) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim15, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim15, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM15_Init 2 */

  /* USER CODE END TIM15_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_RS485Ex_Init(&huart1, UART_DE_POLARITY_HIGH, 0, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, C5_Pin|C10_Pin|LED_LOW_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, R1_Pin|R2_Pin|R3_Pin|C3_Pin
                          |C4_Pin|C6_Pin|C7_Pin|C8_Pin
                          |C9_Pin|R4_Pin|R5_Pin|R6_Pin
                          |R7_Pin|R8_Pin|C1_Pin|C2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(OFF_GPIO_Port, OFF_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(TESTPIN_GPIO_Port, TESTPIN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : C5_Pin C10_Pin LED_LOW_Pin */
  GPIO_InitStruct.Pin = C5_Pin|C10_Pin|LED_LOW_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : R1_Pin R2_Pin R3_Pin C3_Pin
                           C4_Pin C6_Pin C7_Pin C8_Pin
                           C9_Pin R4_Pin R5_Pin R6_Pin
                           R7_Pin R8_Pin C1_Pin C2_Pin */
  GPIO_InitStruct.Pin = R1_Pin|R2_Pin|R3_Pin|C3_Pin
                          |C4_Pin|C6_Pin|C7_Pin|C8_Pin
                          |C9_Pin|R4_Pin|R5_Pin|R6_Pin
                          |R7_Pin|R8_Pin|C1_Pin|C2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : OFF_Pin */
  GPIO_InitStruct.Pin = OFF_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(OFF_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : TESTPIN_Pin */
  GPIO_InitStruct.Pin = TESTPIN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(TESTPIN_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
