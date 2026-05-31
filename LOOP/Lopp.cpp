
//#include <windows.h>
//#include <tchar.h>
//#include <fstream>
//#include <iostream>
//#include <iomanip>
//#include <cmath>
//#include <complex> 
//#include <string> 
//#include "variable.h"
//#include "function.h"
//#include <typeinfo.h>
//#include <bitset>
//#include <vector>
//#include "winuser.h"
//#include "MetroForm.h"
#include "MyForm.h"
#include "WorkForm.h"

#pragma warning(disable:4244)
using namespace std;
using namespace System;
using namespace System::Windows::Forms;
//using namespace System::Runtime::InteropServices;
//using namespace System::Windows::Forms::DataVisualization::Charting;

////new///////////////////////////////////////////////////////////////
vector <SONDE_PARAM> sonde_param;
vector <LOOP_PARAM> loop_param;
vector <V10> PH_computed; 
vector <V10> Ro_computed;
V10 AIR = {0.0f,};
V10 ph_data;
V10 ro_data;

// 10 - зонды 20 - усреднение
int PH_percent_from_sonde[10][20];
int Ro_percent_from_sonde[10][20];


HINSTANCE AMK_DLL;// Объявляем идентификатор библиотеки
StartComPort  AMK_StartComPort;  // Объявляем указатель на  функцию AMK_StartComPort
GetCurrState AMK_GetCurrState;
Init AMK_Init;
BeginCyclogram AMK_BeginCyclogram;
GetData AMK_GetData;
SetIdle AMK_SetIdle;
PwrOn AMK_PwrOn;
PwrOff AMK_PwrOff;
DeleteComPort AMK_DeleteComPort;
CheckConnect AMK_CheckConnect; 
StopDDS3 AMK_StopDDS3;
ResumeDDS3 AMK_ResumeDDS3;
SetIQPhaseOffset AMK_SetIQPhaseOffset;
DDS3FreqOffset AMK_DDS3FreqOffset;
GetMetroData AMK_GetMetroData;
SetMetroData AMK_SetMetroData;
GetSerial AMK_GetSerial;

string Data_Path;
string Metrology_Name = "";
string Com_Name = "";
string Com1_Name = "";
string Com_LWD_Name = "";
string  Metrology_Path;

METROLOGY metrology_struct;
METROLOGY_FOR_AUTONOM metrology_struct_for_autonom; 
vector <AllData_1DDS> Temp_data;
V8 Air_Phase;
V8 Ro_percent_temperature[200][10] = { 0.0, };
complex<double> SGN[20] ;

int version_now, version_new;
unsigned int Tick = 800;
unsigned int Points = 100;
int norm[10];
int medium[10];
int out_of_norm[10];
double R_loop_physical[9];
double loop_radius;
bool AM_TX;
bool mode = 0;
bool Z_downloaded_from_loop = 0;
bool sonde_ro_calk;
bool comp_smt;
bool metrology_is_set = false;
bool sonde_on_off;
bool LWDsonde_on_off;
bool LWD_1DDS_sonde_on_off;
bool stop_working = 0;
bool metrology_is_captured_from_sonde = false;
bool LocalMetro_working;
bool LWD_1DDS = false;
bool express;
bool Ro_to_raport;
bool PH_to_raport;
bool param_form_closed;
bool metro_check;

int min_n;//начальная и конечная точки на первом графике
int max_n;//
void *ComPort = NULL;
const char* Metrology1 = "Metrology.bin";
int N_aver ;// количество точек для сбора данных
///////////////////////////////////////////////////////////////
int freq0;
int D_freq;
[STAThread]
//int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int){
int main() {
	//LOOP::MyForm SONDE_OKR;
	//Application::Run(%SONDE_OKR);
	LOOP::WorkForm SONDE;
	Application::Run(%SONDE);
	getchar(); getchar();
	return 0;
}