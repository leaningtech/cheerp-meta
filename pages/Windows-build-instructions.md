The following instructions will guide you in building the lastest version of cheerp on Windows systems. Building the core compiler on Windows using Microsoft Visual Studio is fully supported, but building the base libraries (newlib, libc++, libc++abi) is not. You can grab a working copy from the latest release from any platform as they are platform independent.

# Requirements

* [Git](http://git-scm.com/download/win)
* [Python](https://www.python.org) (please select "Add Python to PATH" when asked)  
* Microsoft Visual Studio 2012, with the [latest update](http://www.microsoft.com/en-us/download/details.aspx?id=39305)
* [CMake](http://www.cmake.org)  

# Get the sources

Open a ```cmd``` shell and execute the following commands to download the source code for the Cheerp enabled llvm and clang repositories.

```cmd
git clone https://github.com/leaningtech/cheerp-compiler
```

# Generate Microsoft Visual Studio projects using CMake

Start the CMake GUI tool and point the source code directory to where you have cloned the Cheerp LLVM repository. Also set the binaries directory to the ```build``` subdirectory. Now press Configure

![](https://raw.githubusercontent.com/wiki/leaningtech/cheerp-wiki/cmake1.png)

Now select your version of Visual Studio, since some C++11 features are used you need VS2012 at least.

![](https://raw.githubusercontent.com/wiki/leaningtech/cheerp-wiki/cmake2.png)

Wait until the project is configured. Now you need to tick the Advanced box, set the ```LLVM_INSTALL_PREFIX``` variable to "C:\cheerp\" and press Configure again.

![](https://raw.githubusercontent.com/wiki/leaningtech/cheerp-wiki/cmake3.png)

Now press Generate

# Build Visual Studio project

Open the ```cheerp-llvm\build\LLVM.sln``` file.

![](https://raw.githubusercontent.com/wiki/leaningtech/cheerp-wiki/cmake5.png)

Make sure the ```ALL_BUILD``` project is selected and ```Release``` mode is enabled, then click ```Build Solution```

![](https://raw.githubusercontent.com/wiki/leaningtech/cheerp-wiki/cmake6.png)

Go grab some coffe and relax, it is going to take some time.

# Install

When the build is finished, select the ```INSTALL``` project in the ```CMakePredefinedTargets``` group and build it.

![](https://raw.githubusercontent.com/wiki/leaningtech/cheerp-wiki/cmake7.png)

Finally you need to make a copy of the ```c:\cheerp\bin\clang.exe``` and call it ```clang++.exe```
