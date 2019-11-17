#include "pch.h"

class temp{
    public:
    temp(int &d){
        d += 1;
    }
};
class DynamicArray{
    public:
    int ** notFreedAddress;
    void trySomething(){
        // auto q = make_shared<int>(42);
        // auto r = make_shared<int>(100);
        //q.reset();


    //     q = r;
    //    cout << *q << endl;
    //    q.reset();
    //    r.reset();

        // int * q1 = new int;
        //q1 = q.get();

    //    int * q2 = new int();
    //    q2 = q1;
        // cout << *q1 << endl;
        // constexpr int **temp = std::addressof(q1);
        //cout << &q1 << endl;
        // int **temp = std::addressof(q1);
        // cout << *temp << endl;
        // int **temp2 = std::addressof(q2);

        // int *** temp3 = std::addressof(temp);
        // cout << q1 << endl;
        // cout << temp << endl;
        // cout << *temp <<endl;
        // cout << **temp <<endl;
        // cout << temp3 <<endl;
        // cout << *temp3 <<endl;
        // cout << **temp3 <<endl;
        // cout << ***temp3 <<endl;

        // cout << &temp << endl;
        // delete q1;
        //delete q2;


        // int *p(new int(42));
        // int * q = p;
        // //delete q;
        // cout << *p << endl;
        // shared_ptr<int>a (new int(42));
        // cout << *a << endl;
        
        // unique_ptr<int>a (new int (42));
        //process(a);
        //int i = *a;

        // shared_ptr<int> p(new int(42));
        // int *q = p.get(); // ok: but don't use q in any way that might delete its
        // //pointer
        // { // new block
        //     // undefined: two independent shared_ptrs point to the same memory
        //     shared_ptr<int>(q);
          
        // }             // block ends, q is destroyed, and the memory to which q points is freed
        // int foo = *p; // undefined; the memory to which p points was freed

        // cout << foo <<endl;
        // shared_ptr<int>a(new int (42));
        // shared_ptr<int>b(a.get());
        // process(shared_ptr<int>(b));
        // cout << *a <<endl;
        //  cout << *b <<endl;
         //cout << a <<endl;
        //  cout << b << endl;

        // int i = 1;
        // temp c = temp(i);
        // cout << i << endl;

        int *ptr = new int(1);
        shared_ptr<int> a (ptr);
        shared_ptr<int> b (new int(*ptr));
        
         
        cout << *ptr<< endl;
    }

    void checkNotFreedAddress(){
        //cout << *reinterpret_cast<int *>(notFreedAddress) << endl;
    }

    //Wrong unique_ptr will be deleted in this scope which is not allowed.
    // void process(unique_ptr<int>ptr){
    // }

    void process(shared_ptr<int>ptr){
        *ptr = 43;
    }
};