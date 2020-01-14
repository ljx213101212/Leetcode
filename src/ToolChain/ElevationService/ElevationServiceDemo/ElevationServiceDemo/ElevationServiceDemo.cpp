// ElevationServiceDemo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "windows.h"

#include <objbase.h>
#include <unknwn.h>
#include <wrl/client.h>
#include <iostream>
#include "elevation_service_idl.h"

void CallElevationService();

int main()
{
	CallElevationService();
    std::cout << "Hello World!\n";
	return 0;
}


void CallElevationService() {
	HRESULT hr = S_FALSE;
	CLSID chromeElevator_CLSID = { 0x708860E0, 0xF641, 0x4611,
		{ 0x88, 0x95, 0x7D, 0x86, 0x7D, 0xD3, 0x67, 0x5B }
	}; // 708860E0-F641-4611-8895-7D867DD3675B

/*
	IID IElevator_IID = { 0xA949CB4E, 0xC4F9, 0x44C4,
		{ 0xB2, 0x13, 0x6B, 0xF8, 0xAA, 0x9A, 0xC6, 0x9C }
	}; // A949CB4E-C4F9-44C4-B213-6BF8AA9AC69C

	IID IElevatorLib_IID = { 0x0014D784, 0x7012, 0x4A79,
		{ 0x8A, 0xB6, 0xAD, 0xDB, 0x81, 0x93, 0xA0, 0x6E }
	}; // 0014D784-7012-4A79-8AB6-ADDB8193A06E

	IID IElevatorRazer_IID = { 0x27D74468, 0xB929, 0x44D5,
		{ 0xA5, 0x48, 0x4D, 0xD1, 0xA8, 0x16, 0xCE, 0xF3 }
	}; // 27D74468-B929-44D5-A548-4DD1A816CEF3

	IID IElevatorChromium_IID = { 0xB88C45B9, 0x8825, 0x4629,
		{ 0xB8, 0x3E, 0x77, 0xCC, 0x67, 0xD9, 0xCE, 0xED }
	}; // B88C45B9-8825-4629-B83E-77CC67D9CEED

*/
	IID IElevatorChrome_IID = { 0x463ABECF, 0x410D, 0x407F,
		{ 0x8A, 0xF5, 0x0D, 0xF3, 0x5A, 0x00, 0x5C, 0xC8 }
	};	// 463ABECF-410D-407F-8AF5-0DF35A005CC8

	CoInitializeEx(nullptr, COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE);
	{
		Microsoft::WRL::ComPtr<IElevatorChrome> chromeElevator;
		hr = CoCreateInstance(
			chromeElevator_CLSID, nullptr, CLSCTX_LOCAL_SERVER,
			IElevatorChrome_IID, IID_PPV_ARGS_Helper(&chromeElevator));

		// At this point, the "elevation_service.exe" is launched, visible in "ProcessExplorer"

		if (S_OK == hr)
		{


			//ULONG_PTR proc_handle = 0;	// using this crashes
			ULONG_PTR* proc_handle_ptr = (ULONG_PTR*)malloc(sizeof(ULONG_PTR) * 2);

			if (NULL != proc_handle_ptr)
			{
				*proc_handle_ptr = 0;
				DWORD proc_id = GetCurrentProcessId();

				hr = chromeElevator->RunRecoveryCRXElevated(L"ChromeRecovery.crx3"/*TestFile("ChromeRecovery.crx3").value().c_str()*/,
					L"{c49ab053-2387-4809-b188-1902648802e1}", L"57.8.0.1",
					L"{c49ab053-2387-4809-b188-1902648802e1}", proc_id, proc_handle_ptr);

				std::cout << "hr: 0x" << std::hex << hr << std::endl;

				free(proc_handle_ptr);
				proc_handle_ptr = NULL;
			}
		}
	}
	CoUninitialize();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
