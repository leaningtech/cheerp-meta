<h1><img src="assets/logotype.svg" alt="Cheerp" height="96"></h1>

[![Discord server](https://img.shields.io/discord/988743885121548329?color=%237289DA&logo=discord&logoColor=ffffff)](https://discord.leaningtech.com)
[![GitHub Issues](https://img.shields.io/github/issues/leaningtech/cheerp-meta.svg)](https://github.com/leaningtech/cheerp-meta/issues)
[![Latest version](https://img.shields.io/badge/cheerp-3.0-brightgreen.svg)](https://leaningtech.com/cheerp/#download)

Cheerp is an open-source, permissively licensed, enterprise-grade C/C++ compiler for Web applications. It allows to compile virtually any C/C++ code to WebAssembly and JavaScript.

Download the latest version of Cheerp: [![Latest version](https://img.shields.io/badge/cheerp-3.0-brightgreen.svg)](https://leaningtech.com/cheerp/#download) [![Latest version changelog](https://img.shields.io/badge/Changelog-3.0-brightgreen.svg)](Changelog)

Main project link: [https://leaningtech.com/cheerp/](https://leaningtech.com/cheerp/)

This repository is the main reference for documentation, issue tracking, and release notes of Cheerp. The main component of Cheerp, i.e. the Cheerp compiler, can be found on Github: [cheerp-compiler](https://github.com/leaningtech/cheerp-compiler), while other repositories are at [cheerp-musl](https://github.com/leaningtech/cheerp-musl), [cheerp-utils](https://github.com/leaningtech/cheerp-utils) and [cheerp-libs](https://github.com/leaningtech/cheerp-libs)

## What is Cheerp

Cheerp is a C/C++ compiler to WebAssembly and JavaScript, based on and integrated into the LLVM/Clang infrastructure, and featuring numerous custom optimisation steps to maximise performance and minimise size of the compiled JavaScript/WebAssembly output. Thanks to this, Cheerp is the best performing, most optimised C++ to WebAssembly compiler available on the market.

Cheerp is used primarily to port existing C/C++ libraries and applications to HTML5, but can also be used to write high-performance Web applications and WebAssembly components from scratch. Using Cheerp, C/C++ code gets compiled into JavaScript/WebAssembly and optimised, with the right JavaScript interfaces being exposed, and can easily be integrated in a web application.

Cheerp is licensed under the "Apache 2.0 License with LLVM exceptions", i.e. the same license of upstream LLVM/Clang. This is a permissive license that allows virtually any use of the software.

![Diagram of Cheep usage](assets/Diagram_browser.png "Icons from https://github.com/ubuntu/yaru/tree/master/icons, Creative Commons BY-SA 4.0")

## What is unique about Cheerp?

Cheerp enables C/C++ code to be compiled to a combination of WebAssembly and JavaScript (or asm.js when targeting legacy browsers). Compared to alternative C/C++ compilers for web applications such as Emscripten, Cheerp:

1. Generates more optimized (smaller) WebAssembly code, but can also compile to a JavaScript output with dynamic memory (garbage-collectible output), zero-overhead DOM manipulation and access to Web APIs and superior C++-JavaScript interoperability.
2. Allows to compile a single code base into a combination of WebAssembly and JavaScript by (optionally) tagging portions of the code with `[[cheerp::genericjs]]` and `[[cheerp::wasm]]`.

Cheerp is free open-source software, actively developed and maintained by Leaning Technologies. Commercial support, feature fast tracking, sponsored developoment and consulting packages are available for Enterprise customers.

## Get started

First time user? We recommend following the [Getting started](https://docs.leaningtech.com/cheerp/Getting-started#install) page, that will guide though these steps:

- [Install](https://docs.leaningtech.com/cheerp/Getting-started#install "Install")
- [Cheerp basics](https://docs.leaningtech.com/cheerp/Getting-started#cheerp-basics "Cheerp basics")
- [Ready?](https://docs.leaningtech.com/cheerp/Getting-started#ready "Ready?")
- [Hello, World!](https://docs.leaningtech.com/cheerp/Getting-started#hello-world "Hello, World!")
- [Tutorials](https://docs.leaningtech.com/cheerp/Getting-started#tutorials "Tutorials")

Or look at the sidebar to find lots of useful materials

## Bugs and Questions

You can get in touch with us using [Discord](https://discord.leaningtech.com), or by filing a bug on our [Issue tracker](https://github.com/leaningtech/cheerp-meta/issues).
