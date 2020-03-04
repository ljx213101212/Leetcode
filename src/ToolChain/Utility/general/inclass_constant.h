//http://www.stroustrup.com/bs_faq2.html#in-class
/**
 * How do I define an in-class constant?
If you want a constant that you can use in a constant expression, say as an array bound, you have two choices:

class X {
	static const int c1 = 7;
	enum { c2 = 19 };

	char v1[c1];
	char v2[c2];

	// ...
};
At first glance, the declaration of c1 seems cleaner, but note that to use that in-class initialization syntax, the constant must be a static const of integral or enumeration type initialized by a constant expression. That's quite restrictive:
class Y {
	const int c3 = 7;		// error: not static
	static int c4 = 7;		// error: not const
	static const float c5 = 7;	// error: not integral
};
I tend to use the "enum trick" because it's portable and doesn't tempt me to use non-standard extensions of the in-class initialization syntax.
So why do these inconvenient restrictions exist? A class is typically declared in a header file and a header file is typically included into many translation units. However, to avoid complicated linker rules, C++ requires that every object has a unique definition. That rule would be broken if C++ allowed in-class definition of entities that needed to be stored in memory as objects. See D&E for an explanation of C++'s design tradeoffs.

You have more flexibility if the const isn't needed for use in a constant expression:

	class Z {
		static char* p;		// initialize in definition
		const int i;		// initialize in constructor
	public:
		Z(int ii) :i(ii) { }
	};

	char* Z::p = "hello, there";
You can take the address of a static member if (and only if) it has an out-of-class definition:
	class AE {
		// ...
	public:
		static const int c6 = 7;
		static const int c7 = 31;
	};

	const int AE::c7;	// definition

	int f()
	{
		const int* p1 = &AE::c6;	// error: c6 not an lvalue
		const int* p2 = &AE::c7;	// ok
		// ...
	}
 */ 