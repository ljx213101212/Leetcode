// TestApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "..\RzUtility_i.h"

int main()
{
    ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

    IElevator* elevator = NULL;
    HRESULT hr = CoCreateInstance(CLSID_Elevator, NULL, CLSCTX_LOCAL_SERVER, IID_IElevator, (void**)&elevator);
    if (SUCCEEDED(hr))
    {
        BSTR app = SysAllocString(_T("Notepad.exe"));
        BSTR args = SysAllocString(_T(""));

        elevator->LaunchProcess(app, args);
        elevator->Release();

        SysFreeString(app);
    }

    CoUninitialize();
    return 0;
}

