---
title: WebAssembly output
---

# Generate WebAssembly output

Setting the command line flag `-target cheerp-wasm` lets Cheerp generate binary WebAssembly output:

```
/opt/cheerp/bin/clang++ \
  -target cheerp-wasm \
  -cheerp-linear-heap-size=128 \
  -o output.js \
  input.cpp
```

The command above will generate a JavaScript file `output.js` and a corresponding binary WebAssembly module `output.wasm` from the C++ source file `input.cpp`.

The JavaScript file includes all C++ code compiled to JavaScript (code with the `[[cheerp::genericjs]]` attribute), and the code necessary to load the WebAssembly module and run the program.

You can specify a different name and runtime path for the wasm file with the `-cheerp-secondary-output-file=<file>` and `-cheerp-secondary-output-path=<path>` options respectively.

For debugging purposes, human-readable WebAssembly output can be generated using `-cheerp-linear-output=wasm`.
For older browsers or for easier inspection of the generated code, AsmJS output is also supported, with the `-cheerp-linear-output=asmjs` option.

# Avoid Wasm traps
The Wasm standard mandates that certain undefined operations should trap and stop the execution of the program (possibly notifying back what happened wrongly). This could serves as run-time checker (es out-of-bound memory accesses lead to a trap) but it's also could lead to some false positives being raised. An example: converting between int and double may lead to a trap being raised, while in C++ it has a well defined (at a cost of possible loss of precision) meaning.
If you want to avoid the trap mechanism, passing `-cheerp-avoid-wasm-traps` do as the name suggests, and generates non-trapping Wasm code.

# WebAssembly heap size

Since WebAssembly uses linear memory with a certain size, the heap size can be set using `-cheerp-linear-heap-size=128`. The unit of the `-cheerp-linear-heap-size` is megabytes, and when omitted defaults to 1 megabyte.

# Using DOM APIs

From WebAssembly it is not possible to call any DOM APIs, not even the basic ```client::console.log```. Cheerp provides automatic generation of FFI/code bridges to indirectly use DOM API though. As an example take the following code.

```
#include <cheerp/clientlib.h>

[[cheerp::genericjs]] int domOutput(const char* str)
{
    client::String* s = new client::String(str);
    client::console.log(s);
    // Also add it to the DOM for good measure
    client::document.get_body()->set_textContent(s);
    return s->get_length();
}

void webMain()
{
    int len = domOutput("Hello WASM!");
    assert(len == 11);
}
```

# Using cheerp::jsexport in combination with WebAssembly

```[[cheerp::jexport]]``` makes it possible to expose C++ classes and functions to JavaScript. [(See here for details on jsexport)](JSExport-attribute).
Free functions can be tagged ```[[cheerp::jsexport]]``` independently of the compilation mode they are tied to (either ```[[cheerp::genericjs]]``` or ```[[cheerp::wasm]]```). For Wasm tagged functions, a wrapper will be exposed on the JavaScript side.
Classes and structs currently supports only the ```[[cheerp::genericjs]]``` compilation mode, so it should be manually added to each JSExported class/struct. This limitation may be relaxed in the future, but for now this means that code compiled to Wasm needs to be exposed from a _genericjs_ class.

As the loading of the Wasm module must be asynchronous it is also necessary to wait for the completion of the process before using jsexport-ed code. To achieve this Cheerp expose a promise property in every jsexport-ed class and function. The promise can (and should) be used to know when the code is eventually ready to run.

```c++
class [[cheerp::jsexport]] [[cheerp::genericjs]] JsStruct
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
```

```js
JsStruct.promise.then(function(){
        var obj = new JsStruct();
        obj.test();
});
```
