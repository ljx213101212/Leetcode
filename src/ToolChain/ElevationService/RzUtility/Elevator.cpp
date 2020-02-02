// Elevator.cpp : Implementation of CElevator

#include "stdafx.h"
#include "Elevator.h"

#include <shlobj.h>
#include <shlwapi.h>

// CElevator

STDMETHODIMP CElevator::LaunchProcess(BSTR appName, BSTR args)
{
    OutputDebugString(_T("Elevator::LaunchProcess"));
    OutputDebugString(appName);

    SHELLEXECUTEINFO ShExecInfo = {};

    ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    ShExecInfo.fMask = NULL;
    ShExecInfo.hwnd = NULL;
    ShExecInfo.lpVerb = NULL;
    ShExecInfo.lpFile = appName;
    ShExecInfo.lpParameters = args;
    ShExecInfo.lpDirectory = NULL;
    ShExecInfo.nShow = SW_SHOWNORMAL;
    ShExecInfo.hInstApp = NULL;

    if (!ShellExecuteEx(&ShExecInfo)) {
        OutputDebugString(_T("Failed to launch process"));
    }
    return S_OK;
}
