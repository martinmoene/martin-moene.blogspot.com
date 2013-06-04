#include <memory>

struct S
{
    S() : ap( new int(77)) {}

    S( S const & other ) : ap( other.ap ) {}

    std::auto_ptr<int> ap;
};

#include <iostream>

int main()
{
    S s;

    std::cout << "s.ap:" << (*s.ap) << " p:" << s.ap.get() << std::endl;
    {
    S t = s;

//    std::cout << "s.ap:" << (*s.ap) << " p:" << s.ap.get() << std::endl;
    std::cout << "t.ap:" << (*t.ap) << " p:" << t.ap.get() << std::endl;
    }
    {
    S t; t = s;

//    std::cout << "s.ap:" << (*s.ap) << " p:" << s.ap.get() << std::endl;
//    std::cout << "t.ap:" << (*t.ap) << " p:" << t.ap.get() << std::endl;
    }

    std::auto_ptr<int> p1( new int(33) );

    std::cout << "p1:" << (*p1) << " p1:" << p1.get() << std::endl;

    std::auto_ptr<int> p2;
    p2 = p1;

//    std::cout << "p1:" << (*p1) << " p1:" << p1.get() << std::endl;
    std::cout << "p2:" << (*p2) << " p2:" << p2.get() << std::endl;

    p1.reset(0);
//    std::cout << "p1:" << (*p1) << " p1:" << p1.get() << std::endl;
}

// cl -nologo -EHsc auto_ptr-in-class.cpp && auto_ptr-in-class.exe
// g++ -fpermissive -o auto_ptr-in-class.exe auto_ptr-in-class.cpp && auto_ptr-in-class.exe
