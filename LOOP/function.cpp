#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "function.h"
#include <vector>
using namespace System;

extern GP_TOOL_METROLOGY metrology_struct;
extern PHASE_ORIENTATION_DEBUG phase_orientation_debug;

// Декодирование сигнатуры прибора.
// low20 = младшие 20 бит сигнатуры; toolCode = low20/1000 = XYZ:
//   X (тип): low20/100000, Y (число Tx): (low20%100000)/10000, Z (мод): (low20%10000)/1000.
TOOL_IDENTITY decode_tool_signature(uint32_t signature) {
	TOOL_IDENTITY id = { 0 };
	id.signature = signature;
	uint32_t low20 = (signature << 12) >> 12;
	id.device_type = low20 / 100000;
	id.n_tx = (low20 % 100000) / 10000;
	id.modification = (low20 % 10000) / 1000;
	id.number = low20 % 1000;
	id.valid = (id.device_type == DEVTYPE_AUTONOM || id.device_type == DEVTYPE_LWD)
	           && (id.n_tx >= 3 && id.n_tx <= 5);
	if (id.n_tx < 3 || id.n_tx > 5) id.n_tx = 5;
	return id;
}

static float normalize_phase(float phase) {
	while (phase > PI) phase -= 2 * PI;
	while (phase <= -PI) phase += 2 * PI;
	return phase;
}

static uint32_t normalized_rx_position(uint32_t rx_position) {
	return (rx_position == 1) ? 1 : 0;
}

static int rx_phase_orientation_sign(uint32_t Tx, uint32_t rx_position) {
	const uint32_t rx = normalized_rx_position(rx_position);
	const bool odd_tx_number = ((Tx + 1) & 1) != 0; // T1, T3, T5
	bool invert = !odd_tx_number; // Rx_Position 0: R1->T1: T2/T4 = RX1-RX2
	if (rx == 1) invert = !invert; // Rx_Position 1: R1->T2 
	return invert ? -1 : 1;
}

static float apply_rx_phase_orientation(float raw_phase, float air_phase, uint32_t Tx, uint32_t rx_position) {
	float phase = normalize_phase(raw_phase) - air_phase;
	if (rx_phase_orientation_sign(Tx, rx_position) < 0) phase = -phase;
	return normalize_phase(phase);
}

static void fill_phase_from_tool_data(GP_TOOL_DATA *data, V10 &ph_data) {
	for (int i = 0; i < 10; i++)
		ph_data.VALUE[i] = 0;

	if (data == nullptr)
		return;

	TOOL_IDENTITY id = decode_tool_signature(data->signature);
	const uint32_t rx_position = normalized_rx_position(metrology_struct.Rx_Position);
	phase_orientation_debug = {};
	phase_orientation_debug.valid = true;
	phase_orientation_debug.signature = data->signature;
	phase_orientation_debug.n_tx = id.n_tx;
	phase_orientation_debug.rx_position = rx_position;
	for (uint32_t freq = 0; freq < 2; freq++) {
		for (uint32_t Tx = 0; Tx < id.n_tx; Tx++) {
			uint32_t idx = freq * id.n_tx + Tx;
			const int sign = rx_phase_orientation_sign(Tx, rx_position);
			ph_data.VALUE[idx] = apply_rx_phase_orientation(data->DELTA_PH[freq][Tx], 0.0f, Tx, rx_position);
			phase_orientation_debug.raw_delta[freq][Tx] = data->DELTA_PH[freq][Tx];
			phase_orientation_debug.raw_normalized[freq][Tx] = normalize_phase(data->DELTA_PH[freq][Tx]);
			phase_orientation_debug.orientation_sign[freq][Tx] = sign;
			phase_orientation_debug.final_phase[freq][Tx] = ph_data.VALUE[idx];
		}
	}
}

// Конвертация метрологии в параметры зондов.
void metro_convert(void *metrology_struct, vector <SONDE_PARAM> &sonde_param) {
	GP_TOOL_METROLOGY metrology = *(GP_TOOL_METROLOGY*)metrology_struct;
	TOOL_IDENTITY id = decode_tool_signature(metrology.signature);

	sonde_param.clear();

	if (id.valid) {
		for (int freq = 0; freq < 2; freq++) {
			for (uint32_t Tx = 0; Tx < id.n_tx; Tx++) {
				SONDE_PARAM buff_sp;
				buff_sp.L1 = (metrology.L1[Tx] / 1000.0);
				buff_sp.L2 = (metrology.L2[Tx] / 1000.0);
				buff_sp.F = (metrology.F[freq] * 1000.0);
				sonde_param.push_back(buff_sp);
			}
		}
	}
}

// Фаза от кольца.
float dfi_from_loop(SONDE_PARAM *param, LOOP_PARAM *loop_param) {
	complex<float>ZFull;
	if(param->F < 1000000)
		ZFull = (loop_param->R + 0.3f) + 1if*loop_param->Re;
	if (param->F > 1000000)
		ZFull = (loop_param->R + 0.5f) + 1if*loop_param->Re;
	cout << "param->F " << param->F << endl;
	float piomegamu0 = (0.00002480502 * param->F);
	float Q = pow((pow((param->L2 + param->L1) / 2, 2) + pow(loop_param->r, 2)) * (pow((param->L2 - param->L1) / 2, 2) + pow(loop_param->r, 2)), 1.5);
	return arg(((2.0f * 1if*ZFull * Q + (float)(piomegamu0 * pow(loop_param->r, 4) * pow(param->L1, 3)))*(float)pow(param->L2, 3)) /
		       ((2.0f * 1if*ZFull * Q + (float)(piomegamu0 * pow(loop_param->r, 4) * pow(param->L2, 3)))*(float)pow(param->L1, 3)));
	
}

// Фаза от среды.
float dfi_ro(SONDE_PARAM *param, float ro) {
	float omegamu0sigma = (0.0000078957f * param->F) / ro;
	complex <float> ik = 1if * sqrt(1if * omegamu0sigma);
	complex <float> SGN = exp(ik*(param->L2 - param->L1)) * ((1.0f - ik * param->L2) / (1.0f - ik * param->L1));
	return arg(SGN);
}
// УЭС от фазы.
float  ro_dfi(SONDE_PARAM *param, float PH_from_loop) {
	float Ro0;
	float epsilon_ARG = 0.0000005;
	float epsilon_INF = 0.000000005;
	float  Ro_0 = 0.01, Ro_max = 7000.0;
	bool stop_epsl_ARG = false;
	bool stop_epsl_INF = false;
	float  ro_0 = Ro_0;
	float  ro_max = Ro_max;

	do {
		float X1 = ro_0 + 0.382*(ro_max - ro_0);
		float X2 = ro_max - 0.382*(ro_max - ro_0);
		float A = PH_from_loop - dfi_ro(param, X1);
		float B = PH_from_loop - dfi_ro(param, X2);
		if (fabs(A) > fabs(B)) { ro_0 = X1; }
		else { ro_max = X2; }
		if (fabs(A) < epsilon_ARG || fabs(B) < epsilon_ARG) { stop_epsl_ARG = true; }
		else if (fabs(A - B) < epsilon_INF) { stop_epsl_INF = true; }

	} while (stop_epsl_ARG == false && stop_epsl_INF == false);
	if (stop_epsl_ARG == true) Ro0 = (ro_0 + ro_max) / 2;
	if (stop_epsl_INF == true) Ro0 = 7200;//-100
	ro_0 = Ro_0;
	ro_max = Ro_max;
	return   Ro0;
}
// Кажущееся УЭС по сигналам прибора.
V10 ro_dfi(vector <SONDE_PARAM> &sonde_param, V10 PH_from_loop) {
	V10 ro_data;
	for (int i = 0; i < 10; i++)
		ro_data.VALUE[i] = 0;
	for (int i = 0; i < sonde_param.size(); i++)
		ro_data.VALUE[i] = ro_dfi(&sonde_param[i], PH_from_loop.VALUE[i]);
	return ro_data;
}

// Расчет фаз по сопротивлениям кольца.
 void calc_PH_from_loop(vector <SONDE_PARAM> &sonde_param, vector <LOOP_PARAM> &loop_param, vector <V10> &PH_computed) {
	 PH_computed.clear();
	 V10 buff = { 0.0f, };
	 PH_computed.push_back(buff);
	 for (int R_idx = 0; R_idx < 8; R_idx++) {
	
		 LOOP_PARAM loop_param_local;
		 loop_param_local.R = loop_param[R_idx].R;	
		 loop_param_local.r = loop_param[R_idx].r;
		 loop_param_local.R_Add_Wire = loop_param[R_idx].R_Add_Wire;
		 for (int i = 0; i < sonde_param.size(); i++) {
			 loop_param_local.Re = loop_param[i].Re;
			 buff.VALUE[i] = dfi_from_loop(&sonde_param[i], &loop_param_local);
		 } 
		  PH_computed.push_back(buff);
	 }
 }

// Расчет УЭС по фазам кольца.
 void calc_Ro_from_loop(vector <SONDE_PARAM> &sonde_param, vector <V10> &PH_computed, vector <V10> &Ro_computed) {
	 Ro_computed.clear();
	 for (int R_idx = 0; R_idx < PH_computed.size(); R_idx++) {
		 V10 buff;
		 for (int i = 0; i < sonde_param.size(); i++)
			 buff.VALUE[i] = ro_dfi(&sonde_param[i], PH_computed[R_idx].VALUE[i]);
		 Ro_computed.push_back(buff);
	 }
 }

V10 get_ph_from_SDR(System::IO::Ports::SerialPort^ COM, int mode, int &res) {
    V10 ph_data;
	for (int i = 0; i < 10; i++) 
	    ph_data.VALUE[i] = 0;
		
	GP_TOOL_DATA *All_Data_SDR = new GP_TOOL_DATA;
	const uint16_t length = 242;

	uint8_t CMD = 0;
	uint8_t DATA_IN[length] = { 0, };
	int result;
	bool stop = false;

	// Опрос выполняется только на открытом порту.
	if (!COM->IsOpen) {
		res = 3;
		Console::WriteLine("no port 112");
		return ph_data;
	}
	COM->ReadTimeout = 100;
	COM->WriteTimeout = 100;

	uint8_t DATA[2] = { 0x05 ,0x00 };
	if (send_to_tool(COM, CMD_GET_DATA, DATA, 2) == 1) {
		Console::WriteLine("Соединение с прибором прервано переподключите !");
	}
	Sleep(50);
	result = receive_from_tool(COM, CMD, DATA_IN, length);
	res = result;
	if (result == 0) {
		All_Data_SDR = (GP_TOOL_DATA*)(DATA_IN + 2);
		fill_phase_from_tool_data(All_Data_SDR, ph_data);
	}

	return ph_data;
}

V10 get_ph_from_1DDS(System::IO::Ports::SerialPort^ COM, int mode, int &res) {
	V10 ph_data;
	for (int i = 0; i < 10; i++)
		ph_data.VALUE[i] = 0;

	GP_TOOL_DATA *All_Data_1DDS = new GP_TOOL_DATA;
	const uint16_t length = 242;
	uint8_t CMD = 0;
	uint8_t DATA_IN[length] = { 0, };
	int result;
	bool stop = false;

	// Опрос выполняется только на открытом порту.
	if (!COM->IsOpen) {
		res = 3;
		Console::WriteLine("no port 112");
		return ph_data;
	}
	COM->ReadTimeout = 200;
	COM->WriteTimeout = 200;

	uint8_t DATA[2] = { 0x05, 0x00 };
	if (send_to_tool(COM, CMD_GET_DATA, DATA, 2) == 1) {
		Console::WriteLine("Соединение с прибором прервано переподключите !");
	}
	Sleep(50);
	result = receive_from_tool(COM, CMD, DATA_IN, length);
	res = result;
	if (result == 0) {
		All_Data_1DDS = (GP_TOOL_DATA*)(DATA_IN + 2);
		fill_phase_from_tool_data(All_Data_1DDS, ph_data);

		cout << " SGN " << std::hex << All_Data_1DDS->signature << endl;
		cout << " condition " << std::dec << All_Data_1DDS->condition << endl;
		cout << " frame " << All_Data_1DDS->frame << endl;
		cout << "TEMP " << All_Data_1DDS->temperature << endl;
		cout << "Ro_400 " << All_Data_1DDS->rho_smt[0][0] << " " << All_Data_1DDS->rho_smt[0][1] << " " << All_Data_1DDS->rho_smt[0][2] << " " << All_Data_1DDS->rho_smt[0][3] << endl;
		cout << "Ro_2000 " << All_Data_1DDS->rho_smt[1][0] << " " << All_Data_1DDS->rho_smt[1][1] << " " << All_Data_1DDS->rho_smt[1][2] << " " << All_Data_1DDS->rho_smt[1][3] << endl;
	}

	return ph_data;
}

V10 get_ph_from_AUTONOM(void *ComPort, int mode, int &res) {
	V10 ph_data;
	for (int i = 0; i < 10; i++)
		ph_data.VALUE[i] = 0;

	GP_TOOL_DATA *All_Data_1DDS = new GP_TOOL_DATA;
	
	extern GetData AMK_GetData;
	extern HINSTANCE AMK_DLL;
	AMK_GetData = (GetData)GetProcAddress(AMK_DLL, "AMK_GetData");
	_controlfp(_CW_DEFAULT, 0xfffff);
	int result = AMK_GetData(ComPort, All_Data_1DDS) ;

	if (result == 1) {
		res = 0;
		fill_phase_from_tool_data(All_Data_1DDS, ph_data);
	}

	else res = 1;

	return ph_data;
}

// Чтение сигнатуры прибора.
int read_tool_signature(System::IO::Ports::SerialPort^ COM, uint32_t &signature) {
	uint8_t DATA[2] = { 0x05, 0x00 };
	if (send_to_tool(COM, CMD_GET_SIGNATURE, DATA, 2) != 0)
		return 1;

	Sleep(50);

	const uint16_t length = 6;
	uint8_t CMD = 0;
	uint8_t DATA_IN[length] = { 0 };
	int result = receive_from_tool(COM, CMD, DATA_IN, length);
	if (result != 0)
		return result;

	if (CMD != CMD_GET_SIGNATURE)
		return 3;

	memcpy(&signature, DATA_IN + 2, sizeof(uint32_t));
	return 0;
}

// Опрос кадра данных прибора.
V10 get_ph_from_tool(System::IO::Ports::SerialPort^ COM, uint8_t CMD, int mode, int &res) {
	V10 ph_data;
	for (int i = 0; i < 10; i++)
		ph_data.VALUE[i] = 0;

	const uint16_t length = 242;
	uint8_t recv_CMD = 0;
	uint8_t DATA_IN[length] = { 0 };
	int result;
	const int MAX_RETRIES = 1;

	if (!COM->IsOpen) {
		res = 3;
		Console::WriteLine("get_ph_from_tool: порт закрыт");
		return ph_data;
	}

	for (int retry = 0; retry <= MAX_RETRIES; retry++) {
		if (retry > 0) {
			Console::WriteLine("get_ph_from_tool: повторная попытка #{0}", retry);
		}

		Console::WriteLine("get_ph_from_tool: ReadTimeout={0} WriteTimeout={1} BytesToRead={2}",
							 COM->ReadTimeout, COM->WriteTimeout, COM->BytesToRead);

		COM->DiscardInBuffer();

		uint8_t DATA[2] = { 0x05, 0x00 };
		if (send_to_tool(COM, CMD, DATA, 2) != 0) {
			Console::WriteLine("get_ph_from_tool: ошибка отправки команды опроса");
			res = 1;
			return ph_data;
		}

		result = receive_from_tool(COM, recv_CMD, DATA_IN, length);
		res = result;

		if (result == 0) {
			GP_TOOL_DATA *All_Data = (GP_TOOL_DATA*)(DATA_IN + 2);
			fill_phase_from_tool_data(All_Data, ph_data);
			return ph_data;
		}

		if (result == 1 && retry < MAX_RETRIES) {
			Console::WriteLine("get_ph_from_tool: повтор после таймаута...");
			continue;
		}

		if (result != 0) {
			break;
		}
	}

	return ph_data;
}

// Симметризованная разница фаз от кольца.
V10 calc_phase_from_loop(GP_TOOL_METROLOGY *metrology_struct, double R, double R_Add_Wire, double *Re, double L, double C, double r, bool mode) {
	V10 phase_loop, phase_loop_smt;
	double L1[10], L2[10], F[10];

	uint32_t buff = (metrology_struct->signature << 12) >> 12;
	uint32_t N_Tx = (buff % 100000) / 10000;
	if (N_Tx < 3 || N_Tx > 5) N_Tx = 5;
	uint32_t N = 2 * N_Tx;

	for (uint32_t i = 0; i < N; i++) {
		uint32_t freq = i / N_Tx;
		uint32_t Tx = i % N_Tx;
		L1[i] = (metrology_struct->L1[Tx] / 1000.0) + 0.0000001;
		L2[i] = (metrology_struct->L2[Tx] / 1000.0) + 0.0000001;
		F[i] = (metrology_struct->F[freq] * 1000.0) + 0.0000001;
	}

	double omega[10];
	complex<double>ZFull[10];
	double Q[10];
	for (uint32_t i = 0; i < N; i++) {
		omega[i] = (2.0 * PI * F[i]);	
		if (mode == 0)
		    ZFull[i] = (R + R_Add_Wire) + 1i*(omega[i]*L - 1 / (omega[i]*C));
		if (mode == 1)
			ZFull[i] = (R + R_Add_Wire) + 1i*Re[i];
		Q[i] = pow((pow((L2[i] + L1[i]) / 2, 2) + r * r) * (pow((L2[i] - L1[i]) / 2, 2) + r * r), 1.5);
		phase_loop.VALUE[i] = arg(((2.0 * 1i*ZFull[i] * Q[i] + PI*omega[i]*mu0 * pow(r, 4) * pow(L1[i], 3))*pow(L2[i], 3)) / ((2.0 * 1i*ZFull[i] * Q[i] + PI*omega[i]*mu0 * pow(r, 4) * pow(L2[i], 3))*pow(L1[i], 3)));
	}
	
	return phase_loop;
}

double SIGNAL( GP_TOOL_METROLOGY *metrology, uint16_t n, double ro) {
	// Линейный индекс n соответствует freq = n / N_Tx, Tx = n % N_Tx.
	uint32_t buff = (metrology->signature << 12) >> 12;
	uint32_t N_Tx = (buff % 100000) / 10000;
	if (N_Tx < 3 || N_Tx > 5) N_Tx = 5;
	uint16_t freq = n / N_Tx;
	uint16_t Tx = n % N_Tx;
	double L1 = metrology->L1[Tx] / 1000.0;
	double L2 = metrology->L2[Tx] / 1000.0;
	double omegamu0sigma = (0.0078957 * metrology->F[freq]) / ro;
	complex <double> ik = 1i * sqrt(1i * omegamu0sigma);
	complex <double> SGN = exp(ik*(L2 - L1)) * ((1.0 - ik * L2) / (1.0 - ik * L1));
	return arg(SGN);
}

int  RO(GP_TOOL_METROLOGY *metrology, uint16_t n, V10 *Signal_from_loop, V10 *Ro_calculated_adecvat_r_loop) {
	float Ro0;
	float epsilon_ARG = 0.0000005;
	float epsilon_INF = 0.000000005;
	float  Ro_0 = 0.01, Ro_max = 7000.0;
	bool stop_epsl_ARG = false;
	bool stop_epsl_INF = false;
	float  ro_0 = Ro_0;
	float  ro_max = Ro_max;

	do {
		float X1 = ro_0 + 0.382*(ro_max - ro_0);
		float X2 = ro_max - 0.382*(ro_max - ro_0);
		float A = Signal_from_loop->VALUE[n] - SIGNAL(metrology, n, X1);
		float B = Signal_from_loop->VALUE[n] - SIGNAL(metrology, n, X2);
		if (fabs(A) > fabs(B)) { ro_0 = X1; }
		else { ro_max = X2; }
		if (fabs(A) < epsilon_ARG || fabs(B) < epsilon_ARG) { stop_epsl_ARG = true; }
		else if (fabs(A - B) < epsilon_INF) { stop_epsl_INF = true; }

	} while (stop_epsl_ARG == false && stop_epsl_INF == false);
	if (stop_epsl_ARG == true) Ro0 = (ro_0 + ro_max) / 2;
	if (stop_epsl_INF == true) Ro0 = 7200;
	ro_0 = Ro_0;
	ro_max = Ro_max;
	Ro_calculated_adecvat_r_loop->VALUE[n] = Ro0;
	return  0;
}

V10  R0_adecvat_r_loop(GP_TOOL_METROLOGY *metrology, V10 *Signal_from_loop) {
	V10 Ro_calculated_adecvat_r_loop = {0.0,};
	uint32_t buff = (metrology->signature << 12) >> 12;
	uint32_t N_Tx = (buff % 100000) / 10000;
	if (N_Tx < 3 || N_Tx > 5) N_Tx = 5;
	for (uint32_t i = 0; i < 2 * N_Tx; i++) {
		RO(metrology, i, Signal_from_loop, &Ro_calculated_adecvat_r_loop);
	}
	return   Ro_calculated_adecvat_r_loop;
}

V10 get_phase_raw(GP_TOOL_DATA *P) {
	V10 phase = { 0.0, };
	uint32_t buff = (P->signature << 12) >> 12;
	uint32_t N_Tx = (buff % 100000) / 10000;
	if (N_Tx < 3 || N_Tx > 5) N_Tx = 5;
	for (uint32_t freq = 0; freq < 2; freq++) {
		for (uint32_t Tx = 0; Tx < N_Tx; Tx++) {
			uint32_t idx = freq * N_Tx + Tx;
			phase.VALUE[idx] = normalize_phase(P->DELTA_PH[freq][Tx]);
		}
	}
	return phase;
}

V10 get_phase(GP_TOOL_DATA *P, GP_TOOL_METROLOGY *metrology) {
	V10 phase = { 0.0, };
	uint32_t buff = (P->signature << 12) >> 12;
	uint32_t N_Tx = (buff % 100000) / 10000;
	if (N_Tx < 3 || N_Tx > 5) N_Tx = 5;
	for (uint32_t freq = 0; freq < 2; freq++) {
		for (uint32_t Tx = 0; Tx < N_Tx; Tx++) {
			uint32_t idx = freq * N_Tx + Tx;
			phase.VALUE[idx] = apply_rx_phase_orientation(
				P->DELTA_PH[freq][Tx],
				metrology->Air_zz[freq][Tx] / mG,
				Tx,
				metrology->Rx_Position);
		}
	}
	
	return phase;
}

// ============================================================
// Симметризация фаз (логика портирована из Amk-Horizon-New-Chart).
// Матрица симметризации K выбирается по числу передатчиков N_Tx и по маске
// работающих передатчиков condition. Поддерживаются приборы с 3/4/5 Tx.
// Для приборов с N_Tx < 5 выходы для несуществующих передатчиков всегда нулевые.
// ============================================================

// Максимальное число передатчиков в раскладке матриц симметризации.
static const int SMT_MAX_TX = 5;

// Извлекает 8-битную маску работающих передатчиков для частоты из 32-битного
// поля condition. Layout (LSB-first): byte0 = 2000 кГц (freq 1), byte1 = 400 кГц (freq 0).
// Внутри байта: bit0 = T1, bit1 = T2, ..., bit4 = T5.
static uint8_t extract_condition_byte(uint32_t raw_condition, int freq_idx) {
	if (freq_idx == 0)
		return (uint8_t)((raw_condition >> 8) & 0xFFu);
	return (uint8_t)(raw_condition & 0xFFu);
}

static void zero_smt_matrix(float K[SMT_MAX_TX][SMT_MAX_TX]) {
	for (int r = 0; r < SMT_MAX_TX; r++)
		for (int c = 0; c < SMT_MAX_TX; c++)
			K[r][c] = 0.0f;
}

static void copy_smt_matrix(float dst[SMT_MAX_TX][SMT_MAX_TX], const float src[SMT_MAX_TX][SMT_MAX_TX]) {
	for (int r = 0; r < SMT_MAX_TX; r++)
		for (int c = 0; c < SMT_MAX_TX; c++)
			dst[r][c] = src[r][c];
}

// Identity по маске работающих передатчиков (T_t -> T_t при бите condition = 1).
// Используется как fallback для нераспознанных комбинаций condition.
static void fill_identity_symmetry_matrix(float K[SMT_MAX_TX][SMT_MAX_TX], uint8_t condition, int valid_tx_count) {
	zero_smt_matrix(K);
	for (int t = 0; t < valid_tx_count && t < SMT_MAX_TX; t++) {
		const bool working = ((condition >> t) & 0x1u) != 0;
		K[t][t] = working ? 1.0f : 0.0f;
	}
}

// Матрицы симметризации для N_Tx = 5.
static bool build_symmetry_matrix_5tx(float K[SMT_MAX_TX][SMT_MAX_TX], uint8_t condition) {
	switch (condition) {
	case 0x1F: // все 5 работают
	case 0xFF: { // все 5 работают
		static const float M[SMT_MAX_TX][SMT_MAX_TX] = {
			{ +0.75f, +0.50f, -0.25f, +0.00f, +0.00f },
			{ +0.25f, +0.50f, +0.25f, +0.00f, +0.00f },
			{ +0.00f, +0.25f, +0.50f, +0.25f, +0.00f },
			{ +0.00f, +0.00f, +0.25f, +0.50f, +0.25f },
			{ +0.00f, +0.00f, -0.25f, +0.50f, +0.75f } };
		copy_smt_matrix(K, M); return true; }
	case 0x1E: { // не работает T1
		static const float M[SMT_MAX_TX][SMT_MAX_TX] = {
			{ +0.00f, +1.25f, +0.50f, -0.75f, +0.00f },
			{ +0.00f, +0.75f, +0.50f, -0.25f, +0.00f },
			{ +0.00f, +0.25f, +0.50f, +0.25f, +0.00f },
			{ +0.00f, +0.00f, +0.25f, +0.50f, +0.25f },
			{ +0.00f, +0.00f, -0.25f, +0.50f, +0.75f } };
		copy_smt_matrix(K, M); return true; }
	case 0x1D: { // не работает T2
		static const float M[SMT_MAX_TX][SMT_MAX_TX] = {
			{ +1.25f, +0.00f, -0.75f, +0.50f, +0.00f },
			{ +0.75f, +0.00f, -0.25f, +0.50f, +0.00f },
			{ +0.00f, +0.00f, +0.75f, +0.50f, -0.25f },
			{ +0.00f, +0.00f, +0.25f, +0.50f, +0.25f },
			{ +0.00f, +0.00f, -0.25f, +0.50f, +0.75f } };
		copy_smt_matrix(K, M); return true; }
	case 0x1B: { // не работает T3
		static const float M[SMT_MAX_TX][SMT_MAX_TX] = {
			{ +0.50f, +0.75f, +0.00f, -0.25f, +0.00f },
			{ +0.00f, +1.25f, +0.00f, -0.75f, +0.50f },
			{ +0.00f, +0.75f, +0.00f, -0.25f, +0.50f },
			{ +0.00f, +0.25f, +0.00f, +0.25f, +0.05f },
			{ +0.00f, -0.25f, +0.00f, +0.75f, +0.05f } };
		copy_smt_matrix(K, M); return true; }
	case 0x17: { // не работает T4
		static const float M[SMT_MAX_TX][SMT_MAX_TX] = {
			{ +0.75f, +0.50f, -0.25f, +0.00f, +0.00f },
			{ +0.25f, +0.50f, +0.25f, +0.00f, +0.00f },
			{ -0.25f, +0.50f, +0.75f, +0.00f, +0.00f },
			{ +0.00f, +0.50f, -0.25f, +0.00f, +0.75f },
			{ +0.00f, +0.50f, -0.75f, +0.00f, +1.25f } };
		copy_smt_matrix(K, M); return true; }
	case 0x0F: { // не работает T5
		static const float M[SMT_MAX_TX][SMT_MAX_TX] = {
			{ +0.75f, +0.50f, -0.25f, +0.00f, +0.00f },
			{ +0.25f, +0.50f, +0.25f, +0.00f, +0.00f },
			{ +0.05f, +0.25f, +0.50f, +0.25f, +0.00f },
			{ +0.00f, -0.25f, +0.50f, +0.75f, +0.00f },
			{ +0.00f, -0.75f, +0.50f, +1.25f, +0.00f } };
		copy_smt_matrix(K, M); return true; }
	case 0x00: { // несимметризованные значения для всех 5 передатчиков
		zero_smt_matrix(K);
		for (int t = 0; t < 5; t++) K[t][t] = 1.0f;
		return true; }
	default:
		return false;
	}
}

// Матрицы симметризации для N_Tx = 4 (T5 не существует).
static bool build_symmetry_matrix_4tx(float K[SMT_MAX_TX][SMT_MAX_TX], uint8_t condition) {
	switch (condition) {
	case 0x0F: // все 4 работают
	case 0xFF: { // все работают
		static const float M[SMT_MAX_TX][SMT_MAX_TX] = {
			{ +0.75f, +0.50f, -0.25f, +0.00f, +0.00f },
			{ +0.25f, +0.50f, +0.25f, +0.00f, +0.00f },
			{ +0.00f, +0.25f, +0.50f, +0.25f, +0.00f },
			{ +0.00f, -0.25f, +0.50f, +0.75f, +0.00f },
			{ +0.00f, +0.00f, +0.00f, +0.00f, +0.00f } };
		copy_smt_matrix(K, M); return true; }
	case 0x0E: { // не работает T1
		static const float M[SMT_MAX_TX][SMT_MAX_TX] = {
			{ +0.00f, +1.25f, +0.50f, -0.75f, +0.00f },
			{ +0.00f, +0.75f, +0.50f, -0.25f, +0.00f },
			{ +0.00f, +0.25f, +0.50f, +0.25f, +0.00f },
			{ +0.00f, -0.25f, +0.50f, +0.75f, +0.00f },
			{ +0.00f, +0.00f, +0.00f, +0.00f, +0.00f } };
		copy_smt_matrix(K, M); return true; }
	case 0x0D: { // не работает T2
		static const float M[SMT_MAX_TX][SMT_MAX_TX] = {
			{ +1.25f, +0.00f, -0.75f, +0.50f, +0.00f },
			{ +0.75f, +0.00f, -0.25f, +0.50f, +0.00f },
			{ +0.25f, +0.00f, +0.25f, +0.50f, +0.00f },
			{ -0.25f, +0.00f, +0.75f, +0.50f, +0.00f },
			{ +0.00f, +0.00f, +0.00f, +0.00f, +0.00f } };
		copy_smt_matrix(K, M); return true; }
	case 0x0B: { // не работает T3
		static const float M[SMT_MAX_TX][SMT_MAX_TX] = {
			{ +0.50f, +0.75f, +0.00f, -0.25f, +0.00f },
			{ +0.50f, +0.25f, +0.00f, +0.25f, +0.00f },
			{ +0.50f, -0.25f, +0.00f, +0.75f, +0.00f },
			{ -0.50f, -0.75f, +0.00f, +1.25f, +0.00f },
			{ +0.00f, +0.00f, +0.00f, +0.00f, +0.00f } };
		copy_smt_matrix(K, M); return true; }
	case 0x07: { // не работает T4
		static const float M[SMT_MAX_TX][SMT_MAX_TX] = {
			{ +0.75f, +0.50f, -0.25f, +0.00f, +0.00f },
			{ +0.25f, +0.50f, +0.25f, +0.00f, +0.00f },
			{ -0.25f, +0.50f, +0.75f, +0.00f, +0.00f },
			{ -0.75f, +0.50f, +1.25f, +0.00f, +0.00f },
			{ +0.00f, +0.00f, +0.00f, +0.00f, +0.00f } };
		copy_smt_matrix(K, M); return true; }
	case 0x00: { // несимметризованные значения (только T1..T4)
		zero_smt_matrix(K);
		for (int t = 0; t < 4; t++) K[t][t] = 1.0f;
		return true; }
	default:
		return false;
	}
}

// Матрицы симметризации для N_Tx = 3 (T4, T5 не существуют).
static bool build_symmetry_matrix_3tx(float K[SMT_MAX_TX][SMT_MAX_TX], uint8_t condition) {
	switch (condition) {
	case 0x07: { // все 3 работают
		static const float M[SMT_MAX_TX][SMT_MAX_TX] = {
			{ +0.75f, +0.50f, -0.25f, +0.00f, +0.00f },
			{ +0.25f, +0.50f, +0.25f, +0.00f, +0.00f },
			{ -0.25f, +0.50f, +0.75f, +0.00f, +0.00f },
			{ -0.75f, +0.50f, +1.25f, +0.00f, +0.00f },
			{ +0.00f, +0.00f, +0.00f, +0.00f, +0.00f } };
		copy_smt_matrix(K, M); return true; }
	case 0x00: { // несимметризованные значения (только T1..T3)
		zero_smt_matrix(K);
		for (int t = 0; t < 3; t++) K[t][t] = 1.0f;
		return true; }
	default:
		return false;
	}
}

// Диспетчер: выбирает матрицу по (N_Tx, condition). Для нераспознанных
// комбинаций возвращает identity по маске condition.
static void build_symmetry_matrix(float K[SMT_MAX_TX][SMT_MAX_TX], uint32_t N_Tx, uint8_t condition) {
	bool resolved = false;
	if (N_Tx == 5)
		resolved = build_symmetry_matrix_5tx(K, condition);
	else if (N_Tx == 4)
		resolved = build_symmetry_matrix_4tx(K, condition);
	else if (N_Tx == 3)
		resolved = build_symmetry_matrix_3tx(K, condition);
	if (!resolved)
		fill_identity_symmetry_matrix(K, condition, (int)N_Tx);
}

V10 get_phase_smt(GP_TOOL_DATA *P, GP_TOOL_METROLOGY *metrology) {
	V10 phase_smt = { 0.0f, };
	if (P == nullptr || metrology == nullptr)
		return phase_smt;

	TOOL_IDENTITY id = decode_tool_signature(P->signature);
	const uint32_t N_Tx = id.n_tx;

	for (uint32_t freq = 0; freq < 2; freq++) {
		const uint8_t condition = extract_condition_byte(P->condition, (int)freq);
		float K[SMT_MAX_TX][SMT_MAX_TX];
		build_symmetry_matrix(K, N_Tx, condition);

		// Подготовка входных разностей фаз: нормализация к воздуху по Air_zz и
		// применение знака ориентации (Tx + Rx_Position). Для Rx_Position 0 знаки
		// чередуются как +,-,+,-,+ — совместимо с матрицами симметризации.
		float deltas[SMT_MAX_TX] = { 0.0f, };
		for (uint32_t Tx = 0; Tx < N_Tx && Tx < SMT_MAX_TX; Tx++) {
			deltas[Tx] = apply_rx_phase_orientation(
				P->DELTA_PH[freq][Tx],
				metrology->Air_zz[freq][Tx] / mG,
				Tx,
				metrology->Rx_Position);
		}

		// phase_smt[freq*N_Tx + t] = sum_j K[t][j] * deltas[j].
		for (uint32_t t = 0; t < N_Tx && t < SMT_MAX_TX; t++) {
			float sum = 0.0f;
			for (uint32_t j = 0; j < SMT_MAX_TX; j++)
				sum += K[t][j] * deltas[j];
			uint32_t idx = freq * N_Tx + t;
			if (idx < 10)
				phase_smt.VALUE[idx] = sum;
		}
	}
	return phase_smt;
}

V10 get_air_smt(V10 *S, GP_TOOL_METROLOGY *metrology) {
	V10 phase_smt = { 0.0f, };
	if (S == nullptr || metrology == nullptr)
		return phase_smt;

	TOOL_IDENTITY id = decode_tool_signature(metrology->signature);
	const uint32_t N_Tx = id.n_tx;
	// Для воздушной поправки считаем все физические передатчики работающими.
	const uint8_t condition = (uint8_t)((1u << N_Tx) - 1u);

	float K[SMT_MAX_TX][SMT_MAX_TX];
	build_symmetry_matrix(K, N_Tx, condition);

	// Вход S и выход phase_smt раскладываются freq-major со страйдом N_Tx.
	for (uint32_t freq = 0; freq < 2; freq++) {
		for (uint32_t t = 0; t < N_Tx && t < SMT_MAX_TX; t++) {
			float sum = 0.0f;
			for (uint32_t j = 0; j < SMT_MAX_TX; j++) {
				uint32_t in_idx = freq * N_Tx + j;
				if (j < N_Tx && in_idx < 10)
					sum += K[t][j] * S->VALUE[in_idx];
			}
			uint32_t idx = freq * N_Tx + t;
			if (idx < 10)
				phase_smt.VALUE[idx] = sum;
		}
	}
	return phase_smt;
}

complex <double> Z(double R, double L, double C, double f) {
	double Rd = 0.0007;
	double omega = 2 * PI*f;
	double K0 = PI * omega*mu0;
	double R_Add_Wire = 0.3;
	complex<double>ZFull = (R + R_Add_Wire) + 1i*((omega*L) - 1 / (omega*C));
	return (ZFull);
}

double L(double R_loop, double R_wire) {
	return  mu0 * R_loop * (log(8 * R_loop / R_wire) - 1.75);
}

double L(double L) {
	return  L;
}

complex <double> Loop(double R,  double C, double L, double r, double f, double L1, double L2) {
	double omega = 2 * PI*f;
	double K0 = PI * omega*mu0;
	complex<double>ZFull = (R) + 1i*((omega*L) - 1 / (omega*C));
	double Q = pow((pow((L2 + L1) / 2, 2) + r * r) * (pow((L2 - L1) / 2, 2) + r * r), 1.5);
	complex<double> VzzR1R2 = ((2.0 * 1i*ZFull * Q + K0 * pow(r, 4) * pow(L1, 3))*pow(L2, 3)) / ((2.0 * 1i*ZFull * Q + K0 * pow(r, 4) * pow(L2, 3))*pow(L1, 3));
	return (VzzR1R2);
}

complex <double> Loop_Re(double R,  double Re, double r, double f, double L1, double L2) {
	double omega = 2 * PI*f;
	double K0 = PI * omega*mu0;
	complex<double>ZFull = (R) + 1i*(Re);
	double Q = pow((pow((L2 + L1) / 2, 2) + r * r) * (pow((L2 - L1) / 2, 2) + r * r), 1.5);
	complex<double> VzzR1R2 = ((2.0 * 1i*ZFull * Q + K0 * pow(r, 4) * pow(L1, 3))*pow(L2, 3)) / ((2.0 * 1i*ZFull * Q + K0 * pow(r, 4) * pow(L2, 3))*pow(L1, 3));
	return (VzzR1R2);
}

double Re(double f, double r, double L, double L1, double L2) {
	double left = 0.1, right = 50, EPS = 0.01;
	double m1, m2;
	while (right - left > EPS) {
		m1 = left + (right - left) / 3;
		m2 = right - (right - left) / 3;
		if (abs(Loop_Re(0, m1, r, f, L1, L2)) < abs(Loop_Re(0, m2, r, f, L1, L2)))
			left = m1;
		else
			right = m2;
	}
	return (left + right) / 2;
}

double C_max_fi(double f, double r, double L, double L1, double L2) {
	double left = 500.0 / 1e12, right = 70000.0 / 1e12, EPS = 1.0 / 1e12;
	double m1, m2;
	while (right - left > EPS) {
		m1 = left + (right - left) / 3;
		m2 = right - (right - left) / 3;
		if (arg(Loop(1.0, m1, L, r, f, L1, L2)) < arg(Loop(1.0, m2, L, r, f, L1, L2)))
			left = m1;
		else
			right = m2;
	}
	return (left + right) / 2;
}

double F(double f,  double r, double L, double L1, double L2) {
	double left = 50000, right = 5000000, EPS = 1;
	double m1, m2;
	while (right - left > EPS) {
		m1 = left + (right - left) / 3;
		m2 = right - (right - left) / 3;

		if (abs(Z(1.0, L, C_max_fi(f, r, L, L1, L2), m1)) < abs(Z(1.0, L, C_max_fi(f,  r, L, L1, L2), m2)))
			right = m2;
		else
			left = m1;
	}
	return (left + right) / 2;
}

double C_f(double L, double f) {
	double R = 0;
	return  L / (pow(2 * PI * f * L, 2) + R * R);
}

uint16_t CRC16(const BYTE *nData, uint16_t wLength){
	static const uint16_t wCRCTable[] = {
	0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241,
	0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440,
	0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40,
	0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
	0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40,
	0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41,
	0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641,
	0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040,
	0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240,
	0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441,
	0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
	0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840,
	0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
	0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40,
	0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640,
	0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041,
	0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240,
	0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441,
	0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41,
	0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
	0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41,
	0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
	0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640,
	0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041,
	0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241,
	0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440,
	0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40,
	0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841,
	0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40,
	0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41,
	0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
	0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040 };

	BYTE nTemp;
	uint16_t wCRCWord = 0xFFFF;

	while (wLength--)
	{
		nTemp = *nData++ ^ wCRCWord;
		wCRCWord >>= 8;
		wCRCWord ^= wCRCTable[nTemp];
	}
	return wCRCWord;

}

int send_to_tool(System::IO::Ports::SerialPort^ COM, uint8_t CMD, uint8_t* DATA, uint8_t length) {
	bool stop = false;
	uint8_t *packet = new uint8_t[length + 3];
	cli::array<byte>^ packet_to_send = gcnew cli::array<byte>(length + 3);
	packet[0] = CMD;
	packet_to_send[0] = CMD;
	for (int i = 1; i < length + 1; i++) {
		packet[i] = DATA[i - 1];
		packet_to_send[i] = DATA[i - 1];
	}
	uint16_t CRC_res = CRC16(packet, length + 1);
	packet_to_send[length + 1] = CRC_res;
	packet_to_send[length + 2] = CRC_res >> 8;

	try {
		COM->Write(packet_to_send, 0, length + 3);
	}
	catch (...) { stop = 1; }
	if (!stop) {
		delete packet;
		delete packet_to_send;
		return 0;
	}
	else return 1;
}

int receive_from_tool(System::IO::Ports::SerialPort^ COM, uint8_t& CMD, uint8_t* DATA, uint16_t length) {
	int  stop = 0;
	uint8_t *packet = new uint8_t[length + 3];
	cli::array<byte>^ packet_to_receive = gcnew cli::array<byte>(length + 3);

	for (int i = 0; i < (length + 3); i++) {
		packet[i] = 0xff;
		packet_to_receive[i] = 0xff;
	}

	std::cout << "[receive_from_tool] BytesToRead=" << COM->BytesToRead
			  << " need=" << (length + 3)
			  << " ReadTimeout=" << COM->ReadTimeout << std::endl;

	// Накопление пакета до полного размера или истечения таймаута.
	const int total = length + 3;
	int bytes_read = 0;
	DateTime deadline = DateTime::Now.AddMilliseconds(COM->ReadTimeout);
	try {
		while (bytes_read < total) {
			int n = COM->Read(packet_to_receive, bytes_read, total - bytes_read);
			if (n <= 0) break;
			bytes_read += n;
			if (DateTime::Now > deadline) break;
		}
	}
	catch (...) {
		if (bytes_read == 0) stop = 1;
	}

	if (!stop && bytes_read < total) {
		std::cout << "timeout (short read: " << bytes_read << "/" << total << ")" << std::endl;
		COM->DiscardInBuffer();
		delete packet;
		delete packet_to_receive;
		return 1;
	}

	if (!stop) {
		for (int i = 0; i < length + 3; i++)
			packet[i] = packet_to_receive[i];

		uint16_t calc_crc = CRC16(packet, length + 1);
		uint16_t recv_crc = *(uint16_t *) &(packet[length + 1]);
		if (calc_crc == recv_crc) {
			CMD = packet[0];
			for (int i = 0; i < length; i++)
				DATA[i] = packet_to_receive[i + 1];
			delete packet;
			delete packet_to_receive;
			return 0;
		}
		else {
			std::stringstream hex_ss;
			hex_ss << "crc no valid | CalcCRC=0x" << std::hex << calc_crc
				   << " RecvCRC=0x" << recv_crc << std::dec
				   << " | Packet(" << (length + 3) << "b): ";
			for (int i = 0; i < length + 3; i++) {
				hex_ss << std::hex << std::setw(2) << std::setfill('0')
					   << (int)packet_to_receive[i] << " ";
			}
			std::cout << hex_ss.str() << std::dec << std::endl;
			delete packet;
			delete packet_to_receive;
			return 2;
		}
	}

	else {
		stop = 0;
		std::cout << "timeout (exception on Read)" << std::endl;
		delete packet;
		delete packet_to_receive;
		return 1;
	}

}

GP_TOOL_DATA READ_BLOK_GP_TOOL_DATA(ifstream &fin, int n) {
	GP_TOOL_DATA P;
	fin.seekg(n * sizeof(GP_TOOL_DATA));
	fin.read((char*)&P, sizeof(GP_TOOL_DATA));
	return P;
}

ARGB rgb(double ratio)
{
	ARGB argb = { 0, };
	int normalized = int(128 + ratio * 256 * 5.5);

	unsigned char x = normalized % 256;

	unsigned char red = 0, grn = 0, blu = 0;
	switch (normalized / 256)
	{
	case 1: red = 255;      grn = x;        blu = 0;       break;
	case 2: red = 255 - x;  grn = 255;      blu = 0;       break;
	case 3: red = 0;        grn = 255;      blu = x;       break;
	case 4: red = 0;        grn = 255 - x;  blu = 255;     break;
	case 5: red = x;        grn = 0;        blu = 255;     break;
	case 0: red = 255;      grn = 0;        blu = 255 - x; break;
	}
	argb.A = 255; argb.B = blu; argb.G = grn; argb.R = red;
	return argb;
}
