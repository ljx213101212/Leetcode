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

//#include "ToolChain/Utility/file/file_info.h"
//#include "ToolChain/Utility/smart_pointer/unique_pointer_deleter.h"
//#include "ToolChain/Utility/bit_operation/Endianess.h"
//#include "ToolChain/Utility/template/rvalue_reference.h"
// #include "ToolChain/Utility/thread/multithread_debugging.h"
// #include "ToolChain/Utility/win32_api/registry_crud.h"
#include "ToolChain/Utility/file/file_write.h"

int main()
{
    cout<<"Hello World";
    // my_util::SampleOfEndianess test_obj;
    // cout << test_obj.isLittleEndian() << endl;
    // test_obj.charToUINTEndianTest();
    // test_obj.uIntToCharEndianTest();
    // my_util::SampleOfMultithreadDebugging test_obj;
    // test_obj.testMain();
    // my_util::FileInfo test_obj;
    // test_obj.createTempFileAndReadWithFlagDeleteOnClose();

    // my_util::SampleOfRvalueReference test_obj;
    // test_obj.myTestMain();
    // my_util::SampleOfRegistry test_obj;
    //
    // test_obj.EnumHardwareId(L"{4D36E96B-E325-11CE-BFC1-08002BE10318}");

    // my_util::FileWrite file(L"C:\\Users\\jixiang.li\\Desktop\\leetcode.txt");
    // file.writeText("Helloworld! hahaha 😀中文");
    string test = "wo de tian";
    std::ofstream ofs;
    ofs.open("C:\\Users\\jixiang.li\\Desktop\\leetcode.txt", std::ofstream::out | std::ofstream::app);
    ofs<<test + "\r\n";
    ofs.close();
    return 0;

}
