
#include "../pch.h"

namespace my_util{

    class FileInfo{
        public:
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
    };

}