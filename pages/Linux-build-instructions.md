---
title: Linux build instructions
---

Cheerp is composed of multiple components, they are somewhat interdependent and should be built together.
The build instructions are provided for the stable release, Cheerp 2.7, or for the latest / development branch.

[Scroll to bottom](https://docs.leaningtech.com/cheerp/Linux-build-instructions.html#build-latest-version) for the instruction for the latest version, or keep reading to build stable.


## Prerequisites

We assume that you have git, cmake, python and a modern C++ compiler properly installed.
Example, using apt-get:
```bash
apt-get install cmake python3 python3-distutils ninja-build gcc lld git
```


## Build stable version, Cheerp 2.7

### Get the sources, stable version

You need to get all the sources first. Please define the `CHEERP_SRC` environment variable that will be used in various commands.

```bash
mkdir cheerp
cd cheerp
export CHEERP_SRC=$PWD
git clone --branch cheerp-2.7 https://github.com/leaningtech/cheerp-compiler
git clone --branch cheerp-2.7 https://github.com/leaningtech/cheerp-utils
git clone --branch cheerp-2.7 https://github.com/leaningtech/cheerp-newlib
git clone --branch cheerp-2.7 https://github.com/leaningtech/cheerp-libs
```

### Build Cheerp/1: compiler, stable version

```bash
cd cheerp-compiler
cmake -S llvm -B build -C llvm/CheerpCmakeConf.cmake -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_PROJECTS=clang -G Ninja
ninja -C build -j4
ninja -C build install
cd ..
```

By default Cheerp will be installed in `/opt/cheerp`, with the main executable at `/opt/cheerp/bin/clang++`.
If you need write privileges to `/opt/cheerp`, then prepend all install commands with `sudo`.


### Build Cheerp/2: utilities and libraries, stable version

```bash
cd cheerp-utils
cmake -B build -DCMAKE_INSTALL_PREFIX=/opt/cheerp .
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

Now you should have a working Cheerp installation, to test it, follow [here](https://docs.leaningtech.com/cheerp/Linux-build-instructions#hello-world-in-cheerp).


## Build latest version
This allows to benefit from the latest develpments and bug fixes, but we reserve the possibility of forward-incompatible changes.

### Get the sources, latest version

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

### Build Cheerp/1: compiler, latest version

```bash
cd cheerp-compiler
cmake -S llvm -B build -C llvm/CheerpCmakeConf.cmake -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_PROJECTS=clang -G Ninja
ninja -C build -j4
ninja -C build install
cd ..
```

By default Cheerp will be installed in `/opt/cheerp`, with the main executable at `/opt/cheerp/bin/clang++`.
If you need write privileges to `/opt/cheerp`, then prepend all install commands with `sudo`.


### Build Cheerp/2: utilities and libraries, latest version

```bash
cd cheerp-utils
cmake -B build -DCMAKE_INSTALL_PREFIX=/opt/cheerp .
make -C build install
cd ..

cd cheerp-musl
mkdir build_genericjs
cd build_genericjs
RANLIB="/opt/cheerp/bin/llvm-ar s" AR="/opt/cheerp/bin/llvm-ar"  CC="/opt/cheerp/bin/clang -target cheerp -I /opt/cheerp/lib/clang/15.0.0/include" LD="/opt/cheerp/bin/llvm-link" CPPFLAGS="" ../configure --target=cheerp --disable-shared --prefix=/opt/cheerp --with-malloc=oldmalloc
make clean
make -j8
make install
cd ..
mkdir build_asmjs
cd build_asmjs
RANLIB="/opt/cheerp/bin/llvm-ar s" AR="/opt/cheerp/bin/llvm-ar"  CC="/opt/cheerp/bin/clang -target cheerp-wasm -I /opt/cheerp/lib/clang/15.0.0/include" LD="/opt/cheerp/bin/llvm-link" CPPFLAGS="" ../configure --target=cheerp-wasm --disable-shared --prefix=/opt/cheerp --with-malloc=oldmalloc
make clean
make -j8
make install
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
cd system
cmake -B build_genericjs -DCMAKE_INSTALL_PREFIX=/opt/cheerp -DCMAKE_TOOLCHAIN_FILE=/opt/cheerp/share/cmake/Modules/CheerpToolchain.cmake .
cmake --build build_genericjs
cmake --install build_genericjs
cmake -B build_asmjs -DCMAKE_INSTALL_PREFIX=/opt/cheerp -DCMAKE_TOOLCHAIN_FILE=/opt/cheerp/share/cmake/Modules/CheerpWasmToolchain.cmake .
cmake --build build_asmjs
cmake --install build_asmjs
cd ../..
```


## "Hello, World" in Cheerp

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



## Cheerp unit tests

```bash
cd cheerp-utils/tests
python run-tests.py /opt/cheerp/bin/clang++ node --all -j8
```

This command will compile and execute the Cheerp test suite.
