#pragma once
#include "windows.h"
#include <cmath>
#include <complex> 
#define КEY (0x01 << 4)
// 0x05 получить кадр данных измерений 5 Tx 
#define CMD_GET_DATA_5_Tx 0x05 | КEY
// 0x01 получить кадр данных измерений SDR урезаный
#define CMD_GET_DATA 0x01 | КEY
#define ADDRESS (6 << 4)
// 0xF5 установка времени задержки, если получили 0 – то это команда выключения прибора 
#define CMD_TIME_SYNC 0xF5
// 0xFA внешняя синхронизация внутренних часов СП во время работы
#define CMD_BEACON 0xFA
// 0xFD режим повышенной скорости приема-передачи данных по UART
#define CMD_TURBO 0xFD
// 0x01 чтение флэш-памяти
#define CMD_ERAM 0x01 | ADDRESS
// 0x02 чтение информации об устройстве. Так называемые, «метаданные». Описание далее
#define CMD_INFO 0x02 | ADDRESS
// 0x0E обработка ошибок СП 
#define CMD_ERR 0x0E | ADDRESS   
// 0x05 чтение EEPROM памяти (в EEPROM храним калибровки и т.д.)
#define CMD_READ_EE 0x05 | ADDRESS
// 0x06 запись EEPROM памяти
#define CMD_WRITE_EE 0x06 | ADDRESS
// 0x07 Опрос текущего статуса и данных СП 
#define CMD_WORK 0x07 | ADDRESS
// 0x08 bootloader. Для прошивки программного обеспечения микроконтроллера.
#define CMD_BOOT 0x08 | ADDRESS

# define LWD1DDS 0
# define SDR 1
# define AUTONOM 2


using namespace std;

const double  PI = 3.1415927410125732;
const double eps0 = 8.85*1e-12;
const double mu0 = 4 * PI*1e-7;
const double mV = 2500 / pow(2, 28);
const double mV_Tx = PI * 330 / pow(2, 14);
const double mG = 1000 * 180 / PI;
const double Grad = 180 / PI;

typedef void*(__stdcall *StartComPort)(void **, HWND);// Объявляем тип для указателя на  функцию Sonde_set
typedef unsigned long(__stdcall *GetCurrState)(void *);
typedef unsigned long(__stdcall *Init)(void *);
typedef unsigned long(__stdcall *BeginCyclogram)(void *);
typedef unsigned long(__stdcall *GetData)(void *, void *);
typedef unsigned long(__stdcall *SetIdle)(void *);
typedef unsigned long(__stdcall *PwrOn)(void *);
typedef unsigned long(__stdcall *PwrOff)(void *);
typedef void(__stdcall *DeleteComPort)(void *);
typedef unsigned long(__stdcall *StopDDS3)(void *);
typedef unsigned long(__stdcall *ResumeDDS3)(void *);
typedef unsigned long(__stdcall *SetIQPhaseOffset)(void *, void *);
typedef unsigned long(__stdcall *DDS3FreqOffset)(void *, void *);
typedef unsigned long(__stdcall *GetMetroData)(void *, void *);
typedef unsigned long(__stdcall *SetMetroData)(void *, void *);
typedef unsigned long(__stdcall *GetSerial)(void *, void *);  // это указатель на 16-разрядное целое
typedef unsigned long(__stdcall *CheckConnect)(void); // функция не зависит от запуска устройства и показывает
												  // фактическое количество УСИ, подключенных к компьютеру.
												  // Если число их не равно единице - есть смысл что-то предпринять


//Параметры зондов 
//L1 Расстояние между ближним приемником и передатчиком 
//L2 Расстояние между дальним приемником и передатчиком

struct V8  {
	float VALUE[8];
};

struct V10 {
	float VALUE[10];
};

enum R {
	Z1, Z2, X1, Y1, X2, Y2
};

#define int16_t short
#define int32_t long
#define uint32_t unsigned long
#define Complex complex<long int>
#define uint16_t unsigned short

#pragma pack(push, 1)

// Структура калибровки
struct INC_SET {
	uint32_t MA_WINDOW_SIZE; //32
	uint32_t MLD_WINDOW_SIZE; //32
	uint32_t N; //128
	uint32_t K; //128
	uint32_t auto_delta; //1
	uint32_t INTERRUPT_BY_ANGLE_PATH; // 0
	float W_MIN; //3.14
	float W_MAX; //18.84
	float K_predict; //0.5
	float APS_DELTA; //PI/360
	float history_angle; //(20*PI)/180
	float K_delta; //2.5
};

struct	METROLOGY {
	uint32_t signature;
	uint32_t serial;
	uint16_t L1[8];//
	uint16_t L2[8];//
	uint16_t F[8];//
	int16_t Air_zz[8];//
	uint16_t PhaseOffsets[4];//сдвиг между квадратурами
	uint32_t D_sonde_mm;
	int16_t Air_zz_amt[8];//
	INC_SET inc_set;
	uint8_t service[92];//на будущее до 240 байт !!!!
};

struct	METROLOGY_NEW {
	uint32_t signature;
	uint32_t serial;
	uint16_t L1[5];//
	uint16_t L2[5];//
	uint16_t F[2];//
	int16_t Air_zz_400[5];//
	int16_t Air_zz_2000[5];//
	int16_t min_zz_amt_400[5];//
	int16_t min_zz_amt_2000[5];//
	int16_t freq_PWM_on;
	int16_t pow_save_mode;
	uint16_t D_sonde_mm;
	int16_t add[80];
};

struct	METROLOGY_FOR_AUTONOM {
	uint32_t signature;
	uint32_t serial;
	uint16_t L1[8];//
	uint16_t L2[8];//
	uint16_t F[8];//
	int16_t Air_zz[8];//	
	int16_t Air_zz_amt[8];//
	int16_t high_temp[8];
	int16_t Air_zz_high_temp[8][8];//	
	int16_t add[4] = { 0, };
};

struct AllData_1DDS {
	uint32_t signature;
	uint32_t condition;
	float Ro_smt[8];
	float PH_smt[8];
	uint32_t frame;
	float temperature;
	float AM_RX_1[10];
	float AM_RX_2[10];
	float PH_RX_1[10];
	float PH_RX_2[10];
	//uint8_t add[6];
};

struct DATA_5_TX {// 616 byte
	uint32_t signature;
	uint32_t condition;
	uint32_t frame;
	float temperature;

	float Ro_smt_400[5];// УЭС, рассчитанные на контроллере
	float Ro_smt_2000[5];//

	float PH_smt_400[5];// симметризованные фазы
	float PH_smt_2000[5];//

	float DELTA_PH_400[5];// сырая разница фаз
	float DELTA_PH_2000[5];// она же разница фаз первой гармоники

	float AM_RX_1_400[5];// амплитуды первой гармоники
	float AM_RX_1_2000[5];// они же амплитуды сигнала
	float AM_RX_2_400[5];//на приемниках для пяти передатчиков
	float AM_RX_2_2000[5];//

	float AM_0_RX_1_400;//амплитуды шума первой гармоники
	float AM_0_RX_1_2000;//они же амплитуды шума в полосе приема
	float AM_0_RX_2_400;// при молчащих передатчиках
	float AM_0_RX_2_2000;//


	//debug and tuning
	//5 - передатчиков 4 гармоники
	// в массиве по порядку 0,2,3,4 я гармоники
	float AM_RX_1_400_H[5][4];
	float AM_RX_1_2000_H[5][4];

	float AM_RX_2_400_H[5][4];
	float AM_RX_2_2000_H[5][4];

	float AM_0_RX_1_400_H[4];
	float AM_0_RX_1_2000_H[4];

	float AM_0_RX_2_400_H[4];
	float AM_0_RX_2_2000_H[4];
};

struct DAT {
	DATA_5_TX GP;
};

struct ARGB
{
	int A;
	int R;
	int G;
	int B;
};

struct SONDE_PARAM {
	float L1; 
	float L2; 
	float F;
};

struct LOOP_PARAM {
	float R; // активное сопротивление
	float R_Add_Wire;// сопротивление провода переменному току
	float Re; // реактивное сопротивление
	float r;  //радиус
};

#pragma pack(pop)
