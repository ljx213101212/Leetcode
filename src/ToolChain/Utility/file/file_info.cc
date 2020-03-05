#include "file_info.h"

namespace my_util
{

// //detour {
// #ifdef _WIN32
// typedef int (WINAPI *MESSAGEBOXW)(HWND, LPCWSTR, LPCWSTR, UINT);

// // Pointer for calling original MessageBoxW
// static MESSAGEBOXW fpMessageBoxW = NULL;

// // Detour function which overrides MessageBoxW
// static int WINAPI DetourMessageBoxW(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType)
// {
//   return fpMessageBoxW(hWnd, L"Hooked!", lpCaption, uType);
// }

// typedef BOOL (WINAPI *SETWINDOWPOS)(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags);

// // Pointer for calling original SetWindowPos
// static SETWINDOWPOS fpSetWindowPos = NULL;

// // Detour function which overrides SetWindowPos
// static BOOL WINAPI DetourSetWindowPos(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags)
// {
// cout << "MILLION " << " hwnd " << hWnd << "  DetourSetWindowPos x " << X << " y " << Y << " width " << cx << " heigth " << cy;
//   return fpSetWindowPos(hWnd, hWndInsertAfter, X, Y, cx, cy, uFlags);
// }

// typedef HANDLE (WINAPI *CREATENAMEDPIPEW)(LPCWSTR lpName, DWORD dwOpenMode, DWORD dwPipeMode, DWORD nMaxInstances, DWORD nOutBufferSize, DWORD nInBufferSize, DWORD nDefaultTimeOut, LPSECURITY_ATTRIBUTES lpSecurityAttributes);

// static CREATENAMEDPIPEW fpCreateNamedPipeW = NULL;

// static HANDLE WINAPI DetourCreateNamedPipeW(LPCWSTR lpName, DWORD dwOpenMode, DWORD dwPipeMode, DWORD nMaxInstances, DWORD nOutBufferSize, DWORD nInBufferSize, DWORD nDefaultTimeOut,
//     LPSECURITY_ATTRIBUTES lpSecurityAttributes) {
//   HANDLE hPipe = fpCreateNamedPipeW(lpName, dwOpenMode, dwPipeMode, nMaxInstances, nOutBufferSize, nInBufferSize, nDefaultTimeOut, lpSecurityAttributes);
//   cout << "MILLION DetourCreateNamedPipeW Process " << std::dec << ::GetCurrentProcessId() << " Handle " << hPipe << " name " << lpName << " dwOpenMode " << std::hex << dwOpenMode \
//              << " dwPipeMode " << std::hex << dwPipeMode << " nMaxInstances " << nMaxInstances << " nOutBufferSize " << std::dec << nOutBufferSize << " nInBufferSize " << std::dec << nInBufferSize \
//              << " nDefaultTimeOut " << std::dec << nDefaultTimeOut << " lpSecurityAttributes " << std::hex << lpSecurityAttributes;

//   return hPipe;
// }

// typedef HANDLE (WINAPI * CREATEFILEW)(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);

// static CREATEFILEW fpCreateFileW = NULL;

// static HANDLE WINAPI DetourCreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes,
//     HANDLE hTemplateFile) {

//   HANDLE hPipe = fpCreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);

//   if ((lpFileName[0] != L'C') && (lpFileName[0] != L'c'))
//   cout << "MILLION DetourCreateFileW Process " << std::dec << ::GetCurrentProcessId() << " Thread " << std::dec << ::GetCurrentThreadId() << " Handle " << std::hex << hPipe \
//              << " name " << lpFileName << " dwDesiredAccess " << std::hex << dwDesiredAccess << " dwShareMode " << std::hex << dwShareMode \
//              << " lpSecurityAttributes " << std::hex << lpSecurityAttributes << " dwCreationDisposition " << std::hex << dwCreationDisposition << " dwFlagsAndAttributes " \
//              << std::hex << dwFlagsAndAttributes << " hTemplateFile " << std::hex << hTemplateFile;

//   return hPipe;
// }

// typedef BOOL (WINAPI * READFILE)(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);

// static READFILE fpReadFile = NULL;

// static BOOL WINAPI DetourReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped) {
//   BOOL ret = fpReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);

//   //print named pipe only
//   if (GetFileType(hFile) == FILE_TYPE_PIPE) {
//     if (GetNamedPipeInfo(hFile, nullptr, nullptr, nullptr, nullptr)) {
//       DWORD BytesRead = 0;
//       if (lpNumberOfBytesRead != nullptr) {
//         BytesRead =  *lpNumberOfBytesRead;
//       }

//       cout << "MILLION DetourReadFile Process " << std::dec << ::GetCurrentProcessId() << " Thread " << std::dec << ::GetCurrentThreadId() << " Handle " << std::hex << hFile \
//                  << " nNumberOfBytesToRead " << std::dec << nNumberOfBytesToRead << " lpNumberOfBytesRead " << std::dec << BytesRead << " lpOverlapped " << std::hex << lpOverlapped;

//      }
//   }

//   return ret;
// }

// typedef BOOL (WINAPI * WRITEFILE)(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);

// static WRITEFILE fpWriteFile = NULL;

// static BOOL WINAPI DetourWriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped) {
//   BOOL ret = fpWriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);

//   //print named pipe only
//   if (GetFileType(hFile) == FILE_TYPE_PIPE) {
//     if (GetNamedPipeInfo(hFile, nullptr, nullptr, nullptr, nullptr)) {
//       if (nNumberOfBytesToWrite == 36) {
//         nNumberOfBytesToWrite = 36;
//       }

//       DWORD BytesWrite = 0;
//       if (lpNumberOfBytesWritten != nullptr) {
//         BytesWrite = *lpNumberOfBytesWritten;
//       }

//       cout << "MILLION DetourWriteFile Process " << std::dec << ::GetCurrentProcessId() << " Thread " << std::dec << ::GetCurrentThreadId() << " Handle " << std::hex << hFile \
//                  << " nNumberOfBytesToWrite " << std::dec << nNumberOfBytesToWrite << " lpNumberOfBytesWritten  " << std::dec << BytesWrite << " lpOverlapped " << std::hex << lpOverlapped;
//     }
//   }

//   return ret;
// }


// typedef BOOL (WINAPI * DUPLOCATEHANDLE)(HANDLE hSourceProcessHandle, HANDLE hSourceHandle, HANDLE hTargetProcessHandle, LPHANDLE lpTargetHandle, DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwOptions);

// static DUPLOCATEHANDLE fpDuplicateHandle = NULL;

// static BOOL WINAPI DetourDuplicateHandle(HANDLE hSourceProcessHandle, HANDLE hSourceHandle, HANDLE hTargetProcessHandle, LPHANDLE lpTargetHandle, DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwOptions) {
//   bool isTransferHandle = dwOptions & 0x80000000;
//   bool isCloseHandleInProcess = dwOptions & 0x40000000;
//   dwOptions &= 0x0FFFFFFF;

//   BOOL ret = fpDuplicateHandle(hSourceProcessHandle, hSourceHandle, hTargetProcessHandle, lpTargetHandle, dwDesiredAccess, bInheritHandle, dwOptions);

//   {
//     HANDLE outHandle = nullptr;
//     if (lpTargetHandle != nullptr) {
//       outHandle = *lpTargetHandle;
//     }

//     if (isTransferHandle) {
//       cout << "MILLION DetourDuplicateHandle TransferHandle Process " << std::dec << ::GetCurrentProcessId() << " Thread " << std::dec << ::GetCurrentThreadId() \
//                  << " hSourceProcessHandle " << std::hex << hSourceProcessHandle << " Src Handle " << std::hex << hSourceHandle \
//                  << " hTargetProcessHandle " << std::hex << hTargetProcessHandle << " Target Handle " << std::hex << outHandle \
//                  << " dwDesiredAccess " << std::dec << dwDesiredAccess << " dwOptions " << std::hex << dwOptions;

//     } else if (isCloseHandleInProcess) {
//       cout << "MILLION DetourDuplicateHandle CloseHandleInProcess Process " << std::dec << ::GetCurrentProcessId() << " Thread " << std::dec << ::GetCurrentThreadId() \
//                  << " hSourceProcessHandle " << std::hex << hSourceProcessHandle << " Src Handle " << std::hex << hSourceHandle \
//                  << " hTargetProcessHandle " << std::hex << hTargetProcessHandle << " Target Handle " << std::hex << outHandle \
//                  << " dwDesiredAccess " << std::dec << dwDesiredAccess << " dwOptions " << std::hex << dwOptions;

//     }
//   }

//   return ret;
// }

// typedef BOOL (WINAPI * CLOSEHANDLE)(HANDLE hObject);

// static CLOSEHANDLE fpCloseHandle = NULL;

// static BOOL WINAPI DetourCloseHandle(HANDLE hObject) {
//   BOOL ret = fpCloseHandle(hObject);

//   cout << "MILLION DetourCloseHandle Process " << std::dec << ::GetCurrentProcessId() << " Thread " << std::dec << ::GetCurrentThreadId() << " Handle " << std::hex << hObject;

//   return ret;
// }

// #endif
// //detour }



// static void FileInfo::initMinhook()
// {
// #ifdef _WIN32
//     {
//         HMODULE detourHandle = ::LoadLibraryW(L"MinHook.x64.dll");

//         if (detourHandle != nullptr)
//         {
//             void *addressMHInit = (void *)GetProcAddress(detourHandle, "MH_Initialize");

//             if (addressMHInit != nullptr)
//             {
//                 typedef int(WINAPI * MH_Initialize)();
//                 MH_Initialize MHInitialize = (MH_Initialize)addressMHInit;
//                 int ret = MHInitialize();

//                 //Initialize successfully
//                 if (ret == 0)
//                 {
//                     //Create Hook
//                     typedef int(WINAPI * MH_CreateHook)(LPVOID, LPVOID, LPVOID *);
//                     MH_CreateHook MHCreateHook = (MH_CreateHook) nullptr;

//                     void *addressMHCreateHook = (void *)GetProcAddress(detourHandle, "MH_CreateHook");
//                     if (addressMHCreateHook != nullptr)
//                     {
//                         MHCreateHook = (MH_CreateHook)addressMHCreateHook;

//                         ret = MHCreateHook((LPVOID)&MessageBoxW, (LPVOID)&DetourMessageBoxW, reinterpret_cast<LPVOID *>(&fpMessageBoxW));
//                         ret = MHCreateHook((LPVOID)&SetWindowPos, (LPVOID)&DetourSetWindowPos, reinterpret_cast<LPVOID *>(&fpSetWindowPos));
//                         ret = MHCreateHook((LPVOID)&CreateNamedPipeW, (LPVOID)&DetourCreateNamedPipeW, reinterpret_cast<LPVOID *>(&fpCreateNamedPipeW));
//                         ret = MHCreateHook((LPVOID)&CreateFileW, (LPVOID)&DetourCreateFileW, reinterpret_cast<LPVOID *>(&fpCreateFileW));
//                         ret = MHCreateHook((LPVOID)&ReadFile, (LPVOID)&DetourReadFile, reinterpret_cast<LPVOID *>(&fpReadFile));
//                         ret = MHCreateHook((LPVOID)&WriteFile, (LPVOID)&DetourWriteFile, reinterpret_cast<LPVOID *>(&fpWriteFile));
//                         ret = MHCreateHook((LPVOID)&DuplicateHandle, (LPVOID)&DetourDuplicateHandle, reinterpret_cast<LPVOID *>(&fpDuplicateHandle));
//                         ret = MHCreateHook((LPVOID)&CloseHandle, (LPVOID)&DetourCloseHandle, reinterpret_cast<LPVOID *>(&fpCloseHandle));
//                     }

//                     //Enable Hook
//                     void *addressMHEnableHook = (void *)GetProcAddress(detourHandle, "MH_EnableHook");
//                     if (addressMHEnableHook != nullptr)
//                     {
//                         typedef int(WINAPI * MH_EnableHook)(LPVOID);
//                         MH_EnableHook MHEnableHook = (MH_EnableHook)addressMHEnableHook;

//                         ret = MHEnableHook((LPVOID)&MessageBoxW);
//                         ret = MHEnableHook((LPVOID)&SetWindowPos);
//                         ret = MHEnableHook((LPVOID)&CreateNamedPipeW);
//                         ret = MHEnableHook((LPVOID)&CreateFileW);
//                         ret = MHEnableHook((LPVOID)&ReadFile);
//                         ret = MHEnableHook((LPVOID)&WriteFile);
//                         ret = MHEnableHook((LPVOID)&DuplicateHandle);
//                         ret = MHEnableHook((LPVOID)&CloseHandle);
//                     }
//                 }
//             }
//         }
//         //  MessageBoxW(NULL, L"Not hooked...", L"MinHook Sample", MB_OK);
//     }
// #endif
// }

    bool FileInfo::createTempFileAndReadWithFlagDeleteOnClose()
        {
            //Flags::FLAG_CREATE_ALWAYS | Flags::FLAG_READ |
            //Flags::FLAG_WRITE |
            //Flags::FLAG_SHARE_DELETE
            //Flags::FLAG_TEMPORARY | Flags::FLAG_HIDDEN| Flags::FLAG_DELETE_ON_CLOSE
            //FILE_ATTRIBUTE_TEMPORARY | FILE_FLAG_DELETE_ON_CLOSE

            //initMinhook();
            HANDLE WINHandle = CreateFile(TEXT("D:\\SourceCodeTest\\Leetcode\\org.png"),  (GENERIC_WRITE | GENERIC_READ), (FILE_SHARE_READ | FILE_SHARE_WRITE |
                    FILE_SHARE_DELETE), 0, CREATE_ALWAYS,  FILE_ATTRIBUTE_TEMPORARY | FILE_FLAG_DELETE_ON_CLOSE, 0);
                    bool isOK = true;
                    return isOK;

            CloseHandle(WINHandle);
        }

} // namespace my_util


 