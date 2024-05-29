<h1><img src="assets/logotype.svg" alt="Cheerp" height="96"></h1>

[![Discord server](https://img.shields.io/discord/988743885121548329?color=%237289DA&logo=discord&logoColor=ffffff)](https://discord.leaningtech.com)
[![GitHub Issues](https://img.shields.io/github/issues/leaningtech/cheerp-meta.svg)](https://github.com/leaningtech/cheerp-meta/issues)
[![npm](https://img.shields.io/npm/v/cheerp)](https://npm.im/cheerp)

Cheerp is an open-source, permissively licensed, enterprise-grade C/C++ compiler for Web applications. It compiles virtually any C/C++ code to WebAssembly and JavaScript.

[Documentation](https://labs.leaningtech.com/cheerp)  
[Issues](https://github.com/leaningtech/cheerp-meta/issues)  
[Discord server](https://discord.leaningtech.com)  

## What is Cheerp?

Cheerp is a C/C++ compiler to WebAssembly and JavaScript, based on and integrated into the LLVM/Clang infrastructure, and featuring numerous custom optimisation steps to maximise performance and minimise size of the compiled JavaScript/WebAssembly output. Thanks to this, Cheerp is the best performing, most optimised C++ to WebAssembly compiler available on the market.

Cheerp is used primarily to port existing C/C++ libraries and applications to HTML5, but can also be used to write high-performance Web applications and WebAssembly components from scratch. Using Cheerp, C/C++ code gets compiled into JavaScript/WebAssembly and optimised, with the right JavaScript interfaces being exposed, and can easily be integrated in a web application.

## Repository structure

This repository (cheerp-meta) holds Cheerp's npm packages and tracks Cheerp issues. 

Other repositories hold various components of Cheerp:

- [cheerp-compiler](https://github.com/leaningtech/cheerp-compiler) - primary
- [cheerp-musl](https://github.com/leaningtech/cheerp-musl)
- [cheerp-utils](https://github.com/leaningtech/cheerp-utils)
- [cheerp-libs](https://github.com/leaningtech/cheerp-libs)

## Licensing

Cheerp is licensed under the [Apache 2.0 License with LLVM exceptions](/LICENSE), i.e. the same license of upstream LLVM/Clang. It is actively developed and maintained by Leaning Technologies. Commercial support, feature fast tracking, sponsored development and consulting packages are available for Enterprise customers.
