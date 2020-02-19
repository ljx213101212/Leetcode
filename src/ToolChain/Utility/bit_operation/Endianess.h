#include "../pch.h"

namespace my_util{


class SampleOfEndianess{
   

public:

    //Little-endianness is the dominant ordering for processor architectures(x86, most ARM implementations)
    bool isLittleEndian()
    {
        short int number = 0x1;
        char *numPtr = (char*)&number;
        return (numPtr[0] == 1); //If you are using normal PC , most cases is true.
    }
  
    void charToUINTEndianTest()
    {
        //simulate memory sequence.
        //if the CPU endianess is small endian (networking-byte-order)
        char data_read[4] = {0x00,0x00,0x00,0x1C};//28
        //covert char to UINT 4 bytes
        //the result will swap the data_read sequence to 0x1C 0x00 0x00 0x00.If is small endian.
        const unsigned int cpp_style_cast_size = *reinterpret_cast<unsigned int*>(data_read);
        const unsigned int c_style_cast_size = *((unsigned int*)data_read);
        //if its little endian -> 0x1C000000
        //if its big endian -> 0x1C
        //the unsigned int is always 28d ,only difference is the memory sequence to store this 28d in memory.
        cout<<"Hello World" << " " << cpp_style_cast_size << " " << c_style_cast_size <<endl;
    }

    void uIntToCharEndianTest(){
        //Lets check how current system stores integer 28.
        unsigned int data_read = 28;
        //little endian -> 0x1C 0x00 0x00 0x00
        //big endian -> 0x00 0x00 0x00 0x1C
        char *c_style_cast_data = (char*)&data_read;
        cout << "char[0] (int): "<<(int)c_style_cast_data[0] << endl;
        cout << "char[1](int): "<<(int)c_style_cast_data[1] << endl;
        cout << "char[2](int): "<<(int)c_style_cast_data[2] << endl;
        cout << "char[3](int): "<<(int)c_style_cast_data[3] << endl;
        //expect 28(0x1C) 0 0 0 
    }
};
}
