#include "pch.h"
// #include "Array/1217.h"
// #include "DynamicPrograming/1130.h"
//#include "Math/1238.h"
// #include "WCPlayGround/xxx.hpp"
// #include "HashTable/1252.h"
// #include "MyLab/pointers/DynamicArray.h"
// #include "Tree/5264.h"
// #include "String/1268.h"
//#include "Array/1213.h"

#include "ToolChain/Utility/file/file_info.h"


// std::string getFileExtention(std::string file_path){

//     std::string file_ext;
//     size_t sep = file_path.find_last_of("\\/");
//     if (sep != std::string::npos)
//         file_ext = file_path.substr(sep + 1, file_path.size() - sep - 1);

//     size_t dot = file_ext.find_last_of(".");
//     if (dot != std::string::npos)
//     {
//         file_ext = file_ext.substr(dot, file_ext.size() - dot);
//     }
//     return file_ext;
// }

int main(){
    



    std::string test_path("/home/jixiang/Pictures/prove.png");
    std::string ext = my_util::FileInfo::getFileExtention(test_path);

    cout << ext << endl;
    return 0;
}
