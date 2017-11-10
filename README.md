<p align="center"><img src="https://github.com/leaningtech/cheerp-meta/blob/master/media/cheerp_light_background.png" width="300"></p>

# Cheerp - a C/C++ compiler for Web applications

[![Gitter chat](https://badges.gitter.im/leaningtech/cheerp.svg)](https://gitter.im/leaningtech/cheerp)
[![GitHub Issues](https://img.shields.io/github/issues/leaningtech/cheerp-meta.svg)](https://github.com/leaningtech/cheerp-meta/issues)
[![Latest version](https://img.shields.io/badge/cheerp-2.0rc1-brightgreen.svg)](https://leaningtech.com/cheerp/download/)  [![Latest version changelog](https://img.shields.io/badge/cheerp2.0rc1-changelog-brightgreen.svg)](https://github.com/leaningtech/cheerp-meta/wiki/Changelog)


Cheerp is an open-source, commercial C/C++ compiler for Web applications. It allows to compile virtually any C/C++ code (up to C++14) to WebAssembly, JavaScript, asm.js or a combination thereof.

Download latest version: [![Latest version](https://img.shields.io/badge/cheerp-2.0rc1-brightgreen.svg)](https://leaningtech.com/cheerp/download/)  [![Latest version changelog](https://img.shields.io/badge/cheerp2.0rc1-changelog-brightgreen.svg)](https://github.com/leaningtech/cheerp-meta/wiki/Changelog)


Main documentation link: https://github.com/leaningtech/cheerp-meta/wiki

Main project link: https://leaningtech.com/cheerp/

This repository is the main reference for documentation, issue tracking, and release notes. Please contribute to Cheerp!

What is Cheerp?
------

Cheerp is a C/C++ compiler for the Web, based and integrated into the LLVM/clang infrastructure, and featuring numerous custom optimisation steps to maximise performance and minimise size of the compiled JavaScript/WebAssembly output.

Cheerp is used mainly to port existing C/C++ libraries and applications to HTML5, but can also be used to write high-performance Web applications and components from scratch. C/C++ gets optimised and compiled into JavaScript/WebAssembly, and can easily be deployed as part of a web page.

What is unique about Cheerp?
------

Cheerp enables C/C++ code to be compiled to WebAssembly, JavaScript, asm.js or a combination thereof. Compared to alternative C/C++ compilers for web applications such as Emscripten, Cheerp:
1. Achieves the same near-native runtime performance when compiling to WebAssembly, but can also compile to a JavaScript output with dynamic memory (garbage-collectible output), zero-overhead DOM manipulation and access to WebApis and superior C++-JavaScript interoperability.
2. Allows to compile a single code base into a combination of WebAssembly and JavaScript by (optionally) tagging portions of the code with ```[[cheerp::genericjs]]``` and ```[[cheerp::wasm]]```.
3. Can generate code 30-90% smaller than Emscripten, with faster startup.

Cheerp is open-source software and is free for non-commercial uses. Paid support and consulting packages are available for commercial users. 

Getting Started
------
You can download the latest official release of Cheerp for Windows, Linux and macOS a this address:
https://leaningtech.com/cheerp/download/. Installation instructions are available [here](https://github.com/leaningtech/cheerp-meta/wiki#buildinstallation).

Alternatively, you can build Cheerp from source. Please follow the instructions available [here](https://github.com/leaningtech/cheerp-meta/wiki#buildinstallation).

To get started, you can have a look at our simple tutorial to build your first JavaScript [Hello World](https://github.com/leaningtech/cheerp-meta/wiki/Cheerp-Tutorial), or your first WebAssembly [program](https://github.com/leaningtech/cheerp-meta/wiki/WebAssembly-output-(wasm-or-wast-mode)).

Bugs and Questions
------

You can get in touch with us using our [Gitter chat](https://gitter.im/leaningtech/cheerp), or by filing a bug on our [Issue tracker](https://github.com/leaningtech/cheerp-meta/issues).

Documentation
-----

Documentation is available at https://github.com/leaningtech/cheerp-meta/wiki.
