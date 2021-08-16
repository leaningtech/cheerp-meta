Cheerp generates JavaScript code, which should be integrated into an HTML page to be executed in a browser. The following trivial example should be ok to start.

```html
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <title>Cheerp test</title>
    <script src="cheerp_output.js"></script>
  </head>
  <body>
  </body>
</html>
```

You can also run the generated JavaScript code directly using a command line JavaScript shell like node.js

# Single step JS generation

```bash
/opt/cheerp/bin/clang++ -O3 -target cheerp <file1.cpp> <file2.cpp> -o <file.js>
```
# Separate objects compilation and linking

```bash
/opt/cheerp/bin/clang++ -O3 -c -target cheerp <file1.cpp> -o <file1.bc>
/opt/cheerp/bin/clang++ -O3 -c -target cheerp <file2.cpp> -o <file2.bc>
/opt/cheerp/bin/clang++ -target cheerp <file1.bc> <file2.bc> -o <linkedfile.js>
```

# Building libraries

Libraries are stored using LLVM binary representation

```bash
/opt/cheerp/bin/clang++ -O3 -c -target cheerp <file1.cpp> -o <file1.bc>
/opt/cheerp/bin/clang++ -O3 -c -target cheerp <file2.cpp> -o <file2.bc>
/opt/cheerp/bin/llvm-link <file1.bc> <file2.bc> ... -o <library.bc>
```

# Using cmake

A cmake toolchain file is provided in `/opt/cheerp/share/cmake/Modules/CheerpToolchain.cmake`
it's possible to cross-compile cmake projects for the Cheerp platform using the syntax

```bash
cmake -DCMAKE_TOOLCHAIN_FILE=/opt/cheerp/share/cmake/Modules/CheerpToolchain.cmake <regular arguments>
```

Please note that, currently, only building static libraries is supported.

## The ```webMain``` entry point

By convention the method that will be called first on the client side should have the following signature

```c++
void webMain()
```

The ```main``` function is the traditional
entry point of a C++ program. It also contains the whole program, which terminates after the ```main``` function returns. On a Cheerp application, the ```webMain``` function plays the role of the entry point in a very different manner.  ```webMain``` initializes the program, sets up event handlers and returns control to the browser as soon as possible. After returning, the browser notifies the events you registered handlers for.

When ```webMain``` is absent, ```main``` defaults to it, and in some cases the behaviour may be the same, but what happens for example here?
```c++
#include <iostream>

class C
{
public:
        C()
        {
                std::cout << "Constructor\n";
        }
        ~C()
        {
                std::cout << " and Destructor\n";
        }
};

C global;

int main()
{
        return 0;
}
```

Try to compile it with a C++ to x86 compiler, and the result should be `Constructor\n and Destructor\n` while the Cheerp version's output will be only `Constructor\n`. Basically global objects survives the call to the webMain, that is meant for performing initializations/setting up event listeners.

## What is `client::console.log()`?

```client``` is a C++ namespace that contains all browser APIs declared in cheerp/clientlib.h. The ```console``` object represent the console object of the browser. In the example, the ```console.log()``` function is invoked with a C string literal argument that will be implicitly converted to a JavaScript String.

The header ```cheerp/clientlib.h``` contains declarations for the browser APIs. All functions are only declared so that the Cheerp C++ compiler can use them, but they are simply forwarded (with no additional overhead) to the browser at runtime. No implementation of these functions is provided by Cheerp, so that all browser APIs have the semantics provided by the browser you are currently using.

## Accessing Web/HTML5/DOM/Browser APIs

Global variables and types that are provided by the browser environment are accessible through the 'client' namespace

```c++
client::document.addEventListener(...)
```

Of course, being a regular C++ namespace you can reduce code verbosity by

```c++
using namespace client;
```

The relevant headers that defines client interfaces are

```c++
#include <cheerp/client.h> //Misc client side stuff
#include <cheerp/clientlib.h> //Complete DOM/HTML5 interface
#include <cheerp/webgl.h> //WebGL interface
```

## Waiting DOMContentLoaded

As usual in JavaScript the code will be executed before the DOM is completely loaded, and so webMain will be called on an incomplete document.
To execute a callback after loading is complete you can do this:

```c++
void loadCallback()
{
        ...
}

void webMain()
{
        document.addEventListener("DOMContentLoaded",Callback(loadCallback));
}
```

## Accessing DOM properties
In a similar way you can bind to any other DOM event

Properties of DOM objects are always accessed through setters/getters

```c++
{
        auto body = document.get_body();
}
```

Small examples are included in `/opt/cheerp/share/cheerp/examples/`
