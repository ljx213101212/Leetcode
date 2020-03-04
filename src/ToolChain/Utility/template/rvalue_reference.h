#include "../pch.h"
namespace my_util{

/**
 * @see
 * https://stackoverflow.com/questions/5481539/what-does-t-double-ampersand-mean-in-c11
 * https://stackoverflow.com/questions/3582001/advantages-of-using-forward/3582313#3582313
 * http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2002/n1385.htm
 * http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2004/n1690.html
 */ 
class SampleOfRvalueReference{
   

public:
    void overloaded( int const &arg ) { std::cout << "by lvalue\n"; }
    void overloaded( int && arg ) { std::cout << "by rvalue\n"; }
    template< typename t >
    /* "t &&" with "t" being template param is special, and  adjusts "t" to be
    (for example) "int &" or non-ref "int" so std::forward knows what to do. */
    void forwarding( t && arg ) {
        std::cout << "via std::forward: ";
        overloaded( std::forward< t >( arg ) );
        std::cout << "via std::move: ";
        overloaded( std::move( arg ) ); // conceptually this would invalidate arg
        std::cout << "by simple passing: ";
        overloaded( arg );
    }

    void myTestMain(){
        std::cout << "initial caller passes rvalue:\n";
    forwarding( 5 );
    std::cout << "initial caller passes lvalue:\n";
    int x = 5;
    forwarding( x );
    }
};
}
