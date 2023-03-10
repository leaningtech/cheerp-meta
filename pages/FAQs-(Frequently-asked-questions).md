---
title: Frequently Asked Questions
---

## Is Cheerp free software? Or is it proprietary?

Cheerp is 100% free software. The Cheerp compiler, headers and libraries are licensed under the Apache License v2.0 with LLVM Exceptions license, the same license as the upstream LLVM/Clang projects. The musl implementation of the C library is released under the MIT licence. 

Both licenses are permissive and allow Cheerp to be used for both commercial and non-commercial projects with no strings attached.

## Should I release my code if I use cheerp?

No. Neither the Apache License nor the MIT license are "copyleft". You are under no obligation of releasing any of your code if you use Cheerp.

## Does Cheerp enforce C++ type safety at run-time? 

No. Similarly to what happens when compiling native code from C++, type information is only used to enforce type safety at compile time. Generated code (both from Cheerp or from traditional compilers) keeps no type information at run-time. This also means that type safety has no overhead at run-time. If you change the internal representation of an object using e.g. a JavaScript debugger, the results will probably be bad, unless you know very well what you are doing.

## What features of C++ are currently not supported?

Cheerp currently supports most of C++ features up to C++20. While we try very hard to have a full C++20 compatibility, some features are currently not supported in JavaScript mode. All C++20 features are expected to work in WebAssembly mode.

* Global ordering of pointers . This means that pointers have a definite ordering (as required by the standard) only if they are part of the same array. Unrelated pointers at the moment have undefined ordering, which means that constructs like ```std::map<void*>``` will not work in Cheerp when compiling to JavaScript.

## Does Cheerp support C++ excepptions?

Yes, exceptions are supported in both WebAssembly and JavaScript modes. They must be enabled by passing the ```-fexceptions``` command line to Cheerp, following which the compiler will correctly parse and implement exception handlers.
