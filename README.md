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

##clang++ command:
clang++ -g -std=c++11 922/922.cpp main.cpp -o main.out
