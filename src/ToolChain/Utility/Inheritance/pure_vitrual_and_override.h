#include <iostream>
using namespace std;

//https://stackoverflow.com/questions/53809865/undefined-reference-error-in-overriding-virtual-functions-in-c/53809941#53809941

//Whole idea
//A virtual member function has to be defined, be pure virtual, or both
class Base {
  public:
    virtual void doAdd() = 0;// Pure virtual
};

class Child : public Base{
    public :
    void doAdd() override;
};

void Child::doAdd(){
    int res = 10+10;
    res = 1;
}

struct A
{
    virtual void foo(){};//defined. if not -> virtual void foo(); then you will get undefined-reference-error
};
 
struct B : A
{

    void foo() override; // OK: B::foo overrides A::foo
};

void B::foo (){
    
    cout << "oh,my god" << endl;
}
