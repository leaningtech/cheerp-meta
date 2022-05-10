---
title: Linux build instructions
---

Cheerp is composed of multiple components, they are somewhat interdependent and should be built together.

# Prerequisites

We assume that you have git, cmake, python and a modern C++ compiler properly installed.
Example, using apt-get:
```bash
apt-get install cmake python3 python3-distutils ninja-build gcc lld git
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

# Build Cheerp compiler

```bash
cd cheerp-compiler
cmake -S llvm -B build -C llvm/CheerpCmakeConf.cmake -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_PROJECTS=clang -G Ninja
ninja -C build -j4
ninja -C build install
cd ..
```

By default Cheerp will be installed in `/opt/cheerp`, with the main executable at `/opt/cheerp/bin/clang++`.
If you need write privileges to `/opt/cheerp`, then prepend all install commands with `sudo`.


# Build utilities and libraries

```bash
cd cheerp-utils
cmake -B build -DCMAKE_INSTALL_PREFIX=/opt/cheerp ..
make -C build install
cd ..

cd cheerp-newlib/newlib
mkdir build_genericjs
cd build_genericjs
../configure --host=cheerp-genericjs --with-cxx-headers=$PWD/../../cheerp-compiler/libcxx/include --prefix=/opt/cheerp --enable-newlib-io-long-long --enable-newlib-iconv --enable-newlib-iconv-encodings=utf-16,utf-8,ucs_2 --enable-newlib-mb --enable-newlib-nano-formatted-io AR=/opt/cheerp/bin/llvm-ar RANLIB="/opt/cheerp/bin/llvm-ar s"
make
make install
../build-bc-libs.sh genericjs
cd ..
mkdir build_asmjs
cd build_asmjs
../configure --host=cheerp-asmjs --with-cxx-headers=$PWD/../../cheerp-compiler/libcxx/include --prefix=/opt/cheerp --enable-newlib-io-long-long --enable-newlib-iconv --enable-newlib-iconv-encodings=utf-16,utf-8,ucs_2 --enable-newlib-mb --enable-newlib-nano-formatted-io AR=/opt/cheerp/bin/llvm-ar RANLIB="/opt/cheerp/bin/llvm-ar s"
make
make install
../build-bc-libs.sh asmjs
cd ..
cd ../..

cd cheerp-compiler
cmake -S runtimes -B build_runtimes_genericjs -GNinja -C runtimes/CheerpCmakeConf.cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE="/opt/cheerp/share/cmake/Modules/CheerpToolchain.cmake"
ninja -C build_runtimes_genericjs
ninja -C build_runtimes_genericjs install

cmake -S runtimes -B build_runtimes_wasm -GNinja -C runtimes/CheerpCmakeConf.cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE="/opt/cheerp/share/cmake/Modules/CheerpWasmToolchain.cmake"
ninja -C build_runtimes_wasm
ninja -C build_runtimes_wasm install
cd ..

cd cheerp-libs
make -C webgles install INSTALL_PREFIX=/opt/cheerp CHEERP_PREFIX=/opt/cheerp
make -C wasm install INSTALL_PREFIX=/opt/cheerp CHEERP_PREFIX=/opt/cheerp
make -C stdlibs install INSTALL_PREFIX=/opt/cheerp CHEERP_PREFIX=/opt/cheerp
cd ..
```

# "Hello, World" in Cheerp

```c++
//save as example.cpp
#include <iostream>

int main()
{
    std::cout << "Hello, World!\n";
    return 0;
}
```

```/opt/cheerp/bin/clang++ example.cpp -o cheerp_example.js -O3 && node cheerp_example.js```
Should compile and execute the relevant file.



# Cheerp unit tests

```bash
cd cheerp-utils/tests
python run-tests.py /opt/cheerp/bin/clang++ node --all -j8
```

This command will compile and execute the Cheerp test suite.


