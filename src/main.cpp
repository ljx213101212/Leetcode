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
#include "ToolChain/Utility/bit_operation/Endianess.h"

int main()
{
    cout<<"Hello World";
    my_util::SampleOfEndianess test_obj;
    cout << test_obj.isLittleEndian() << endl;
    test_obj.charToUINTEndianTest();
    test_obj.uIntToCharEndianTest();
    return 0;

}
