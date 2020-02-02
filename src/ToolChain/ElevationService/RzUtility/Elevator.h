// Elevator.h : Declaration of the CElevator

#pragma once
#include "resource.h"       // main symbols



#include "RzUtility_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;


// CElevator

class ATL_NO_VTABLE CElevator :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CElevator, &CLSID_Elevator>,
	public IDispatchImpl<IElevator, &IID_IElevator, &LIBID_RzUtilityLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CElevator()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ELEVATOR)


BEGIN_COM_MAP(CElevator)
	COM_INTERFACE_ENTRY(IElevator)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:


    STDMETHOD(LaunchProcess)(BSTR appName, BSTR args);
};

OBJECT_ENTRY_AUTO(__uuidof(Elevator), CElevator)
