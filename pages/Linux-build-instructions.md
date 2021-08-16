---
title: Linux build instructions
---

Cheerp is composed of multiple components, they are somewhat interdependent and should be built together.

# Prerequisites

We assume that you have git, cmake, python and a modern C++ compiler properly installed.
Example, using apt-get:
```bash
apt-get install git cmake python gcc
```

# Get the sources

You need to get all the sources first. Please define the `CHEERP_SRC` environment variable that will be used in various commands.


```bash
mkdir cheerp
cd cheerp
export CHEERP_SRC=$PWD
git clone https://github.com/leaningtech/cheerp-compiler
git clone https://github.com/leaningtech/cheerp-utils
git clone https://github.com/leaningtech/cheerp-newlib
git clone https://github.com/leaningtech/cheerp-libs
```
# Build Cheerp LLVM/clang based compiler

```bash
cd $CHEERP_SRC/cheerp-compiler
mkdir build
cd build
cmake -C ../llvm/CheerpCmakeConf.cmake -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_PROJECTS=clang -G Ninja ../llvm/
ninja -j4
ninja install
```
# Build Cheerp Utilities

```bash
cd $CHEERP_SRC/cheerp-utils
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/opt/cheerp ..
make install
```

# Build Cheerp C library (newlib)

```bash
cd $CHEERP_SRC/cheerp-newlib/newlib
mkdir build
cd build
../configure --host=cheerp-genericjs --with-cxx-headers=$CHEERP_SRC/cheerp-compiler/libcxx/include --prefix=/opt/cheerp --enable-newlib-io-long-long --enable-newlib-iconv --enable-newlib-iconv-encodings=utf-16,utf-8,ucs_2 --enable-newlib-mb --enable-newlib-nano-formatted-io
make
make install
CHEERP_PREFIX=/opt/cheerp ../build-bc-libs.sh genericjs
```

# Build Cheerp C++ library

```bash
cd $CHEERP_SRC/cheerp-compiler/libcxx
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/opt/cheerp -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=/opt/cheerp/share/cmake/Modules/CheerpToolchain.cmake -DLIBCXX_ENABLE_SHARED=OFF -DLIBCXX_ENABLE_ASSERTIONS=OFF -DLIBCXX_CXX_ABI_INCLUDE_PATHS=$CHEERP_SRC/cheerp-compiler/libcxxabi/include -DLIBCXX_CXX_ABI=libcxxabi -DCMAKE_CXX_FLAGS="-fexceptions" ..
make
make install
```
# Build Cheerp C++ ABI library

```bash
cd $CHEERP_SRC/cheerp-compiler/libcxxabi
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/opt/cheerp -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=/opt/cheerp/share/cmake/Modules/CheerpToolchain.cmake -DLIBCXXABI_ENABLE_SHARED=OFF -DLIBCXXABI_ENABLE_ASSERTIONS=OFF -DLIBCXXABI_LIBCXX_PATH=$CHEERP_SRC/cheerp-compiler/cheerp-libcxx/ -DLIBCXXABI_LIBCXX_INCLUDES=$CHEERP_SRC/cheerp-compiler/libcxx/include -DLIBCXXABI_ENABLE_THREADS=0 -DLLVM_CONFIG=/opt/cheerp/bin/llvm-config ..
make
make install
```

# Build Cheerp libraries

```bash
cd $CHEERP_SRC/cheerp-libs
```

## Cheerp GLES implementation

```bash
make -C webgles install INSTALL_PREFIX=/opt/cheerp CHEERP_PREFIX=/opt/cheerp
```

## Cheerp helper library for Wasm

```bash
make -C wasm install INSTALL_PREFIX=/opt/cheerp CHEERP_PREFIX=/opt/cheerp
```

## Cheerp optimized standard libraries

```bash
make -C stdlibs install_genericjs INSTALL_PREFIX=/opt/cheerp CHEERP_PREFIX=/opt/cheerp
```

# Build asm.js/wasm version of standard libraries

## Build Cheerp C library (newlib)

```bash
cd $CHEERP_SRC/cheerp-newlib/newlib
mkdir build_asmjs
cd build_asmjs
../configure --host=cheerp-asmjs --with-cxx-headers=$CHEERP_SRC/cheerp-compiler/libcxx/include --prefix=/opt/cheerp --enable-newlib-io-long-long --enable-newlib-iconv --enable-newlib-iconv-encodings=utf-16,utf-8,ucs_2 --enable-newlib-mb --enable-newlib-nano-formatted-io
make
make install
CHEERP_PREFIX=/opt/cheerp ../build-bc-libs.sh asmjs
```

## Build Cheerp C++ library

```bash
cd $CHEERP_SRC/cheerp-compiler/libcxx
mkdir build_asmjs
cd build_asmjs
cmake -DCMAKE_INSTALL_PREFIX=/opt/cheerp -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=/opt/cheerp/share/cmake/Modules/CheerpWasmToolchain.cmake -DLIBCXX_ENABLE_SHARED=OFF -DLIBCXX_ENABLE_ASSERTIONS=OFF -DLIBCXX_CXX_ABI_INCLUDE_PATHS=$CHEERP_SRC/cheerp-compiler/libcxxabi/include -DLIBCXX_CXX_ABI=libcxxabi -DCMAKE_CXX_FLAGS="-fexceptions" ..
make
make install
```
## Build Cheerp C++ ABI library

```bash
cd $CHEERP_SRC/cheerp-compiler/libcxxabi
mkdir build_asmjs
cd build_asmjs
cmake -DCMAKE_INSTALL_PREFIX=/opt/cheerp -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=/opt/cheerp/share/cmake/Modules/CheerpWasmToolchain.cmake -DLIBCXXABI_ENABLE_SHARED=OFF -DLIBCXXABI_ENABLE_ASSERTIONS=OFF -DLIBCXXABI_LIBCXX_PATH=$CHEERP_SRC/cheerp-compiler/libcxx/ -DLIBCXXABI_LIBCXX_INCLUDES=$CHEERP_SRC/cheerp-compiler/libcxx/include -DLIBCXXABI_ENABLE_THREADS=0 -DLLVM_CONFIG=/opt/cheerp/bin/llvm-config ..
make
make install
```

## Build Cheerp libraries

```bash
cd $CHEERP_SRC/cheerp-libs
```

### Cheerp optimized standard libraries

```bash
make -C stdlibs install_asmjs INSTALL_PREFIX=/opt/cheerp CHEERP_PREFIX=/opt/cheerp
```

# FAQ

```
/opt/cheerp/bin/../include/client/cheerp/types.h:24:10: fatal error: 'utility' file not found
#include <utility>
         ^~~~~~~~~
```

This means that make cannot find the libcxx library. Please check the path of the variable LIBCXXABI_LIBCXX_PATH of cmake
