---
title: JavaScript interoperability
---

Cheerp implements three advanced interoperability mechanisms to interface C++ code with pure JavaScript:

* Expose C++ classes and methods to JavaScript code
* Inlining JavaScript code within a C++ codebase.
* Use JavaScript methods and classes directly from C++

# The jsexport attribute

The ```[[cheerp::jsexport]]``` attribute can be applied to C++ class and struct definitions, to expose them to JavaScript.

[See here for in-depth information about how to use it and its limitations](JSExport-attribute).

A basic example of its usage would be:

```c++
class [[cheerp::jsexport]][[cheerp::genericjs]] JsStruct
{
private:
        float a;
        int b;
public:
        JsStruct(float _a, int _b):a(_a),b(_b)
        {
        }
        void test()
        {
                client::console.log("Invoked test");
        }
};

[[cheerp::jsexport]]
int factorial(int N) {
        if (N < 2)
                return 1;
        return N * factorial(N-1);
}
```

This example exposes the a factorial free function and a JsStruct class, and allows to create and use the JsStruct instances from JavaScript, for example:

```js
var testExport = new JsStruct(3.0, 42);
testExport.test();

console.log(factorial(5));
```

Classes or struct that have to be JSExported have to tagged with both ```[[cheerp::jsexport]]``` and ```[[cheerp::genericjs]]```.

# The \__asm__ keyword

A different mechanism allows to inline JavaScript code in the middle of a C++ application. Similarly to a traditional architecture, the ```__asm__``` keyword permits to write native (JavaScript) code. This functionality can be used to interface with external JS libraries which have no Cheerp compatible headers yet.

The following is a simple example of JavaScript inlining in C++:
```c++
__asm__("alert('Alert from __asm__')");
```

It is possible to pass arguments and get values back from inline \__asm__ code, here is a more complex example

```c++
int stringLength;
client::String* stringFromDom = ...;
__asm__("%1.length" : "=r"(stringLength) : "r"(stringFromDom));
assert(stringLength == stringFromDom->get_length());
```

The syntax follows the usual conventions of inline asm code, namely

```
__asm__(code : output constraints : input constraints : clobber constraints)
```

Both the input constraints and clobber constraints parts are optional.

There are some Cheerp specific limitations at this time:

* Only the ```"r"``` constraint is supported. It represents a local variable in the generated JS code (equivalent to a register on traditional architecture).
* Only integral types, floating point types and pointers to object in the ```client``` namespace (i.e. DOM or native JavaScript objects) can be used.

A working example/tutorial is here: [example of _ asmjs _ and namespace client on Github](https://gist.github.com/carlopi/c36e9b8f0eaf72c569491fadac331707)

Code inside ```__asm__``` tag should never throw to the external scope.


## Clobbering names

Cheerp minifies the output by default (unless the ```-cheerp-pretty-code``` option is used). This happens by assigning the smallest possible symbols to the most used local or global variables. If you need to use temporary variables in inline asm code you need to declare those variables in the clobber list, for example

```
__asm__("var jsTemp1=%0; var jsTemp2=jsTemp+1; console.log(jsTemp2);" : /*No output*/ : "r"(42) : /*Clobber list*/ "jsTemp1","jsTemp2"); // This will print out "43"
```

All names declared as clobbered will be globally excluded from the list of symbol that are used for minification. The effect is the same as marking those names as reserved using the ```-cheerp-reserved-names``` command line option. For best results we recommend to choose temporary names while keeping the following into account:

* Using very short names (i.e. 1 or 2 letters) will reserve those symbols during minification, which may have a significant impact in terms of code size.
* Using names with a leading underscore (i.e ```_foo```) may cause collisions with symbols in the global scope if ```-cheerp-pretty-code``` is used.
* For best compatibility we recommend to choose temporary values **at least 3 letters long and without a leading underscore**.

# The CHEERP_OBJECT macro

A common use case for inline asm is to return a literal object to JavaScript:
```
double field1 = 1;
client::String* field2 = new client::String("hello");
client::Object* result;
__asm__("{field1:%1,field2:%2}" : "=r"(result) : "r"(field1), "r"(field2));
```
This is usually much more performant than creating a `new client::Object()` and populating it with the `set_()` method.

The `CHEERP_OBJECT` macro can be used to achieve the same result without the boilerplate and with a more elegant syntax:
```
double field1 = 1;
client::String* field2 = new client::String("hello");
client::Object* result = CHEERP_OBJECT(field1, field2);
```
Currently the macro has the following limitations:

- The maximum number of arguments is 16
- The arguments need to be global or local variable names, and the same names will be used for the js object fields 


# The client namespace

Cheerp treats every function and class inside the ```client``` namespace as a declaration for something implemented by the browser or JavaScript. You are free to add new declarations there for functions implemented in JavaScript. For example:

```c++
namespace [[cheerp::genericjs]] client
{
	int someJavaScriptMethod(int arg1, const String& arg2);
}

[[cheerp::genericjs]]
void webMain()
{
	printf("JavaScript returned %i\n", client::someJavaScriptMethod(42, "This is converted to a JavaScript String"));
}
```

And on the JavaScript side:

```js
function someJavaScriptMethod(arg1, arg2)
{
	return arg1 - arg2.length;
}
```

# The CHEERP_SAFE_INLINE macro

As an alternative to directly using inline JS code Cheerp also provides a macro designed to quickly introduce type safe DOM code in existing code, which can be especially useful when compiling with the `cheerp-wasm` target.

```c++
CHEERP_SAFE_INLINE(returnType, (type1 arg1, type2 arg2), { /* C++ code that access the DOM */ }, param1, param2);
```

Example usage:
```c++
[[cheerp::wasm]] double wasmCompiledCode(const char* str)
{
    return CHEERP_SAFE_INLINE(double, (const char* s), {
        client::console.log(s);
        return client::Date::now();
    }, str);
}
```

# The ```cheerp::ArrayRef``` class and ```cheerp::makeArrayRef``` helpers

Cheerp provides an helper class to simplify access to JS array-like objects, such as ```client::Array``` and ```client::Int32Array```. It's common to pass them as pointers, which makes it inconvenient to use the ```operator[]``` to access their elements. The ```cheerp::ArrayRef``` class can be wrapper around a pointer to use the ```operator[]``` more naturally. ```cheerp::makeArrayRef``` is an helper to automatically deduce the right template type for the ```cheerp::ArrayRef``` class.

```c++
// Code without using cheerp::ArrayRef
int sumAllElements(client::Int32Array* ar)
{
    int ret = 0;
    for(int i=0;i<ar->get_length();i++)
        ret += (*ar)[i];
    return ret;
}

// Code with cheerp::ArrayRef
int sumAllElements(client::Int32Array* ar)
{
    auto ref = cheerp::makeArrayRef(ar);
    int ret = 0;
    for(int i=0;i<ref->get_length();i++)
        ret += ref[i];
    return ret;
}
```
