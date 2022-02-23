//compile command:
//	/opt/cheerp/bin/clang++ factorial.cpp -o factorial.js -O3
//or  	/opt/cheerp/bin/clang++ factorial.cpp -o factorial.js -O3 -target cheerp

#include <cheerp/clientlib.h>

[[cheerp::jsexport]] int factorial(int n)
{
        if (n < 2)
                return 1;
        return n * factorial(n-1);
}

class [[cheerp::jsexport]][[cheerp::genericjs]] JsStruct
{
private:
        float a;
        int b;
public:
        JsStruct(float _a):a(_a),b(0)
        {
                client::console.log("Instance created");
        }
        void test()
        {
                client::console.log("Invoked test", a, b++);
        }
};
