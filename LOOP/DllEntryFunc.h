#pragma once
#include "windows.h"

//---------------------------------------------------------------------------
extern "C" __declspec(dllexport) void* __cdecl AMK_StartComPort(void **ComPort, HWND callbackHandle);
extern "C" __declspec(dllexport)  unsigned  AMK_GetCurrState(void *ComPort);
extern "C" __declspec(dllexport)  unsigned  AMK_Init(void *ComPort);
extern "C" __declspec(dllexport)  unsigned  AMK_BeginCyclogram(void *ComPort);
extern "C" __declspec(dllexport)  unsigned  AMK_GetData(void *ComPort, void *data);
extern "C" __declspec(dllexport)  unsigned  AMK_SetIdle(void *ComPort);
extern "C" __declspec(dllexport)  unsigned  AMK_PwrOn(void *ComPort);
extern "C" __declspec(dllexport)  unsigned  AMK_PwrOff(void *ComPort);
extern "C" __declspec(dllexport)  void __cdecl  AMK_DeleteComPort(void *ComPort);
//---------------------------------------------------------------------------

