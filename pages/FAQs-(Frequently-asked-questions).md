---
title: Frequently Asked Questions
---

## The size of cheerp output JavaScript code is very large. Is this normal? What can I do to reduce it?

If you only use browser APIs in your code and no functions from the C or C++ library you can compile using the ```-nostdlib``` option. This will reduce the code size by not linking in initialization code for ```<iostream>``` functionalities. Please node that Cheerp already compiles only the code which is actually used, but currently the static initializations of ```std::cout``` and similar objects are seen as executed code.

## Does Cheerp enforce C++ type safety at run-time? 

No. Similarly to what happens when compiling native code from C++, type information is only used to enforce type safety at compile time. Generated code (both from cheerp or from traditional compilers) keeps no type information at run-time. This also means that type safety has no overhead at run-time. If you change the internal representation of an object using e.g. a JavaScript debugger, the results will probably be bad, unless you know very well what you are doing.

## What features of C++ are currently not supported?

cheerp currently supports most of C++ features. While we try very hard to have a full C++14 compatibility, some features are currently not supported:

* Global ordering of pointers. This means that pointers have a definite ordering (as required by the standard) only if they are part of the same array. Unrelated pointers at the moment have undefined ordering, which means that constructs like ```std::map<void*>``` will not work in cheerp.
* Exceptions. This limitation is caused by the fact that JavaScript engines handle JavaScript exceptions very poorly. There are ways to workaround this limitation and exceptions will be supported in the future. To simplify the porting of existing codebases you can pass the ```-fexceptions``` command line to Cheerp, the compiler will correctly parse and ignore exception handlers. If an exception is actually thrown at runtime an abort will be triggered.

## Is cheerp free software? Or is it proprietary?

Cheerp is 100% free software. The cheerp compiler, headers and libraries are licensed under the Apache License v2.0 with LLVM Exceptions license, same as the upstream LLVM/Clang projects. The musl implementation of the C library is released under the MIT licence. Both licenses are permissive and allow Cheerp to be used for both commercial and non-commercial projects with no strings attached.

## Should I release my code if I use cheerp?

No. Neither the Apache License nor the MIT license are "copyleft". You are under no obligation of releasing any of your code if you use Cheerp.
