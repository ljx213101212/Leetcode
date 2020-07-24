#include <windows.h>
#include <wtsapi32.h>
#include <userenv.h>
#include <tchar.h>
#include <stdio.h>

#pragma comment (lib, "user32.lib")
#pragma comment (lib, "wtsapi32.lib")
#pragma comment (lib, "userenv.lib")
#pragma comment (lib, "advapi32.lib")


//https://stackoverflow.com/questions/50644181/createprocessasuser-process-exits-with-1073741502/50743993#50743993
DWORD report_error(const char* operation)
{
    DWORD err = GetLastError();
    return err;
}

// Launch notepad as currently logged-on user
DWORD LaunchProcess(DWORD SessionId, const char** failed_operation)
{
    HANDLE hToken;
    BOOL ok = WTSQueryUserToken(SessionId, &hToken);
    if (!ok)
        return report_error(*failed_operation = "WTSQueryUserToken");

    void* environment = NULL;
    ok = CreateEnvironmentBlock(&environment, hToken, TRUE);

    if (!ok)
    {
        CloseHandle(hToken);
        return report_error(*failed_operation = "CreateEnvironmentBlock");
    }

    TOKEN_LINKED_TOKEN lto;
    DWORD nbytes;
    ok = GetTokenInformation(hToken, TokenLinkedToken, &lto, sizeof(lto), &nbytes);

    if (ok)
    {
        CloseHandle(hToken);
        hToken = lto.LinkedToken;
    }

    STARTUPINFOW si = { sizeof(si) };
    PROCESS_INFORMATION pi = { };
    //https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createprocessasuserw#remarks
    si.lpDesktop = (LPWSTR)(L"winsta0\\default");

    // Do NOT want to inherit handles here, surely
    DWORD dwCreationFlags = NORMAL_PRIORITY_CLASS | /* CREATE_NEW_CONSOLE | */ CREATE_UNICODE_ENVIRONMENT;
    ok = CreateProcessAsUserW(hToken, L"c:\\windows\\system32\\notepad.exe", NULL, NULL, NULL, FALSE,
        dwCreationFlags, environment, NULL, &si, &pi);

    DestroyEnvironmentBlock(environment);
    CloseHandle(hToken);

    if (!ok)
        return report_error(*failed_operation = "CreateProcessAsUser");

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return 0;
}

// Determine the session ID of the currently logged-on user
DWORD GetCurrentSessionId()
{
    WTS_SESSION_INFO* pSessionInfo;
    DWORD n_sessions = 0;
    BOOL ok = WTSEnumerateSessions(WTS_CURRENT_SERVER, 0, 1, &pSessionInfo, &n_sessions);
    if (!ok)
        return 0;

    DWORD SessionId = 0;

    for (DWORD i = 0; i < n_sessions; ++i)
    {
        if (pSessionInfo[i].State == WTSActive)
        {
            SessionId = pSessionInfo[i].SessionId;
            break;
        }
    }

    WTSFreeMemory(pSessionInfo);
    return SessionId;
}


#define SERVICE_NAME __T ("demo_service")

bool quit;

// CtrlHandler callback
DWORD WINAPI CtrlHandler(DWORD dwControl, DWORD  dwEventType, LPVOID lpEventData, LPVOID lpContext)
{
    if (dwControl == SERVICE_CONTROL_STOP)
        quit = true;
    return NO_ERROR;
}

// SvcMain callback
VOID WINAPI SvcMain(DWORD dwArgc, LPTSTR* lpszArgv)
{
    // Register for callbacks
    SERVICE_STATUS_HANDLE sh = RegisterServiceCtrlHandlerEx(SERVICE_NAME, CtrlHandler, NULL);

    // Tell the SCM that we are up and running
    SERVICE_STATUS ss = { };
    ss.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    ss.dwCurrentState = SERVICE_RUNNING;
    ss.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    SetServiceStatus(sh, &ss);

    TCHAR buf[256];
    const TCHAR* title = __T("(c) 2020 Never Give Up");

    while (!quit)
    {
        DWORD response = IDOK;

        DWORD SessionId = GetCurrentSessionId();
        if (SessionId == 0)
        {
            Sleep(2000);
            continue;
        }

        // Pop-up a message on the screen of the currently logged-on user (session 1)
        swprintf_s(buf, __T("Ready to launch..., SessionId = %d"), SessionId);
        WTSSendMessageW(WTS_CURRENT_SERVER_HANDLE, SessionId, (TCHAR*)title, _tcslen(title),
            buf, _tcslen(buf), MB_OKCANCEL, 0, &response, TRUE);
        if (response == IDCANCEL)
            break;

        const char* failed_operation = "";
        DWORD dwResult = LaunchProcess(SessionId, &failed_operation);

        // Report results
        swprintf_s(buf, __T("LaunchProcess returned %lx from %s"), dwResult, failed_operation);
        WTSSendMessageW(WTS_CURRENT_SERVER_HANDLE, SessionId, (TCHAR*)title, _tcslen(title),
            buf, _tcslen(buf), MB_OK, 0, &response, TRUE);

       /* FILE* logfile = fopen("g:\\temp\\service.log", "at");
        if (logfile)
        {
            fprintf(logfile, "%s\n", buf);
            fclose(logfile);
        }*/
    }

    // Tell the SCM we are going away and exit
    ss.dwCurrentState = SERVICE_STOPPED;
    SetServiceStatus(sh, &ss);
}


// main
int main(void)
{
    SERVICE_TABLE_ENTRY DispatchTable[] =
    {
        { (LPWSTR)SERVICE_NAME, SvcMain },
        { NULL, NULL }
    };

    // This call returns when the service has stopped. 
    // The process should simply terminate when the call returns.
    StartServiceCtrlDispatcherW(DispatchTable);
    return 0;
}