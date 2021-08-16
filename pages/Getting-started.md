The purpose of this page is setting the basics of working with Cheerp, getting to the compilation of a "Hello World" example program and pointing to further reading and recommended tutorials.

This is the table of contents:
+ [Install](Getting-started#install "Install")
+ [Cheerp basics](Getting-started#cheerp-basics "Cheerp basics")
+ [Ready?](Getting-started#ready "Ready?")
+ [Hello, World!](Getting-started#hello-world "Hello, World!")
+ [Tutorials](Getting-started#tutorials "Tutorials")

Install
-----
Precompiled packages are available for [Ubuntu/Debian](Ubuntu-Debian-installation-using-PPA "Ubuntu/Debian apt packages"), [Windows](Windows-and-macOS-installation#windows "Windows binaries") or [macOS](Windows-and-macOS-installation#macos "macOS binaries").

Otherwise go to the [build instructions](Linux-build-instructions "Linux build instruction") to build directly from sources.

### Cheerp basics
+ Cheerp is command line compatible with clang++/g++ compilers (to be invoked by developers or build tools)

Linux/macOS: `/opt/cheerp/bin/clang++ -target cheerp file.cpp -O3 -o target.js`

Windows: `C:\cheerp\bin\clang++ -target cheerp file.cpp -O3 -o target.js`

Most arguments are exactly the same, while [cheerp specific options](Cheerp-specific-clang-options) are prefixed by `cheerp`.

+ Cheerp compiles from C/C++ sources to a mix of JavaScript, WebAssembly and ams.js

The first few examples will concentrate on pure JavaScript since it's conceptually easier, moving on to Wasm and asm.js later in the tutorials.
+ The generated files requires no postprocessing and can be put to use directly in a web page or called by any JavaScript engine.

Ready?
-----
Cheerp in itself has no dependencies, but the recommended workflow and the tutorials make use of:
+ a JavaScript engine
+ an Http server
+ a browser

The tutorials require very basic usage of a command line and use Unix notations, but it should be easy to translate to other environments.
You could check [Getting ready](Getting-ready) and come back here in a few minutes when everything is set.

Now we test if everything is set:
1. open the Terminal of your choice
2. `opt/cheerp/bin/clang++ --version` (`C:\cheerp\bin\clang++ --version` on Windows systems)
3. `nodejs --version`
4. `http-server -o`

All set? Go!

Hello, World!
-------
You are now ready for compiling your first Web application using Cheerp.
Move to a folder of your choice and save the following C++ program as ```hello.cpp```. ([link](tutorials/hello_world/hello.cpp))

```c++
// The cheerp/clientlib.h header contains declarations for the browser APIs
#include <cheerp/clientlib.h>

// webMain is the entry point for web applications written in Cheerp
void webMain()
{
        client::console.log("Hello, World Wide Web!");
}
```

You can then compile this program using the following command line:

```
/opt/cheerp/bin/clang++ -target cheerp hello.cpp -o hello.js
```

Great, you have compiled your first program with Cheerp. You can now run the generated JavaScript directly with
```
nodejs hello.js
```

You can also save this HTML file as ```hello.html``` ([link](tutorials/hello_world/hello.html)) 

```html
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <title>Cheerp example 0: hello</title>
    <script defer src="hello.js"></script>
  </head>
  <body>
  <h1 id="pagetitle">Open the console log
	(Ctrl + Shift + J or Ctrl + Option + J)
	to read the output</h1>
  </body>
</html>
```
and from the same folder ```firefox hello.html``` (or equivalent).

This is what the result should look like: [hello.html](tutorials/hello_world/hello.html)

[Why `webMain()` instead of `main()`?](Cheerp-basics#the-webmain-entry-point)

[What is `console.log()`?](Cheerp-basics#what-is-clientconsolelog)

It would have compiled also with `std::cout<< `, `printf()` or `int main()` (try it yourself), but we wanted to show first what zero-overhead access to the browser looks like.

Tutorials
-------
Now that you completed the first tutorial, we advise to going through the others to aquire some important knoledge:
+ [Hello, Wasm!](Tutorial-Hello-Wasm)
+ [DOM manipulation](Cheerp-Tutorial-DOM-Manipulation)
+ [your first WebAssembly program](WebAssembly-output-(wasm-or-wast-mode))
+ [a game of Pong in WebAssembly](Cheerp-Tutorial%3A-Mixed-mode-C++-to-WebAssembly-and-JavaScript).

We recommend also reading the [porting guidelines](Porting-guidelines), but there is plenty of interesting materials in this wiki to get back when you will want/need more, some of the most important topics to check out are:
+ [jsexport attribute](JavaScript-interoperability)
+ Compilation modes tradeoff [JavaScript / Wasm / asm.js](...)
+ [Manipulating the DOM](Browser-side-programming-guide)

You can also try Cheerp without installation thanks to this [Cheerp playground](https://cheerp.cppse.nl) by [Ray Burgemeestre](https://github.com/rayburgemeestre).

Bugs and Questions
------

You can get in touch with us using our [Gitter chat](https://gitter.im/leaningtech/cheerp), or by filing a bug on our [Issue tracker](https://github.com/leaningtech/cheerp-meta/issues).
