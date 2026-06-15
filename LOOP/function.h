#pragma once
#include "variable.h"
#include <vector>
//для новой программы
void metro_convert(void *metrology_struct, vector <SONDE_PARAM> &param );
float  ro_dfi(SONDE_PARAM *param, float PH_from_loop);
V10 ro_dfi(vector <SONDE_PARAM> &sonde_param, V10 PH_from_loop);
void calc_PH_from_loop(vector <SONDE_PARAM> &param, vector <LOOP_PARAM> &loop_param, vector <V10> &PH_computed);
void calc_Ro_from_loop(vector <SONDE_PARAM> &param, vector <V10> &PH_computed, vector <V10> &Ro_computed);
// Идентификация прибора, декодированная из поля signature структуры данных/метрологии.
// toolCode = low20(signature)/1000 = XYZ: X=тип прибора, Y=число передатчиков, Z=модификация.
struct TOOL_IDENTITY {
	uint32_t signature;     // исходная сигнатура
	uint32_t device_type;   // 1 = Автономный, 2 = LWD, 0 = неизвестно
	uint32_t n_tx;          // число передатчиков
	uint32_t modification;  // модификация прибора
	uint32_t number;        // порядковый номер прибора
	bool     valid;         // признак корректно распознанной сигнатуры
};

// Универсальное декодирование сигнатуры в идентификацию прибора.
TOOL_IDENTITY decode_tool_signature(uint32_t signature);

// Чтение сигнатуры прибора командой CMD_GET_SIGNATURE (0x16). Возврат 0 при успехе.
int read_tool_signature(System::IO::Ports::SerialPort^ COM, uint32_t &signature);

// Универсальный опрос кадра данных прибора командой CMD_GET_DATA (0x11).
// Число передатчиков и раскладка фаз определяются из сигнатуры кадра.
V10 get_ph_from_tool(System::IO::Ports::SerialPort^ COM, int mode, int &res);

// Специализированные функции опроса по типу прибора.
// Порт должен быть открыт вызывающим кодом; данные функции не управляют жизнью порта.
V10 get_ph_from_SDR(System::IO::Ports::SerialPort^ COM, int mode, int &res);
V10 get_ph_from_1DDS(System::IO::Ports::SerialPort^ COM, int mode, int &res);
V10 get_ph_from_AUTONOM(void *ComPort, int mode, int &res);
//вычисляет сигнал от кольца  
V10 calc_phase_from_loop(GP_TOOL_METROLOGY *metrology_struct, double R, double R_Add_Wire, double *Re, double L, double C, double r, bool mode);
	double SIGNAL(GP_TOOL_METROLOGY *metrology, uint16_t n, double ro);
int  RO (GP_TOOL_METROLOGY *metrology, uint16_t n, V10 *Signal_from_loop, V10 *Ro_calculated_adecvat_r_loop);
V10  R0_adecvat_r_loop(GP_TOOL_METROLOGY *metrology, V10 *Signal_from_loop);
V10 get_phase_smt(GP_TOOL_DATA *P, GP_TOOL_METROLOGY *metrology);
V10 get_phase(GP_TOOL_DATA *P, GP_TOOL_METROLOGY *metrology);
V10 get_phase_raw(GP_TOOL_DATA *P);
V10 get_air_smt(V10 *S, GP_TOOL_METROLOGY *metrology);
/////////////////////////////////////////////////////////////////////////////////////
complex <double> Z(double R, double L, double C, double f);

double L(double r, double Rd);

double L(double L);

complex <double> Loop(double R, double C, double L, double r, double f, double L1, double L2);

complex <double> Loop_Re(double R, double Re, double r, double f, double L1, double L2);

double Re(double f, double L_sonde, double r, double L1, double L2);

double C_max_fi(double f, double r, double L, double L1, double L2);

double F(double f, double r, double L, double L1, double L2);

double C_f(double L, double f);

uint16_t CRC16(const BYTE *nData, uint16_t wLength);

int send_to_tool(System::IO::Ports::SerialPort^ COM, uint8_t CMD, uint8_t* DATA, uint8_t length);

int receive_from_tool(System::IO::Ports::SerialPort^ COM, uint8_t CMD, uint8_t* DATA, uint16_t length);

GP_TOOL_DATA READ_BLOK_GP_TOOL_DATA(ifstream &fin, int n);

ARGB rgb(double ratio);