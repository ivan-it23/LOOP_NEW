#pragma once
#include "variable.h"
#include <vector>
//для новой программы
void metro_convert(void *metrology_struct, vector <SONDE_PARAM> &param );
float  ro_dfi(SONDE_PARAM *param, float PH_from_loop);
V10 ro_dfi(vector <SONDE_PARAM> &sonde_param, V10 PH_from_loop);
void calc_PH_from_loop(vector <SONDE_PARAM> &param, vector <LOOP_PARAM> &loop_param, vector <V10> &PH_computed);
void calc_Ro_from_loop(vector <SONDE_PARAM> &param, vector <V10> &PH_computed, vector <V10> &Ro_computed);
V10 get_ph_from_SDR(System::IO::Ports::SerialPort^ COM, int mode, int &res);
V10 get_ph_from_1DDS(System::IO::Ports::SerialPort^ COM, int mode, int &res);
V10 get_ph_from_AUTONOM(void *ComPort, int mode, int &res);
//вычисляет сигнал от кольца  
V8 calc_phase_from_loop(METROLOGY *metrology_struct, double R, double R_Add_Wire, double *Re, double L, double C, double r, bool mode);
	double SIGNAL(METROLOGY *metrology, uint16_t n, double ro);
int  RO (METROLOGY *metrology, uint16_t n, V8 *Signal_from_loop, V8 *Ro_calculated_adecvat_r_loop);
V8  R0_adecvat_r_loop(METROLOGY *metrology, V8 *Signal_from_loop);
V8 get_phase_smt(AllData_1DDS *P, METROLOGY *metrology);
V8 get_phase(AllData_1DDS *P, METROLOGY *metrology);
V8 get_phase_raw(AllData_1DDS *P);
V8 get_air_smt(V8 *S, METROLOGY *metrology);
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

AllData_1DDS READ_BLOK_AllData_1DDS(ifstream &fin, int n);

ARGB rgb(double ratio);