
#include "../pch.h"

namespace my_util{

  class FileWrite{

  public:
      FileWrite(std::wstring path);
      bool createFile();
      bool writeText(std::string text);

  private:
       std::wstring file_path;
       HANDLE file_handle;
  };

}