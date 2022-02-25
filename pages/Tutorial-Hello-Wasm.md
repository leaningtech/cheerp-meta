---
title: Tutorial - Hello, Wasm!
---

This tutorial will cover the basics of using Cheerp to compile a generic C++ source file to JavaScript and JavaScript+WebAssembly.

We will show what Wasm is expecially good at: doing the computational heavy lifting.

We haven't covered yet any input methods (that's for the next tutorial), and we will keep using the most basic output method (writing to the console).

I picked a computational heavy task: "Counting how many primes are smaller than X", I choosed a general and clear implementation written in modern C++ of the segmented version of the [sieve of Eratosthenes](https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes) by [Kim Walisch](https://github.com/kimwalisch/primesieve/wiki/Segmented-sieve-of-Eratosthenes). There are a bunch of standard C++ library features used, `std::cout` output, `std::chrono`, `std::vector`, and we will see what happens while compiling it.

Native compiling
------

First, save [segmented_sieve.cpp](tutorials/hello_wasm/segmented_sieve.cpp) on your computer and try to compile it natively:

`g++ segmented_sieve.cpp -o segmented_sieve -O3`
(or `clang++` or equivalent command line C++ compiler)

and then we run it:
`./segmented_sieve`

the output to the console will be something like:
```
4.0052e-05s	to sieve in the interval (1, 10)	4 primes found
5.8115e-05s	to sieve in the interval (1, 100)	25 primes found
2.2484e-05s	to sieve in the interval (1, 1000)	168 primes found
3.3346e-05s	to sieve in the interval (1, 10000)	1229 primes found
0.000181305s	to sieve in the interval (1, 100000)	9592 primes found
0.00166905s	to sieve in the interval (1, 1e+06)	78498 primes found
0.0170393s	to sieve in the interval (1, 1e+07)	664579 primes found
0.103364s	to sieve in the interval (1, 1e+08)	5761455 primes found
0.934908s	to sieve in the interval (1, 1e+09)	50847534 primes found
16.2481s	to sieve in the interval (1, 1e+10)	455052511 primes found
```

Nice, the program works, we can give it a go with Cheerp.


Cheerp JavaScript compiling
-------

<code><b>/opt/cheerp/bin/clang++ -target cheerp</b> segmented_sieve.cpp -o segmented_sieve.js -O3</code>

and then we run it:
`nodejs segmented_sieve.js`

the output to the console will be something like:
```
0.006s	to sieve in the interval (1, 10)	4 primes found
0.007s	to sieve in the interval (1, 100)	25 primes found
0.008s	to sieve in the interval (1, 1000)	168 primes found
0.007s	to sieve in the interval (1, 10000)	1229 primes found
0.044s	to sieve in the interval (1, 100000)	9592 primes found
0.014s	to sieve in the interval (1, 1e+06)	78498 primes found
0.06s	to sieve in the interval (1, 1e+07)	664579 primes found
0.456s	to sieve in the interval (1, 1e+08)	5761455 primes found
4.974s	to sieve in the interval (1, 1e+09)	50847534 primes found
```

It works, internally it does equivalents calculations. There is a performance slowdown (circa 5x), but actually the relevant and non obvious thing is that automatically, without having to touch/understand the code, a C++ program has been compiled to be executable inside a browser or a JavaScript engine.

Want to see it inside a browser?

```html
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <title>Cheerp example 1: hello Wasm</title>
    <script defer src="segmented_sieve.js"></script>
  </head>
  <body>
  <h1 id="pagetitle">Open the console log
	(Ctrl + Shift + J or Ctrl + Option + J)
	to read the output</h1>
  </body>
</html>
```

Save this [html file](tutorials/hello_wasm/segmented_sieve.html) in the same folder as `segmented_sieve.js`, and open it in your browser of choice and check the console log.

Enter WebAssembly / Wasm
------

Now we will get to the serious stuff, compiling to a mix of JavaScript (that will take care of managing/allocating memory and forwarding input and output) and Wasm.

The command line it's basically the same, just changing the target:

<code>/opt/cheerp/bin/clang++ -target cheerp-wasm segmented_sieve.cpp -o segmented_sieve_wasm.js -O3</code>

Note that we are using the **cheerp-wasm** target, not the **cheerp** target. This marks all code to be compiled into wasm (or asmjs) by default, including the C and C++ standard libraries.

Now we just have to run it:
`nodejs segmented_sieve_wasm.js`

The main file to be invoked is still a `.js` file, but a `.wasm` file is also produced, and it is loaded and run from the `.js` one.

Now the result, on my machine, is something like this:
```
0.001s	to sieve in the interval (1, 10)	4 primes found
0s	to sieve in the interval (1, 100)	25 primes found
0s	to sieve in the interval (1, 1000)	168 primes found
0.001s	to sieve in the interval (1, 10000)	1229 primes found
0.002s	to sieve in the interval (1, 100000)	9592 primes found
0.016s	to sieve in the interval (1, 1e+06)	78498 primes found
0.094s	to sieve in the interval (1, 1e+07)	664579 primes found
0.156s	to sieve in the interval (1, 1e+08)	5761455 primes found
1.868s	to sieve in the interval (1, 1e+09)	50847534 primes found
```

Things of note:
+ there is roughly a 3x speedup between the pure JavaScript version and the JavaScript + Wasm
+ JavaScript + Wasm trails natively compiled C++ by 2x here.

Is 2x good, bad, or what?
Here is not very relevant, I would say, since the main point of this page is showing that it should be possible to compile (most) C++ code to be run sandboxed in the browser.

Http-server for executing on the browser
------

Want to see it in your browser?

Take the previous html file, and change `segmented_sieve.js` to `segmented_sieve_loader.js`. You can not open it directly on your browser, since the JavaScript loader will need a way for loading the wasm file, and allowing arbitrary loading of a file is an unsafe operation that most browsers will not allow.

What then? In the [getting ready](Getting-ready#an-http-server) section we covered how to install a web server, now we will need to run it like:

`http-server -o`

This will open a new tab on your favorite browser with a list of the files in the current folder. Choose `segmented_sieve.html` (or whatever name you gave to the file), way few second for the execution and open the console. You should be able to see similar results to the one computed via `nodejs`.


Conclusions
------

We compiled a C++ file to pure JavaScript and to Wasm loaded by a JavaScript. Great.

I suggest trying compiling your own pet projects, playing modifying the source code or going forwards with the tutorials. Up next: DOM manipulation.
