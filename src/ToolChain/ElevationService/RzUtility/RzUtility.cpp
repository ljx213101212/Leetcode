// RzUtility.cpp : Implementation of WinMain


#include "stdafx.h"
#include "resource.h"
#include "RzUtility_i.h"


using namespace ATL;

#include <stdio.h>

class CRzUtilityModule : public ATL::CAtlServiceModuleT< CRzUtilityModule, IDS_SERVICENAME >
{
public :
	DECLARE_LIBID(LIBID_RzUtilityLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_RZUTILITY, "{9C877A5A-7FE0-4E2B-9735-60420DE27084}")
		HRESULT InitializeSecurity() throw()
	{
		// TODO : Call CoInitializeSecurity and provide the appropriate security settings for your service
		// Suggested - PKT Level Authentication, 
		// Impersonation Level of RPC_C_IMP_LEVEL_IDENTIFY 
		// and an appropriate Non NULL Security Descriptor.

		return S_OK;
	}
	// Called when the service is started
	void ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);
};

CRzUtilityModule _AtlModule;



//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
								LPTSTR /*lpCmdLine*/, int nShowCmd)
{
	return _AtlModule.WinMain(nShowCmd);
}


void CRzUtilityModule::ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv)
{
	// TODO: Add your specialized code here and/or call the base class
	CAtlServiceModuleT<CRzUtilityModule,100>::ServiceMain(dwArgc, lpszArgv);
}
