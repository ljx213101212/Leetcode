#include "file_write.h"

namespace my_util
{
    FileWrite::FileWrite(std::wstring path) : file_path(path)
    {
        createFile();
    }

    bool FileWrite::createFile()
    {

        file_handle = CreateFileW((LPCWSTR)file_path.c_str(),   // name of the write
                                 GENERIC_READ | GENERIC_WRITE, // open for writing
                                 (FILE_SHARE_READ | FILE_SHARE_WRITE |
                                  FILE_SHARE_DELETE),   // do not share
                                 NULL,                  // default security
                                 CREATE_ALWAYS,         // open existing file only
                                 FILE_ATTRIBUTE_NORMAL, // normal file
                                 NULL);                 // no attr. template

        return file_handle != NULL;
    }

    VOID WINAPI FileWrittenCallback(DWORD dwErrorCode, DWORD dwBytesTransferred, LPOVERLAPPED lpOverlapped) {
        if (dwErrorCode != 0)
        {
            fprintf(stdout, "CompletionRoutine: Unable to write to file! Error: %u, AddrOverlapped: %p\n", dwErrorCode, lpOverlapped);
        }
        else
        {
            fprintf(stdout, "CompletionRoutine: Transferred: %u Bytes, AddrOverlapped: %p\n", dwBytesTransferred, lpOverlapped);
        }
    }

    bool FileWrite::writeText(std::string text)
    {

        if (file_handle == NULL)
            return false;

        LPOVERLAPPED lpPipeInst;
        LPOVERLAPPED_COMPLETION_ROUTINE lp_r;

        bool is_ok = WriteFileEx(file_handle, text.c_str(), text.length() + 1, (LPOVERLAPPED)lpPipeInst, (LPOVERLAPPED_COMPLETION_ROUTINE)FileWrittenCallback);

        CloseHandle(file_handle);
        if (lpPipeInst != NULL)
            GlobalFree(lpPipeInst);
        return is_ok;
    }
} // namespace my_util