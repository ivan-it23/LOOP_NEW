#include <fstream>
#include <iostream>
#include "function.h"
#include <vector>
using namespace System;

//для новой программы
//извлекаем из файла метрологии длинны и частоты зондов
void metro_convert(void *metrology_struct, vector <SONDE_PARAM> &sonde_param) {
	uint32_t sign = *(uint32_t*)metrology_struct ;
	uint32_t buff = *(uint32_t*)metrology_struct << 12;
	cout << " sign " << sign << endl ;
	buff = buff >> 12;
	uint32_t tool_type = buff / 100000;
	uint32_t tool_N_Tx = (buff % 100000) / 10000;
	uint32_t tool_mod = (buff % 10000) / 1000;
	uint32_t tool_number = (buff % 1000);
	uint32_t signature_for_pallete = buff / 1000;

	
	cout << "tool_N_Tx " << tool_N_Tx << " tool_mod " << tool_mod ;

	sonde_param.clear();

	if (tool_N_Tx == 5 && tool_mod == 1) {
		METROLOGY_NEW metrology = *(METROLOGY_NEW*)metrology_struct;
		for (int freq = 0; freq < 2; freq++) {
			for (int i = 0; i < 5; i++) {
				SONDE_PARAM buff;
				buff.L1 = (metrology.L1[i] / 1000.0);
				buff.L2 = (metrology.L2[i] / 1000.0);
				buff.F = (metrology.F[freq] * 1000.0);
				sonde_param.push_back(buff);
			}
		}						
	}

	if (tool_N_Tx == 4 && tool_mod == 1) {
		METROLOGY metrology = *(METROLOGY*)metrology_struct;
		for (int i = 0; i < 8; i++) {
			SONDE_PARAM buff;
			buff.L1 = (metrology.L1[i] / 1000.0);
			buff.L2 = (metrology.L2[i] / 1000.0);
			buff.F = (metrology.F[i] * 1000.0);
			sonde_param.push_back(buff);
		}
	}
	
	cout << " sonde_param.size " << sonde_param.size() << endl;

}

//фаза от кольца
float dfi_from_loop(SONDE_PARAM *param, LOOP_PARAM *loop_param) {
	complex<float>ZFull;
	if(param->F < 1000000)
		ZFull = (loop_param->R + 0.3f) + 1if*loop_param->Re;
	if (param->F > 1000000)
		ZFull = (loop_param->R + 0.5f) + 1if*loop_param->Re;
	//ZFull = (loop_param->R + loop_param->R_Add_Wire) + 1if*loop_param->Re;
	cout << "param->F " << param->F << endl;
	float piomegamu0 = (0.00002480502 * param->F);
	float Q = pow((pow((param->L2 + param->L1) / 2, 2) + pow(loop_param->r, 2)) * (pow((param->L2 - param->L1) / 2, 2) + pow(loop_param->r, 2)), 1.5);
	return arg(((2.0f * 1if*ZFull * Q + (float)(piomegamu0 * pow(loop_param->r, 4) * pow(param->L1, 3)))*(float)pow(param->L2, 3)) /
		       ((2.0f * 1if*ZFull * Q + (float)(piomegamu0 * pow(loop_param->r, 4) * pow(param->L2, 3)))*(float)pow(param->L1, 3)));

	//float omega = (2.0 * PI * param->F);
	//float Q = pow((pow((param->L2 + param->L1) / 2, 2) + loop_param->r * loop_param->r) * (pow((param->L2 - param->L1) / 2, 2) + loop_param->r * loop_param->r), 1.5);
	//return arg(((2.0f * 1if*ZFull * Q + (float)PI * omega * (float)mu0 * pow(loop_param->r, 4) * pow(param->L1, 3))*pow(param->L2, 3)) /
	//	((2.0f * 1if*ZFull * Q + (float)PI * omega * (float)mu0 * pow(loop_param->r, 4) * pow(param->L2, 3))*pow(param->L1, 3)));
	
}
// фаза от среды 
float dfi_ro(SONDE_PARAM *param, float ro) {
	float omegamu0sigma = (0.0000078957f * param->F) / ro;
	complex <float> ik = 1if * sqrt(1if * omegamu0sigma);
	complex <float> SGN = exp(ik*(param->L2 - param->L1)) * ((1.0f - ik * param->L2) / (1.0f - ik * param->L1));
	return arg(SGN);
}
// УЭС от фазы (обратная задача )
float  ro_dfi(SONDE_PARAM *param, float PH_from_loop) {
	float Ro0;
	float epsilon_ARG = 0.0000005; // точность  фазы
	float epsilon_INF = 0.000000005; // точность  фазы
	float  Ro_0 = 0.01, Ro_max = 7000.0;  //мин. и мах. значение ус для расчета
	bool stop_epsl_ARG = false;
	bool stop_epsl_INF = false;
	float  ro_0 = Ro_0;
	float  ro_max = Ro_max;

	do {
		float X1 = ro_0 + 0.382*(ro_max - ro_0);
		float X2 = ro_max - 0.382*(ro_max - ro_0);
		float A = PH_from_loop - dfi_ro(param, X1);//
		float B = PH_from_loop - dfi_ro(param, X2);//
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
//вычисляем кажущееся УЭС от сигналов прибора
V10 ro_dfi(vector <SONDE_PARAM> &sonde_param, V10 PH_from_loop) {
	V10 ro_data;
	for (int i = 0; i < 10; i++)
		ro_data.VALUE[i] = 0;
	for (int i = 0; i < sonde_param.size(); i++)
		ro_data.VALUE[i] = ro_dfi(&sonde_param[i], PH_from_loop.VALUE[i]);
	return ro_data;
}

//комплект фаз 10 зондов от всех сопротивлений
 void calc_PH_from_loop(vector <SONDE_PARAM> &sonde_param, vector <LOOP_PARAM> &loop_param, vector <V10> &PH_computed) {
	 PH_computed.clear();
	 // с первого по последнее сопротивления  
	 V10 buff = { 0.0f, };
	 PH_computed.push_back(buff);
	 //for (int R_idx = 0; R_idx < loop_param.size(); R_idx++) {
	 for (int R_idx = 0; R_idx < 8; R_idx++) {
	
		 LOOP_PARAM loop_param_local;
		 loop_param_local.R = loop_param[R_idx].R;	
		 loop_param_local.r = loop_param[R_idx].r;
		 loop_param_local.R_Add_Wire = loop_param[R_idx].R_Add_Wire;
		// для всех зондов
		 for (int i = 0; i < sonde_param.size(); i++) {
			 loop_param_local.Re = loop_param[i].Re;
			 buff.VALUE[i] = dfi_from_loop(&sonde_param[i], &loop_param_local);
		 } 
		 //длина вектора равна количеству сопротивлений
		  PH_computed.push_back(buff);
	 }
 }

//комплект кажущихся УЭС бесконечной среды 10 зондов от вычисленных фаз от кольца для всех сопротивлений кольца
 void calc_Ro_from_loop(vector <SONDE_PARAM> &sonde_param, vector <V10> &PH_computed, vector <V10> &Ro_computed) {
	 Ro_computed.clear();
	 // с первого по последнее сопротивления  
	 for (int R_idx = 0; R_idx < PH_computed.size(); R_idx++) {
		 V10 buff;
		 // для всех зондов
		 for (int i = 0; i < sonde_param.size(); i++)
			 buff.VALUE[i] = ro_dfi(&sonde_param[i], PH_computed[R_idx].VALUE[i]);
		 //длина вектора равна количеству сопротивлений
		 Ro_computed.push_back(buff);
	 }
 }

V10 get_ph_from_SDR(System::IO::Ports::SerialPort^ COM, int mode, int &res) {
    V10 ph_data;
	for (int i = 0; i < 10; i++) 
	    ph_data.VALUE[i] = 0;
		
	DATA_5_TX *All_Data_SDR = new DATA_5_TX;
	//const uint16_t length = sizeof(DATA_5_TX) + 2;
	const uint16_t length = 242;

	uint8_t CMD;
	uint8_t DATA_IN[length] = { 0, };
	int result;
	bool stop = false;

	try {
		COM->Close();
		COM->ReadTimeout = 100;
		COM->WriteTimeout = 100;
		COM->BaudRate = 115200;
		COM->Open();
	}
	catch (...) { stop = 1; }

	if (!stop) {
		uint8_t DATA[2] = { 0x05 ,0x00 };
		if (send_to_tool(COM, CMD_GET_DATA, DATA, 2) == 1) {
			Console::WriteLine("Соединение с прибором прервано переподключите !");
		}
		Sleep(50);
		result = receive_from_tool(COM, CMD, DATA_IN, length);
		COM->Close();
	}

	else {
		stop = 0;
		result = 3;
		Console::WriteLine("no port 112");
		COM->Close();
	}
	//cout << "result " << result << endl;
	res = result;
	if (result == 0) {
		All_Data_SDR = (DATA_5_TX*)(DATA_IN + 2);
		for (int i = 0; i < 5; i++) {
			ph_data.VALUE[i] = All_Data_SDR->DELTA_PH_400[i];
			ph_data.VALUE[i + 5] = All_Data_SDR->DELTA_PH_2000[i];
		}
	}
	// меняем знак для зеркальных передатчиков
		//Z1 повернут T1 
	for (int i = 0; i < 10; i++) {
		if (i & 1)ph_data.VALUE[i];//1,3 норм нечет
		else ph_data.VALUE[i] = -ph_data.VALUE[i];//2,4 инвертируем чет	
	}

	/*
		 if (result == 0) {
			 DATA_5_TX *P = new DATA_5_TX;
			 P = All_Data_SDR;
			 cout << " CMD " << std::hex << CMD << endl;
			 cout << " SGN " << std::dec << P->signature << endl;
			  //std::bitset<16> y(c);
			 cout << " condition " << P->condition << endl;
			 cout << " frame " << P->frame << endl;
			 
			 cout << "TEMP " << P->temperature << endl;
			 cout << "Ro_400 " << P->Ro_smt_400[0] << " " << P->Ro_smt_400[1] << " " << P->Ro_smt_400[2] << " " << P->Ro_smt_400[3] << " " << P->Ro_smt_400[4] << endl;
			 cout << "Ro_2000 " << P->Ro_smt_2000[0] << " " << P->Ro_smt_2000[1] << " " << P->Ro_smt_2000[2] << " " << P->Ro_smt_2000[3] << " " << P->Ro_smt_2000[4] << endl;

			 for (int i = 0; i < 5; i++) {
				 cout << "AM_TX_400 " << i << " RX_1 " << P->AM_RX_1_400[i] << " RX_2 " << P->AM_RX_2_400[i] << endl;
			 }
			 for (int i = 0; i < 5; i++) {
				 cout << "AM_TX_2000 " << i << " RX_1 " << P->AM_RX_1_2000[i] << " RX_2 " << P->AM_RX_2_2000[i] << endl;
			 }

			 for (int i = 0; i < 5; i++) {
				 cout << "D_PH_TX_400 " << i << " RX_1 " << round(P->DELTA_PH_400[i] * mG) << endl;
			 }
			 for (int i = 0; i < 5; i++) {
				 cout << "D_PH_TX_2000 " << i << " RX_1 " << round(P->DELTA_PH_2000[i] * mG) << endl;
			 }

		 }
	*/	

	return ph_data;
}

V10 get_ph_from_1DDS(System::IO::Ports::SerialPort^ COM, int mode, int &res) {
	V10 ph_data;
	for (int i = 0; i < 10; i++)
		ph_data.VALUE[i] = 0;

	AllData_1DDS *All_Data_1DDS = new AllData_1DDS;
	const uint16_t length = 252;
	uint8_t CMD;
	uint8_t DATA_IN[length] = { 0, };
	int result;
	bool stop = false;

	try {
		COM->Close();
		COM->ReadTimeout = 200;
		COM->WriteTimeout = 200;
		COM->BaudRate = 125000;
		COM->Open();
	}
	catch (...) { stop = 1; }


	if (!stop) {
		uint8_t *DATA = new uint8_t;
		*DATA = length;
		if (send_to_tool(COM, CMD_WORK, DATA, 1) == 1) {
			Console::WriteLine("Соединение с прибором прервано переподключите !");
		}
		Sleep(50);
		result = receive_from_tool(COM, CMD, DATA_IN, length);
		COM->Close();
	}

	else {
		stop = 0;
		result = 3;
		Console::WriteLine("no port 112");
		COM->Close();
	}
	//cout << "result " << result << endl;
	res = result;
	if (result == 0) {
		All_Data_1DDS = (AllData_1DDS*)(DATA_IN + 12);
	    for (int i = 0; i < 8; i++) {
		    ph_data.VALUE[i] = All_Data_1DDS->PH_RX_2[i] - All_Data_1DDS->PH_RX_1[i];
		    while (ph_data.VALUE[i] > PI) ph_data.VALUE[i] -= 2 * PI;
		    while (ph_data.VALUE[i] <= -PI) ph_data.VALUE[i] += 2 * PI;
	}
	// меняем знак для зеркальных передатчиков
	    //Z1 повернут T1 
		for (int i = 0; i < 8; i++) {
			if (i & 1)ph_data.VALUE[i] ;//1,3 норм нечет
			else ph_data.VALUE[i] = -ph_data.VALUE[i];//2,4 инвертируем чет	
		}	
	}

	if (result == 0) {
		cout << " SGN " << std::hex << All_Data_1DDS->signature << endl;
		cout << " condition " << std::dec << All_Data_1DDS->condition << endl;
		cout << " frame " << All_Data_1DDS->frame << endl;	
		cout << "TEMP " << All_Data_1DDS->temperature << endl;
		cout << "Ro_400 " << All_Data_1DDS->Ro_smt[0] << " " << All_Data_1DDS->Ro_smt[1]  << " " << All_Data_1DDS->Ro_smt[2] << " " << All_Data_1DDS->Ro_smt[3] << endl;
		cout << "Ro_2000 " << All_Data_1DDS->Ro_smt[4] << " " << All_Data_1DDS->Ro_smt[5] << " " << All_Data_1DDS->Ro_smt[6] << " " << All_Data_1DDS->Ro_smt[7] << endl;	
	
	}

	return ph_data;
}

V10 get_ph_from_AUTONOM(void *ComPort, int mode, int &res) {
	V10 ph_data;
	for (int i = 0; i < 10; i++)
		ph_data.VALUE[i] = 0;

	AllData_1DDS *All_Data_1DDS = new AllData_1DDS;
	
	extern GetData AMK_GetData;
	extern HINSTANCE AMK_DLL;
	AMK_GetData = (GetData)GetProcAddress(AMK_DLL, "AMK_GetData");
	_controlfp(_CW_DEFAULT, 0xfffff);
	//ComPort = AMK_StartComPort(&ComPort, NULL);
	int result = AMK_GetData(ComPort, All_Data_1DDS) ;

	if (result == 1) {
		res = 0;
		for (int i = 0; i < 8; i++) {
			ph_data.VALUE[i] = All_Data_1DDS->PH_RX_2[i] - All_Data_1DDS->PH_RX_1[i];
			while (ph_data.VALUE[i] > PI) ph_data.VALUE[i] -= 2 * PI;
			while (ph_data.VALUE[i] <= -PI) ph_data.VALUE[i] += 2 * PI;
		}
		// меняем знак для зеркальных передатчиков
			//Z1 повернут T1 
		for (int i = 0; i < 8; i++) {
			if (i & 1)ph_data.VALUE[i];//1,3 норм нечет
			else ph_data.VALUE[i] = -ph_data.VALUE[i];//2,4 инвертируем чет	
		}
	    /*
		cout << " SGN " << std::hex << All_Data_1DDS->signature << endl;
		cout << " condition " << std::dec << All_Data_1DDS->condition << endl;
		cout << " frame " << All_Data_1DDS->frame << endl;
		cout << "TEMP " << All_Data_1DDS->temperature << endl;
		cout << "Ro_400 " << All_Data_1DDS->Ro_smt[0] << " " << All_Data_1DDS->Ro_smt[1] << " " << All_Data_1DDS->Ro_smt[2] << " " << All_Data_1DDS->Ro_smt[3] << endl;
		cout << "Ro_2000 " << All_Data_1DDS->Ro_smt[4] << " " << All_Data_1DDS->Ro_smt[5] << " " << All_Data_1DDS->Ro_smt[6] << " " << All_Data_1DDS->Ro_smt[47] << endl;
	    */
	}

	else res = 1;

	return ph_data;
}

//старое
//вычисляет симметризованную разницу фаз от кольца  для всех передатчиков
V8 calc_phase_from_loop(METROLOGY *metrology_struct, double R, double R_Add_Wire, double *Re, double L, double C, double r, bool mode) {
	V8 phase_loop, phase_loop_smt;
	double L1[8], L2[8], F[8], Air[8];

	for (int i = 0; i < 8; i++) {
		L1[i] = (metrology_struct->L1[i] / 1000.0) + 0.0000001;
		L2[i] = (metrology_struct->L2[i] / 1000.0) + 0.0000001;
		F[i] = (metrology_struct->F[i] * 1000.0) + 0.0000001;					
	}
	//for (int i = 0; i < 8; i++) {
	//	cout << L1[i] << " " << L2[i] << " " << F[i] << endl;
	//}

	double omega[8];
	complex<double>ZFull[8];
	double Q[8];
	for (int i = 0; i < 8; i++) {
		omega[i] = (2.0 * PI * F[i]);	
		if (mode == 0)
		    ZFull[i] = (R + R_Add_Wire) + 1i*(omega[i]*L - 1 / (omega[i]*C));
		if (mode == 1)
			ZFull[i] = (R + R_Add_Wire) + 1i*Re[i];
		//cout << "ZFull " << ZFull[i] << endl;
		// разницы фаз от кольца
		Q[i] = pow((pow((L2[i] + L1[i]) / 2, 2) + r * r) * (pow((L2[i] - L1[i]) / 2, 2) + r * r), 1.5);
		phase_loop.VALUE[i] = arg(((2.0 * 1i*ZFull[i] * Q[i] + PI*omega[i]*mu0 * pow(r, 4) * pow(L1[i], 3))*pow(L2[i], 3)) / ((2.0 * 1i*ZFull[i] * Q[i] + PI*omega[i]*mu0 * pow(r, 4) * pow(L2[i], 3))*pow(L1[i], 3)));
	}
	
	return phase_loop;
	//return phase_loop_smt;
}

double SIGNAL( METROLOGY *metrology, uint16_t n, double ro) {
	double L1 = metrology->L1[n] / 1000.0;
	double L2 = metrology->L2[n] / 1000.0;
	double omegamu0sigma = (0.0078957 * metrology->F[n]) / ro;
	complex <double> ik = 1i * sqrt(1i * omegamu0sigma);
	complex <double> SGN = exp(ik*(L2 - L1)) * ((1.0 - ik * L2) / (1.0 - ik * L1));
	return arg(SGN);
}

int  RO(METROLOGY *metrology, uint16_t n, V8 *Signal_from_loop, V8 *Ro_calculated_adecvat_r_loop) {
	float Ro0;
	float epsilon_ARG = 0.0000005; // точность  фазы
	float epsilon_INF = 0.000000005; // точность  фазы
	float  Ro_0 = 0.01, Ro_max = 7000.0;  //мин. и мах. значение ус для расчета
	bool stop_epsl_ARG = false;
	bool stop_epsl_INF = false;
	float  ro_0 = Ro_0;
	float  ro_max = Ro_max;

	do {
		float X1 = ro_0 + 0.382*(ro_max - ro_0);
		float X2 = ro_max - 0.382*(ro_max - ro_0);
		float A = Signal_from_loop->VALUE[n] - SIGNAL(metrology, n, X1);//
		float B = Signal_from_loop->VALUE[n] - SIGNAL(metrology, n, X2);//
		if (fabs(A) > fabs(B)) { ro_0 = X1; }
		else { ro_max = X2; }
		if (fabs(A) < epsilon_ARG || fabs(B) < epsilon_ARG) { stop_epsl_ARG = true; }
		else if (fabs(A - B) < epsilon_INF) { stop_epsl_INF = true; }

	} while (stop_epsl_ARG == false && stop_epsl_INF == false);
	if (stop_epsl_ARG == true) Ro0 = (ro_0 + ro_max) / 2;
	if (stop_epsl_INF == true) Ro0 = 7200;//-100
	ro_0 = Ro_0;
	ro_max = Ro_max;
	Ro_calculated_adecvat_r_loop->VALUE[n] = Ro0;
	return  0;
}

V8  R0_adecvat_r_loop(METROLOGY *metrology, V8 *Signal_from_loop) {
	V8 Ro_calculated_adecvat_r_loop = {0.0,};
	for (int i = 0; i < 8; i++) {
		RO(metrology, i, Signal_from_loop, &Ro_calculated_adecvat_r_loop);
	}
	return   Ro_calculated_adecvat_r_loop;
}

V8 get_phase_raw(AllData_1DDS *P) {
	V8 phase = { 0.0, };
	for (int i = 0; i < 8; i++) {
		phase.VALUE[i] = P->PH_RX_2[i] - P->PH_RX_1[i];
		while (phase.VALUE[i] > PI) phase.VALUE[i] -= 2 * PI;
		while (phase.VALUE[i] <= -PI) phase.VALUE[i] += 2 * PI;
	}
	return phase;
}

V8 get_phase(AllData_1DDS *P, METROLOGY *metrology) {
	V8 phase = { 0.0, };
	//получаем разности фаз
	for (int i = 0; i < 8; i++) {
		phase.VALUE[i] = P->PH_RX_2[i] - P->PH_RX_1[i];
		while (phase.VALUE[i] > PI) phase.VALUE[i] -= 2 * PI;
		while (phase.VALUE[i] <= -PI) phase.VALUE[i] += 2 * PI;
	}

	// нормализуем разности фаз к воздуху  и меняем знак для зеркальных передатчиков
	//Z1 повернут T1 
	
	for (int i = 0; i < 8; i++) {
		if (i & 1)phase.VALUE[i] -= (metrology->Air_zz[i] / mG);//1,3 норм нечет
		else phase.VALUE[i] = -phase.VALUE[i] + (metrology->Air_zz[i] / mG);//2,4 инвертируем чет	
	}
	
	return phase;
}

V8 get_phase_smt(AllData_1DDS *P, METROLOGY *metrology) {
	V8 phase_smt;
	V8 phase = { 0.0, };
	double K[4][4] = { 0.0, };//
	enum T_SMT {
		T1, T2, T3, T4,
	};
	//получаем разности фаз
	for (int i = 0; i < 8; i++) {
		phase.VALUE[i] = P->PH_RX_2[i] - P->PH_RX_1[i];
		while (phase.VALUE[i] > PI) phase.VALUE[i] -= 2 * PI;
		while (phase.VALUE[i] <= -PI) phase.VALUE[i] += 2 * PI;
	}

	// нормализуем разности фаз к воздуху  и меняем знак для зеркальных передатчиков
	//Z1 повернут T1 
	for (int i = 0; i < 8; i++) {
		if (i & 1)phase.VALUE[i]-= (metrology->Air_zz[i] / mG);//1,3 норм нечет
		else phase.VALUE[i] = - phase.VALUE[i] + (metrology->Air_zz[i] / mG);//2,4 инвертируем	чет
	}

	//задаем коэфф симметризации для различных версий зондов
	if ((metrology->serial & (1 << 17)) == 0) {//3 transmitters LWD direct messurement
		K[T1][T1] = 0.75; K[T1][T2] = 0.5; K[T1][T3] = -0.25; K[T1][T4] = 0.0;
		K[T2][T1] = 0.25; K[T2][T2] = 0.5; K[T2][T3] = 0.25; K[T2][T4] = 0.0;
		K[T3][T1] = -0.25; K[T3][T2] = 0.5; K[T3][T3] = 0.75; K[T3][T4] = 0.0;
		K[T4][T1] = 0.0;  K[T4][T2] = 0.0; K[T4][T3] = 0.0;  K[T4][T4] = 0.0;
	}

	if ((metrology->serial & (1 << 17)) != 0) {//4 transmitters autonomy and LWD
		K[T1][T1] = 0.75;  K[T1][T2] = 0.50; K[T1][T3] = -0.25; K[T1][T4] = 0.0;
		K[T2][T1] = 0.25;  K[T2][T2] = 0.50; K[T2][T3] = 0.25; K[T2][T4] = 0.0;
		K[T3][T1] = 0.00;  K[T3][T2] = 0.25; K[T3][T3] = 0.50; K[T3][T4] = 0.25;
		K[T4][T1] = 0.00;  K[T4][T2] = -0.25; K[T4][T3] = 0.50; K[T4][T4] = 0.75;
	}

	for (int i = 0; i < 4; i++) {
		phase_smt.VALUE[i] = K[i][0] * phase.VALUE[0] + K[i][1] * phase.VALUE[1] + K[i][2] * phase.VALUE[2] + K[i][3] * phase.VALUE[3];
		phase_smt.VALUE[i + 4] = K[i][0] * phase.VALUE[4] + K[i][1] * phase.VALUE[5] + K[i][2] * phase.VALUE[6] + K[i][3] * phase.VALUE[7];
	}
	return phase_smt;

}

V8 get_air_smt(V8 *S, METROLOGY *metrology) {
	V8 phase_smt;	
	double K[4][4] = { 0.0, };//
	enum T_SMT {
		T1, T2, T3, T4,
	};
	
	//задаем коэфф симметризации для различных версий зондов
	if ((metrology->serial & (1 << 17)) == 0) {//3 transmitters LWD direct messurement
		K[T1][T1] = 0.75; K[T1][T2] = 0.5; K[T1][T3] = -0.25; K[T1][T4] = 0.0;
		K[T2][T1] = 0.25; K[T2][T2] = 0.5; K[T2][T3] = 0.25; K[T2][T4] = 0.0;
		K[T3][T1] = -0.25; K[T3][T2] = 0.5; K[T3][T3] = 0.75; K[T3][T4] = 0.0;
		K[T4][T1] = 0.0;  K[T4][T2] = 0.0; K[T4][T3] = 0.0;  K[T4][T4] = 0.0;
	}

	if ((metrology->serial & (1 << 17)) != 0) {//4 transmitters autonomy and LWD
		K[T1][T1] = 0.75;  K[T1][T2] = 0.50; K[T1][T3] = -0.25; K[T1][T4] = 0.0;
		K[T2][T1] = 0.25;  K[T2][T2] = 0.50; K[T2][T3] = 0.25; K[T2][T4] = 0.0;
		K[T3][T1] = 0.00;  K[T3][T2] = 0.25; K[T3][T3] = 0.50; K[T3][T4] = 0.25;
		K[T4][T1] = 0.00;  K[T4][T2] = -0.25; K[T4][T3] = 0.50; K[T4][T4] = 0.75;
	}

	for (int i = 0; i < 4; i++) {
		phase_smt.VALUE[i] = K[i][0] * S->VALUE[0] + K[i][1] * S->VALUE[1] + K[i][2] * S->VALUE[2] + K[i][3] * S->VALUE[3];
		phase_smt.VALUE[i + 4] = K[i][0] * S->VALUE[4] + K[i][1] * S->VALUE[5] + K[i][2] * S->VALUE[6] + K[i][3] * S->VALUE[7];
	}
	return phase_smt;

}

/////////////////////////////////////////////////////////////////////////////

complex <double> Z(double R, double L, double C, double f) {
	double Rd = 0.0007;//0.7mm
	double omega = 2 * PI*f;
	double K0 = PI * omega*mu0;
	double R_Add_Wire = 0.3;
	complex<double>ZFull = (R + R_Add_Wire) + 1i*((omega*L) - 1 / (omega*C));
	return (ZFull);
}

double L(double R_loop, double R_wire) {
	return  mu0 * R_loop * (log(8 * R_loop / R_wire) - 1.75);//индуктивность кольца 
}

double L(double L) {
	return  L;
}

complex <double> Loop(double R,  double C, double L, double r, double f, double L1, double L2) {
	double omega = 2 * PI*f;
	double K0 = PI * omega*mu0;
	//Импеданс кольца со вставкой
	complex<double>ZFull = (R) + 1i*((omega*L) - 1 / (omega*C));
	double Q = pow((pow((L2 + L1) / 2, 2) + r * r) * (pow((L2 - L1) / 2, 2) + r * r), 1.5);
	complex<double> VzzR1R2 = ((2.0 * 1i*ZFull * Q + K0 * pow(r, 4) * pow(L1, 3))*pow(L2, 3)) / ((2.0 * 1i*ZFull * Q + K0 * pow(r, 4) * pow(L2, 3))*pow(L1, 3));
	return (VzzR1R2);
}

complex <double> Loop_Re(double R,  double Re, double r, double f, double L1, double L2) {
	double omega = 2 * PI*f;
	double K0 = PI * omega*mu0;
	//Импеданс кольца со вставкой
	complex<double>ZFull = (R) + 1i*(Re);
	double Q = pow((pow((L2 + L1) / 2, 2) + r * r) * (pow((L2 - L1) / 2, 2) + r * r), 1.5);
	complex<double> VzzR1R2 = ((2.0 * 1i*ZFull * Q + K0 * pow(r, 4) * pow(L1, 3))*pow(L2, 3)) / ((2.0 * 1i*ZFull * Q + K0 * pow(r, 4) * pow(L2, 3))*pow(L1, 3));
	return (VzzR1R2);
}

double Re(double f, double r, double L, double L1, double L2) {
	double left = 0.1, right = 50, EPS = 0.01; // входные данные
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
	double left = 500.0 / 1e12, right = 70000.0 / 1e12, EPS = 1.0 / 1e12; // входные данные
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
	double left = 50000, right = 5000000, EPS = 1; // входные данные
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
	//return 1.0 /  pow(2 * PI*f, 2);
}
/////////////////////////////////////////////////////////////////////////////////////////
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
	/////////////////////////////////////
	//char *myDataPtr = (char*)&(wCRCWord);
	//char tmp;
	//tmp = myDataPtr[0];
	//myDataPtr[0] = myDataPtr[1];
	//myDataPtr[1] = tmp;
	//////////////////////////////////////
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


	COM->DiscardInBuffer();
	COM->DiscardOutBuffer();

	try {
		COM->Write(packet_to_send, 0, length + 3);
	}
	catch (...) { stop = 1; }
	if (!stop) {
		/*
		Console::Write("send_to_tool ");
		for (int i = 0; i < length + 3; i++) {
			Console::Write("{0:x}", packet_to_send[i]); Console::Write(" ");
		}
		Console::Write(" CRC ");
		Console::Write("{0:x}", packet_to_send[length + 2]);
		Console::Write("{0:x}", packet_to_send[length + 1]);
		Console::WriteLine("");
		*/
		//Sleep(100);
		delete packet;
		delete packet_to_send;
		return 0;
	}
	else return 1;
}

int receive_from_tool(System::IO::Ports::SerialPort^ COM, uint8_t CMD, uint8_t* DATA, uint16_t length) {
	int  stop = 0;
	uint8_t *packet = new uint8_t[length + 3];
	cli::array<byte>^ packet_to_receive = gcnew cli::array<byte>(length + 3);

	for (int i = 0; i < (length + 3); i++) {
		packet[i] = 0xff;
		packet_to_receive[i] = 0xff;
	}

	//while (serialPort_LWD_1DDS->BytesToRead < length + 3) { ; }

	try {
		COM->Read(packet_to_receive, 0, length + 3);
	}
	catch (...) { stop = 1; }

	if (!stop) {
		for (int i = 0; i < length + 3; i++)
			packet[i] = packet_to_receive[i];

		COM->DiscardInBuffer();
		COM->DiscardOutBuffer();

		if (CRC16(packet, length + 1) == *(uint16_t *) &(packet[length + 1])) {
			CMD = packet[0];
			for (int i = 0; i < length; i++)
				DATA[i] = packet_to_receive[i + 1];
			/*
			Console::Write("receive_from_tool_ok ");
			for (int i = 0; i < length; i++) {
				Console::Write("{0:x}", DATA[i]); Console::Write(" ");
			}
			Console::WriteLine("");
			*/
			delete packet;
			delete packet_to_receive;
			return 0;
		}
		else {
			/*
			Console::Write("error ");
			for (int i = 0; i < length+3; i++) {
				Console::Write("{0:x}", packet_to_receive[i]); Console::Write(" ");
			}
			Console::WriteLine("");
			*/
			cout << "crc no valid" << endl;
			COM->DiscardInBuffer();
			COM->DiscardOutBuffer();
			delete packet;
			delete packet_to_receive;
			return 2;
		}
	}

	else {
		stop = 0;
		cout << "timeout" << endl;
		COM->DiscardInBuffer();
		COM->DiscardOutBuffer();
		delete packet;
		delete packet_to_receive;
		return 1;
	}

}

AllData_1DDS READ_BLOK_AllData_1DDS(ifstream &fin, int n) {
	AllData_1DDS P;
	fin.seekg(n * sizeof(AllData_1DDS));
	fin.read((char*)&P, sizeof(AllData_1DDS));
	return P;
}

ARGB rgb(double ratio)
{
	ARGB argb = { 0, };
	//we want to normalize ratio so that it fits in to 6 regions
	//where each region is 256 units long
	int normalized = int(128 + ratio * 256 * 5.5);

	//find the distance to the start of the closest region
	unsigned char x = normalized % 256;

	unsigned char red = 0, grn = 0, blu = 0;
	switch (normalized / 256)
	{
	case 1: red = 255;      grn = x;        blu = 0;       break;//red
	case 2: red = 255 - x;  grn = 255;      blu = 0;       break;//yellow
	case 3: red = 0;        grn = 255;      blu = x;       break;//green
	case 4: red = 0;        grn = 255 - x;  blu = 255;     break;//cyan
	case 5: red = x;        grn = 0;        blu = 255;     break;//blue
	case 0: red = 255;      grn = 0;        blu = 255 - x; break;//magenta
	}
	argb.A = 255; argb.B = blu; argb.G = grn; argb.R = red;
	return argb;
}