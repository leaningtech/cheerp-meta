---
title: JSExport attribute
---

## Introduction

The ```[[cheerp::jsexport]]``` attribute can be applied in C++ code either to free functions or to ```class``` and ```struct``` definitions.
Every definition tagged with this attribute will be directly exposed to JavaScript.

That means that a function with the same name will be available.

Example:

```c++
#include <cheerp/clientlib.h>

[[cheerp::jsexport]] int factorial(int n)
{
        if (n < 2)
                return 1;
        return n * factorial(n-1);
}

class [[cheerp::jsexport]] JsStruct
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
```

This example exposes ```factorial``` free function to be called like a regular JavaScript function.
The ```JsStruct``` class will also be exported and instances can be created via ```new```  in JavaScript, for example:

```js
var testExport = new JsStruct(3.0);              // Instance created
testExport.test();                               // Invoked test 3.0 1
testExport.test();                               // Invoked test 3.0 2

console.log(factorial(23));                      // 862453760
```

## JSExported free functions limitations
 * arguments/return types: no 64 bit integers (we may support this in the future using BigInt). The basic ```int```/```float```/```double``` types are supported.
 * arguments/return types: only pointers (and references) to struct/classes tagged as ```[[cheerp::jsexport]]``` (representing native-like objects) or pointers (and references) to "native" JavaScript objects (DOM elements, Strings, Arrays, Sets or more complex objects declared on the JavaScript code). Pointers to functions (with some restrictions) are also supported.
 * naming: there can't be 2 jsexported function that share the same name (as in the literal name, since C++ overloading rules are based on parameter types and that does not naturally map to JavaScript we have to be more restrictive)
 * no templated functions (there is no way of instantiating templates at runtime from JavaScript)

Free function tagged with ```[[cheerp::genericjs]]``` or ```[[cheerp::wasm]]``` (directly or via the ```-target``` option) are both compatible with the ```[[cheerp::jsexport]]``` attribute.

## JSExported class/struct limitations
 * no inheritance
 * the class/struct must have a trivial destructor, which means that all members must have a trivial destructor as well
 * the class/struct needs to have a public constructor
 * the class/struct needs to have at least a non-static jsexported method
 * the class has to be tagged with the ```[[cheerp::genericjs]]``` attribute
 * "promise" and "valueOf" are reserved names

The same restrictions on free functions are also imposed on all JSExported methods.

Some of these restrictions will be relaxed in future releases, but we strive to offer a forward compatible interface, so that a code compiled with the current restrictions will be also valid in future releases.

Requiring that the class has a trivial destructor means that no custom operation can happen within the destructor. This allows to fully rely on the JavaScript garbage collector to clean up the object.

Member fields (either static or not) are not JsExported (but you define getter / setter methods).

## Promise
All JSExported functions or classes have a member .promise, and no use of these classes or functions should be done before the promise has been resolved. Since the promise may generally include asynchronous fetching of files via HTTP (e.g. the wasm file), the recommended behaviour is to always check that the promise has been resolved. Eg.:
```js
yourFunctionName.promise.then(function() {
             arbitraryCode();
             yourFunctionName(4, 5.023);
             //etc
       });
```

## Advanced usage: selecting JSExported interface
The default behaviour of the compiler is that if a class has a tag ```[[cheerp::jsexport]]```, all its public methods (static or not) will be JSExported. This can be relaxed by selecting a non-empty subset of the public interface, and tagging only the methods you are interested in exporting with the attribute ```[[cheerp::jsexport]]```.

As with all other Cheerp-specific attributes, other compilers will still parse code, so it can be used to target different platforms (eg. native and Web).

Special considerations apply when using the ```jsexport``` attribute and WebAssembly output, for more information [see here](https://github.com/leaningtech/cheerp-meta/wiki/WebAssembly-output-(wasm-or-wast-mode)#using-cheerpjsexport-in-combination-with-webassembly).

