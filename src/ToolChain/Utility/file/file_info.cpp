#include "file_info.h"

namespace my_util
{

//detour {
#ifdef _WIN32



typedef HANDLE (WINAPI * CREATEFILEW)(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);

static CREATEFILEW fpCreateFileW = NULL;

static HANDLE WINAPI DetourCreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile) {

  HANDLE hPipe = fpCreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);

  if ((lpFileName[0] != L'C') && (lpFileName[0] != L'c'))
  cout << "MILLION DetourCreateFileW Process " << std::dec << ::GetCurrentProcessId() << " Thread " << std::dec << ::GetCurrentThreadId() << " Handle " << std::hex << hPipe \
             << " name " << lpFileName << " dwDesiredAccess " << std::hex << dwDesiredAccess << " dwShareMode " << std::hex << dwShareMode \
             << " lpSecurityAttributes " << std::hex << lpSecurityAttributes << " dwCreationDisposition " << std::hex << dwCreationDisposition << " dwFlagsAndAttributes " \
             << std::hex << dwFlagsAndAttributes << " hTemplateFile " << std::hex << hTemplateFile;

  return hPipe;
}

typedef BOOL (WINAPI * READFILE)(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);

static READFILE fpReadFile = NULL;

static BOOL WINAPI DetourReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped) {
  BOOL ret = fpReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);

  //print named pipe only
  if (GetFileType(hFile) == FILE_TYPE_PIPE) {
    if (GetNamedPipeInfo(hFile, nullptr, nullptr, nullptr, nullptr)) {
      DWORD BytesRead = 0;
      if (lpNumberOfBytesRead != nullptr) {
        BytesRead =  *lpNumberOfBytesRead;
      }

      cout << "MILLION DetourReadFile Process " << std::dec << ::GetCurrentProcessId() << " Thread " << std::dec << ::GetCurrentThreadId() << " Handle " << std::hex << hFile \
                 << " nNumberOfBytesToRead " << std::dec << nNumberOfBytesToRead << " lpNumberOfBytesRead " << std::dec << BytesRead << " lpOverlapped " << std::hex << lpOverlapped;

     }
  }

  return ret;
}

typedef BOOL (WINAPI * WRITEFILE)(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);

static WRITEFILE fpWriteFile = NULL;

static BOOL WINAPI DetourWriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped) {
  BOOL ret = fpWriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);

  //print named pipe only
  if (GetFileType(hFile) == FILE_TYPE_PIPE) {
    if (GetNamedPipeInfo(hFile, nullptr, nullptr, nullptr, nullptr)) {
      if (nNumberOfBytesToWrite == 36) {
        nNumberOfBytesToWrite = 36;
      }

      DWORD BytesWrite = 0;
      if (lpNumberOfBytesWritten != nullptr) {
        BytesWrite = *lpNumberOfBytesWritten;
      }

      cout << "MILLION DetourWriteFile Process " << std::dec << ::GetCurrentProcessId() << " Thread " << std::dec << ::GetCurrentThreadId() << " Handle " << std::hex << hFile \
                 << " nNumberOfBytesToWrite " << std::dec << nNumberOfBytesToWrite << " lpNumberOfBytesWritten  " << std::dec << BytesWrite << " lpOverlapped " << std::hex << lpOverlapped;
    }
  }

  return ret;
}

typedef BOOL (WINAPI * CLOSEHANDLE)(HANDLE hObject);

static CLOSEHANDLE fpCloseHandle = NULL;

static BOOL WINAPI DetourCloseHandle(HANDLE hObject) {
  BOOL ret = fpCloseHandle(hObject);

  cout << "MILLION DetourCloseHandle Process " << std::dec << ::GetCurrentProcessId() << " Thread " << std::dec << ::GetCurrentThreadId() << " Handle " << std::hex << hObject;

  return ret;
}

#endif
//detour }



static void initMinhook()
{
#ifdef _WIN32
    {
        HMODULE detourHandle = ::LoadLibraryW(L"MinHook.x64.dll");

        if (detourHandle != nullptr)
        {
            void *addressMHInit = (void *)GetProcAddress(detourHandle, "MH_Initialize");

            if (addressMHInit != nullptr)
            {
                typedef int(WINAPI * MH_Initialize)();
                MH_Initialize MHInitialize = (MH_Initialize)addressMHInit;
                int ret = MHInitialize();

                //Initialize successfully
                if (ret == 0)
                {
                    //Create Hook
                    typedef int(WINAPI * MH_CreateHook)(LPVOID, LPVOID, LPVOID *);
                    MH_CreateHook MHCreateHook = (MH_CreateHook) nullptr;

                    void *addressMHCreateHook = (void *)GetProcAddress(detourHandle, "MH_CreateHook");
                    if (addressMHCreateHook != nullptr)
                    {
                        MHCreateHook = (MH_CreateHook)addressMHCreateHook;

                        ret = MHCreateHook((LPVOID)&CreateFileW, (LPVOID)&DetourCreateFileW, reinterpret_cast<LPVOID *>(&fpCreateFileW));
                        ret = MHCreateHook((LPVOID)&ReadFile, (LPVOID)&DetourReadFile, reinterpret_cast<LPVOID *>(&fpReadFile));
                        ret = MHCreateHook((LPVOID)&WriteFile, (LPVOID)&DetourWriteFile, reinterpret_cast<LPVOID *>(&fpWriteFile));
                        ret = MHCreateHook((LPVOID)&CloseHandle, (LPVOID)&DetourCloseHandle, reinterpret_cast<LPVOID *>(&fpCloseHandle));
                    }

                    //Enable Hook
                    void *addressMHEnableHook = (void *)GetProcAddress(detourHandle, "MH_EnableHook");
                    if (addressMHEnableHook != nullptr)
                    {
                        typedef int(WINAPI * MH_EnableHook)(LPVOID);
                        MH_EnableHook MHEnableHook = (MH_EnableHook)addressMHEnableHook;

                        ret = MHEnableHook((LPVOID)&CreateFileW);
                        ret = MHEnableHook((LPVOID)&ReadFile);
                        ret = MHEnableHook((LPVOID)&WriteFile);
                        ret = MHEnableHook((LPVOID)&DuplicateHandle);
                        ret = MHEnableHook((LPVOID)&CloseHandle);
                    }
                }
            }
        }
        //  MessageBoxW(NULL, L"Not hooked...", L"MinHook Sample", MB_OK);
    }
#endif
}

    bool FileInfo::createTempFileAndReadWithFlagDeleteOnClose()
        {
            //Flags::FLAG_CREATE_ALWAYS | Flags::FLAG_READ |
            //Flags::FLAG_WRITE |
            //Flags::FLAG_SHARE_DELETE
            //Flags::FLAG_TEMPORARY | Flags::FLAG_HIDDEN| Flags::FLAG_DELETE_ON_CLOSE
            //FILE_ATTRIBUTE_TEMPORARY | FILE_FLAG_DELETE_ON_CLOSE

            initMinhook();
            HANDLE WINHandle = CreateFileW((LPCWSTR)TEXT("D:\\SourceCodeTest\\Leetcode\\org.png"),  (GENERIC_WRITE | GENERIC_READ), (FILE_SHARE_READ | FILE_SHARE_WRITE |
                    FILE_SHARE_DELETE), 0, CREATE_ALWAYS,  FILE_ATTRIBUTE_TEMPORARY | FILE_FLAG_DELETE_ON_CLOSE, 0);
                    bool isOK = true;
                    return isOK;

            CloseHandle(WINHandle);
        }

} // namespace my_util


 