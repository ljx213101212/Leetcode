
#include "../pch.h"

namespace my_util{

    class FileInfo{
    public:
        //@see https://source.chromium.org/chromium/chromium/src/+/master:base/files/file.h;bpv=0;bpt=1
        enum Flags
        {
            FLAG_OPEN = 1 << 0,           // Opens a file, only if it exists.
            FLAG_CREATE = 1 << 1,         // Creates a new file, only if it does not
                                          // already exist.
            FLAG_OPEN_ALWAYS = 1 << 2,    // May create a new file.
            FLAG_CREATE_ALWAYS = 1 << 3,  // May overwrite an old file.
            FLAG_OPEN_TRUNCATED = 1 << 4, // Opens a file and truncates it, only if it
                                          // exists.
            FLAG_READ = 1 << 5,
            FLAG_WRITE = 1 << 6,
            FLAG_APPEND = 1 << 7,
            FLAG_EXCLUSIVE_READ = 1 << 8, // EXCLUSIVE is opposite of Windows SHARE.
            FLAG_EXCLUSIVE_WRITE = 1 << 9,
            FLAG_ASYNC = 1 << 10,
            FLAG_TEMPORARY = 1 << 11, // Used on Windows only.
            FLAG_HIDDEN = 1 << 12,    // Used on Windows only.
            FLAG_DELETE_ON_CLOSE = 1 << 13,
            FLAG_WRITE_ATTRIBUTES = 1 << 14,    // Used on Windows only.
            FLAG_SHARE_DELETE = 1 << 15,        // Used on Windows only.
            FLAG_TERMINAL_DEVICE = 1 << 16,     // Serial port flags.
            FLAG_BACKUP_SEMANTICS = 1 << 17,    // Used on Windows only.
            FLAG_EXECUTE = 1 << 18,             // Used on Windows only.
            FLAG_SEQUENTIAL_SCAN = 1 << 19,     // Used on Windows only.
            FLAG_CAN_DELETE_ON_CLOSE = 1 << 20, // Requests permission to delete a file
                                                // via DeleteOnClose() (Windows only).
                                                // See DeleteOnClose() for details.
        };
        static std::string getFileExtention(std::string file_path)
        {

            std::string file_ext;
            size_t sep = file_path.find_last_of("\\/");
            if (sep != std::string::npos)
                file_ext = file_path.substr(sep + 1, file_path.size() - sep - 1);

            size_t dot = file_ext.find_last_of(".");
            if (dot != std::string::npos)
            {
                file_ext = file_ext.substr(dot, file_ext.size() - dot);
            }
            return file_ext;
        }

          bool createTempFileAndReadWithFlagDeleteOnClose();
    };
    

}