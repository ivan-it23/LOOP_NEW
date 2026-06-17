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

// Протокол обмена с прибором.
// Пакет: [CMD:1][LEN:2][PAYLOAD:N][CRC:2], где LEN = полная длина пакета (N + 5).
// 0x16 запрос сигнатуры прибора (ответ: 4 байта signature)
#define CMD_GET_SIGNATURE 0x16
// 0x13 чтение метрологии прибора (ответ: 240 байт GP_TOOL_METROLOGY)
#define CMD_GET_METROLOGY 0x13
// 0x12 запись метрологии прибора (payload: 240 байт GP_TOOL_METROLOGY)
#define CMD_SET_METROLOGY 0x12

// Единая скорость UART для всех приборов унифицированного протокола.
#define TOOL_BAUDRATE 125000

// Тип прибора кодируется первой цифрой toolCode из сигнатуры.
#define DEVTYPE_AUTONOM 1
#define DEVTYPE_LWD     2


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

// Универсальная структура данных прибора для 3/4/5 передатчиков (240 байт).
// Индексация: [freq][Tx], freq 0 = 400 кГц, 1 = 2000 кГц; Tx 0..4 = T1..T5.
struct GP_TOOL_DATA {// 240 byte
	uint32_t signature;
	uint32_t condition;
	uint32_t frame;
	float temperature;
	float rho_smt[2][5];// УЭС, рассчитанные на контроллере [400, 2000][T1-T5]
	float phase_smt[2][5];// симметризованные фазы [400, 2000][T1-T5]
	float AM_RX_1[2][5];// амплитуды на первом приемнике [400, 2000][T1-T5]
	float ZERO_AM_RX_1[2];// амплитуды на первом приемнике [400, 2000] при молчащих передатчиках
	float AM_RX_2[2][5];// амплитуды на втором приемнике [400, 2000][T1-T5]
	float ZERO_AM_RX_2[2];// амплитуды на втором приемнике [400, 2000] при молчащих передатчиках
	float DELTA_PH[2][5];// сырая разница фаз [400, 2000][T1-T5]
	float ZERO_dPH[2];// разница фаз молчащих передатчиков [400, 2000]
};

// Универсальная структура метрологии прибора для 3/4/5 передатчиков (240 байт).
struct GP_TOOL_METROLOGY {// 240 byte
	uint32_t signature;
	uint32_t serial;
	uint16_t L1[5];// расстояние до ближнего приемника, T1-T5
	uint16_t L2[5];// расстояние до дальнего приемника, T1-T5
	uint16_t F[2];// частоты [400, 2000]
	int16_t Air_zz[2][5];// воздушные поправки фаз [400, 2000][T1-T5]
	int16_t Air_zz_amt[2][5];// пороги амплитуд [400, 2000][T1-T5]
	uint32_t D_sonde_mm;// диаметр зонда
	uint32_t work_type;// тип работы прибора
	uint32_t Rx_Position;// 0 - DEFAULT R1->T1, 1 - R1->T2
	uint16_t service[78];// на будущее до 240 байт !!!!
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
