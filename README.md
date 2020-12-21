<p align="center"><img src="https://github.com/leaningtech/cheerp-meta/blob/master/media/cheerp_light_background.png" width="300"></p>

# Cheerp - a C/C++ compiler for Web applications

[![Gitter chat](https://badges.gitter.im/leaningtech/cheerp.svg)](https://gitter.im/leaningtech/cheerp)
[![GitHub Issues](https://img.shields.io/github/issues/leaningtech/cheerp-meta.svg)](https://github.com/leaningtech/cheerp-meta/issues)
[![Latest version](https://img.shields.io/badge/cheerp-2.6-brightgreen.svg)](https://leaningtech.com/pages/cheerp.html#Install)  [![Latest version changelog](https://img.shields.io/badge/Changelog-2.6-brightgreen.svg)](https://github.com/leaningtech/cheerp-meta/wiki/Changelog)

Cheerp is an open-source, commercial C/C++ compiler for Web applications. It allows to compile virtually any C/C++ code (up to C++17) to WebAssembly, JavaScript, asm.js or a combination thereof.

Download latest version: [![Latest version](https://img.shields.io/badge/cheerp-2.6-brightgreen.svg)](https://leaningtech.com/cheerp/download/)  [![Latest version changelog](https://img.shields.io/badge/Changelog-2.6-brightgreen.svg)](https://github.com/leaningtech/cheerp-meta/wiki/Changelog)

Main documentation link: https://github.com/leaningtech/cheerp-meta/wiki

Main project link: https://leaningtech.com/pages/cheerp.html

This repository is the main reference for documentation, issue tracking, and release notes.

What is Cheerp
-----

Cheerp is a C/C++ compiler for the Web, based and integrated into the LLVM/clang infrastructure, and featuring numerous custom optimisation steps to maximise performance and minimise size of the compiled JavaScript/WebAssembly output. Thanks to this, Cheerp is the best performing, most optimised C++ to WebAssembly compiler available on the market

Cheerp is used primarily to port existing C/C++ libraries and applications to HTML5, but can also be used to write high-performance Web applications and WebAssembly components from scratch. C/C++ gets optimised and compiled into JavaScript/WebAssembly, and can easily be integrated in a web page.

![Diagram of Cheep usage](https://raw.githubusercontent.com/wiki/leaningtech/cheerp-meta/Diagram_browser.png "Icons from https://github.com/ubuntu/yaru/tree/master/icons, Creative Commons BY-SA 4.0")

What is unique about Cheerp?
------

Cheerp enables C/C++ code to be compiled to WebAssembly, JavaScript, asm.js or a combination thereof. Compared to alternative C/C++ compilers for web applications such as Emscripten, Cheerp:

1. Generates faster WebAssembly code, but can also compile to a JavaScript output with dynamic memory (garbage-collectible output), zero-overhead DOM manipulation and access to WebApis and superior C++-JavaScript interoperability.
2. Allows to compile a single code base into a combination of WebAssembly and JavaScript by (optionally) tagging portions of the code with ```[[cheerp::genericjs]]``` and ```[[cheerp::wasm]]```.
3. Generates smaller WebAssembly outputs.

Cheerp is open-source software and is free to use for GPLv2 projects. Non-copyleft commercial licenses, commercial support and consulting packages are available from Leaning Technologies.

Get started
------

First time user? We recommend following the [Getting started](https://github.com/leaningtech/cheerp-meta/wiki/Getting-started) page, that will guide though this steps:
+ [Install](https://github.com/leaningtech/cheerp-meta/wiki/Getting-started#install "Install")
+ [Cheerp basics](https://github.com/leaningtech/cheerp-meta/wiki/Getting-started#cheerp-basics "Cheerp basics")
+ [Ready?](https://github.com/leaningtech/cheerp-meta/wiki/Getting-started#ready "Ready?")
+ [Hello, World!](https://github.com/leaningtech/cheerp-meta/wiki/Getting-started#hello-world "Hello, World!")
+ [Tutorials](https://github.com/leaningtech/cheerp-meta/wiki/Getting-started#tutorials "Tutorials")

Or look at the sidebar to find lots of useful materials

Bugs and Questions
------

You can get in touch with us using our [Gitter chat](https://gitter.im/leaningtech/cheerp), or by filing a bug on our [Issue tracker](https://github.com/leaningtech/cheerp-meta/issues).

Documentation
-----

Documentation is available at https://github.com/leaningtech/cheerp-meta/wiki.
