This document provides a brief guide to approach the porting of a C/C++ application to an HTML5 application using Cheerp. It also highlights some of the practical limitations introduced by the object-based memory model used by Cheerp in mapping C++ objects into JavaScripts objects

# Background

Porting a C/C++ application to the Web using Cheerp is the process of obtaining a functional HTML5/JavaScript application starting from a native C++ code base. As a general rule, portable, multi-target code bases are much easier to port to the Web compared to single-target applications. In the framework of porting, the Web should be considered as a new target platform, with its own set of limitations and APIs to access system capabilities.

# Limitations of the Web platform

The main limitations to keep in mind that are introduced when targeting a Web application are:

* No real multithreading model is present (although a model similar to multiprocessing is available thanks to WebWorkers)
* No filesystem access (besides files that the user has explicitly selected)

The Cheerp compiler is developed following the philosophy of exposing both the features and limitations of the Web platform to the application. As such, any function exposed by the Web browser is available to be accessed from C/C++ code compiled with Cheerp, and conversely, all of its architectural limitations are exposed.

# Limitations introduced by Cheerp

The Cheerp compiler provides a fully dynamic memory model by mapping C++ objects into JavaScript objects. This model guarantees complete flexibility in memory usage, unlimited memory, and simple and efficient interoperability with browser APIs and JavaScript code. It also requires stricter type safety compared to standard compilers.

While most C++ applications, especially portable, multi-platform ones, have a sufficent level of type safety to be compilable with Cheerp, some C++ and many C applications and libraries will require some work to be portable using Cheerp. By default, Cheerp is quite verbose in reporting type safety issues, and its console output can be effectively used to guide the type-safety improvement efforts.

## Unsupported code and workarounds

### Custom allocators and type-unsafe pool allocators

Custom allocators and type unsafe pool allocators are not supported. The reason is that mapping C/C++ onto JavaScript objects requires compile-time knowledge of the allocated object type. Cheerp does not allow to allocate a chunk of memory and assign its type later on.

A recommended workaround is to disable custom allocators and directly call ```(struct destinationType*) malloc(sizeof(struct destinationType))``` or ```new destinationType()```. Some C libraries (Zlib is an example) call ```malloc```/```free``` indirectly through function pointers contained in a context structure. In such case you will need to replace the indirect call with direct calls to ```malloc``` with a well defined type.

### Code that requires a byte addressable space

C library functions such as ```qsort```/```bsearch``` are not supported with Cheerp (in ```genericjs``` mode) as they required a byte-addressable architecture. A workaround that can be used is to compile the C file as a C++ file and use C++ function equivalents, such as ```std::sort``` or ```std::binary_search``` from the C++ library. Similarily, ```memcmp``` is also not supported in ```genericjs``` mode, but can be replaced with ```cheerp::memcmp``` or ```CHEERP_MEMCMP``` from the ```cheerp/memory.h``` header.

### Internal pointer ordering

When compiling in ```genericjs``` mode (i.e. not in ```asmjs/wasm```) Cheerp will aggressively re-order members inside every ```struct/class``` to optimize the amount of memory consumed by the corresponding JS object. This means that the ordering between pointer to members is undefined. As an example consider the following code:

```c++
struct S
{
    char c1;
    char c2; 
};
...
S s;
assert((&s.c1) < (&s.c2));
```

On a native target (and when using Cheerp to compile ```asmjs/wasm``` code) the assertion above will always be valid. When compiling in ```genericjs``` mode the result is undefined (i.e. the assertion may trigger). This limitation is most usually not a problem. In case you need to maintain the ordering of members (for example, you need to memcpy the struct from a serialized buffer), you should use the ```[[cheerp::bytelayout]]``` attribute. With this attribute Cheerp will use a different representation which provide the normal ordering between members.

```c++
struct [[cheerp::bytelayout]] S
{
    char c1;
    char c2; 
};
```

### Exceptions

Exceptions are not supported when using Cheerp. Because the use of exception handling in JavaScript leads to highly unoptimized code, we have decided not to support exceptions at the moment.

### Memory manipulation 

Memory functions such as ```memset```, ```memcpy``` and ```memmove``` are supported, but the argument type must be known at compile time.

# A recommended approach to porting an application

At Leaning Technologies we have built quite some experience in porting large-scale C/C++ code bases using Cheerp. In our opinion, the fastest, most effective way to get to a fully working ported application is to follow a top-down approach. Cheerp is designed to help in the porting process by providing verbose error and warning messages that highlight the code that is definitely (in case of errors) or possibly (in case of warnings) not supported on the Web platform.

The top-down approach that we recommend is as follows:

1. Configure the build system to use the Cheerp compiler. Cheerp is derived from clang and it is mostly command line compatible with gcc/clang.
2. If your codebase is already multiplatform, use the Linux or Unix target as the starting point for the Cheerp version. Cheerp defines the ```__CHEERP__``` macro to make it possible to integrate Cheerp code in the platform specific sections of your codebases.
3. Get most of your cosebase to compile, by fixing - if immediately obvious - or commenting out sections using the ```#ifndef __CHEERP__/#endif``` idiom. You will be able to work those sections back in the code base later in the process, allowing you to prioritize the different parts.
4. This should get you quickly to a rough proof of concept, which you can integrate into a Web application and test.
5. Fix type safety and platform specific issues as required to get your tests and benchmarks to run.
6. Gradually reintroduce the commented sections of your code into the code base and test them, until you get to your complete application.

## Commenting out code

We recommend to comment out whole methods as opposed to sections of code. Undefined functions/symbols do not stop Cheerp from generating the output JavaScript, so it is a valid approach to comment out whole methods that cause trouble by using the ```#ifndef __CHEERP__/#endif``` idiom to proceed further with the porting.

If you need to comment out subsections of a method we recommend the following idiom to avoid potential issues.
```
#ifdef __CHEERP__
assert(false && "Missing support for ...");
#else
// Your original code, it will still compile if you switch to a native target
#endif
```

While commenting out code is effective to evaluate the scale of the required porting work and quickly getting to a proof-of-concept state, it should be kept in mind that it can also lead to issues that are difficult to debug, and a careful use of a version control system will help mitigating the risks of losing track of the root cause of some problems.

