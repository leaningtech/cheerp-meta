---
title: Cheerp PreExecuter
---

# Introduction

Standard methods to reduce the size of JavaScript code are compression (using a packer or gzip/brotli through the HTTP server), and minification (e.g. removing comments and white spaces from the source, shortening variable names). This latter approach has been an integral part of Cheerp since the 1.1 release.

More aggressive methods that are commonly employed are constant propagation, dead code elimination and function inlining. This type of minimization typically takes a long time to perform, makes debugging the optimized code much harder, and is therefore typically only intended to be applied on release builds.

# Cheerp PreExecuter

Starting from Cheerp 1.2, our compiler includes the _PreExecuter_, an approach to further reducing the JavaScript output size that relies on a compile-time execution of portions of the code, and their substitution with constant values. Applying the PreExecuter leads to a significant reduction in the output size, particularly for small programs. Moreover, it also reduces the time-to-main, since global memory is directly initialized with the saved values instead of computed at runtime.

The PreExecuter is an optional feature of Cheerp, and needs to be activated with the `--cheerp-preexecute` command line flag. We currently use by default the PreExecuter on all the standard libraries shipped with Cheerp.

## Dead code elimination is not sufficient

The Cheerp PreExecuter goes a step further compared to other aggressive minimization methods. It optimizes the output by selectively executing portions of code at compile-time, recording the resulting global memory state, then substituting the code with a global memory initialization using the saved values.

Currently, Cheerp combines all C and C++ symbols into a single JavaScript output file. This is the equivalent of statically linking a program in the native world. Statically linking allows Cheerp to do global dead code elimination and whole program optimization during link time optimization. These optimizations are applied when -O1 or higher is used.

When global dead code elimination is applied, Cheerp compiles a simple hello world C++ program to 1 MB of JavaScript code. One of the reasons is that Cheerp links to the standard C++ library (libstdc++) which calls a complex constructor that initializes all its memory manually. In the native world, this constructor is executed at runtime and stored in the symbol llvm.global_ctors. That symbol marks the constructor as ‘used’ during compile time. As a result, it is not possible to remove the constructors at compile time using dead code elimination.

Although constructors cannot be eliminated (since that would change the semantics of the program), they can be evaluated at compile time. The constructors of libstdc++ do not depend on any run time input (e.g. environment variables or command line flags), which allows the compiler to compute the initialized values of the constructors at compile time. The initialized values are then stored in memory and loaded at run time.


# Effects of the PreExecuter - Benchmarks

In order to evaluate the effect of the PreExecuter, we will refer to two simple examples:

1. An Hello World program.

`    #include <cheerp/clientlib.h>`
    
    void webMain() {
        client::console.log("Hello world!");
    }

2. A simple program which uses the standard library's vector class.

`   #include <cheerp/clientlib.h>`

`   #include <vector>`

    class A {
    public:
        const char *a;
        A(int i) { a = nullptr; }
    };
    
    void webMain()
    {
        std::vector<A> v;
        client::console.log("sizeof(A):", sizeof(A), "== 4");
        client::console.log("v.end()-v.begin():", v.end()-v.begin(), "== 0");
        for(int i = 0; i < 10; i++)
            v.push_back(A(i));
        client::console.log("v.end()-v.begin():", v.end()-v.begin(), "== 10");
        client::console.log("v.size():", v.size(), "== 10");
        v.erase(v.begin() + 3);
        client::console.log("v.end()-v.begin():", v.end()-v.begin(), "== 9");
    }

The following plot illustrates the output size of these examples with different levels of optimization: our standard minification available in Cheerp 1.1, a more aggressive baseline minimization included in Cheerp 1.2, and the maximum optimization including the PreExecuter.

![PreExecuter Benchmarks](https://docs.google.com/spreadsheets/d/17fFM1YqhMV2-O0ZdymEv-5BXKP3KwjNDzibkkpWf7Aw/pubchart?oid=1266840768&format=image)

Compiling the hello world program with only `-O3` without minification results in a 1.3 MB JavaScript. With minification enabled by default now, the file size is reduced by two (659 KB). After applying the pre-execute pass, the file size reduces to a mere 548 bytes. That’s a three orders of magnitude difference compared to the minified output.

The simple vector program also compiles to a large JavaScript output (665 KB). Performing the pre-execute pass reduces the size to 6968 bytes, a two orders of magnitude difference. The relatively large file size compared to the hello world program is caused by `std::vector`’s method `push_back()`. That method includes a slow path that reallocates the array when the array is full and another element is about to be added. The for-loop is also unrolled 10 times, since the loop body is small, but that increases the file size a bit as well.

# Limitations

A global constructor can be pre-executed a compile time only if it meets certain criteria:

- All the code in the constructor needs to be **type safe** and representable with the [Cheerp Memory Model](https://github.com/leaningtech/cheerp-meta/wiki/Cheerp-memory-model). This is automatically true for all the code that compiles in `genericjs` mode, but not necessarily for code compiled to `asmjs` or `wasm`.

- The constructor cannot call code defined in the `client` namespace, or use the `__asm__` keyword.

