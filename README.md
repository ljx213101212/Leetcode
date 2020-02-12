## Table of Contents
- [Reading materials](#reading-materials)

## Pending to understand
https://stackoverflow.com/questions/120876/what-are-the-rules-for-calling-the-superclass-constructor


## Reading Materials

[Comprehensive Data Structure and Algorithm Study Guide PDF Format](docs/CI_DSA_study_guide.pdf)

# Categories
* Dynamic Programing
1. 顺序区间
- [Normal - 1130](https://github.com/ljx213101212/Leetcode/blob/master/src/DynamicPrograming/1130.h)
- [Normal - 877](https://github.com/ljx213101212/Leetcode/blob/master/src/DynamicPrograming/877.h)

* Math
1. Unit distance code/ Gray code/ Circular 
- [Normal - 1238](https://github.com/ljx213101212/Leetcode/blob/master/src/Math/1238.h)

# To do
加入Google Test, 学习并掌握test driven development 的工作模式

# Solution

## 使用visual studio code 写C++ 怎么样才能正确地引用其他文件的函数
### 答(ref:https://segmentfault.com/q/1010000015905887/a-1020000015912031):
因为g++不会正确的编译其他的CPP文件，你可以看VSCode的命令：
g++ /Users/stanhu/Desktop/Git/Foundation/CPP_Learn/main.cpp -o /Users/stanhu/Desktop/Git/Foundation/CPP_Learn/main.out -g -Wall -fcolor-diagnostics -std=c++11
只编译了main.cpp文件而没有编译a.cpp文件，所以会出现symbols for architecture x86_64错误
所以正确的做法是：g++ main.cpp a.cpp -o main.out
再执行./main.out
就没问题了，所以现在问题出来了，那么怎么样才能让VSCode正确的编译所有链接的CPP文件呢。

## Undefined symbols for architecture x86_64: Solution::sortArrayByParityII()", referenced from:_main in main-e4143f.o ld: symbol(s) not found for architecture x86_64
### 基本原因：https://stackoverflow.com/questions/7533321/error-when-compiling-some-simple-c-code
Normally this sort of failure happens when compiling your C++ code by invoking the C front-end. The gcc you execute understands and compiles the file as C++, but doesn't link it with the C++ libraries. Example:

### 根本原因: 
在922/922.cpp 里面 vector<int> Solution::sortArrayByParityII
我原来写成了 vector<int> sortArrayByParityII， 成员函数实现的时候忘记申明类修饰符



## Win 64 GDB STL support
### https://codeyarns.com/2014/07/17/how-to-enable-pretty-printing-for-stl-in-gdb/
python
import sys
sys.path.insert(0, '/home/joe/stlprettyprinter')
from libstdcxx.v6.printers import register_libstdcxx_printers
register_libstdcxx_printers (None)
end

## Debugger hangs starting gdb.exe on Windows 10 with UTF-8 enabled.
### 解决方案:
https://github.com/microsoft/vscode-cpptools/issues/3353
https://github.com/Microsoft/vscode-cpptools/issues/1527
I disable unicode support on Windows codepage settings, now it works.

## clang++ command:
### Mac OS
clang++ -g -std=c++11 922/922.cpp main.cpp -o main.out
### Win 64 （open VS code in admin mode)
g++ -g -I include 922/922.cpp -o main main.cpp
