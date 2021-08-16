# Tutorials overview

The tutorials are roughtly in order of complexity, but feel free to consume them starting where you are interested the most.
The main idea behind the tutorials is guiding you step by step through the specifics of compiling code to be run on the browser. They are designed to be educationals and easy to follow, and to show bite-sized best practices, not to push the limits of what's attainable.

### Hello, World! -> [link](Getting-Started#hello-world)
Here we cover the basics of invoking the Cheerp compiler and visualizing the results with nodejs or in the browser.

Input methods: none

Output methods: console.log()

Compilation mode: genericjs

Main takeway: everything is ready to get started

Task: print "Hello, World!"

### Hello, Wasm! -> [link](Tutorial-Hello-Wasm)
Here we compile an arbitrary C++ program to JavaScript and Wasm

Input methods: none

Output methods: console.log(), via std::cout<<

Compilation mode: genericjs and wasm

Main takeway: generic C++ program can be compiled with Cheerp, Wasm is great on a computational intensive task

Task: computing the number of primes up to N

### DOM Manipulation -> [link](Browser-side-programming-guide)
Here we modify the DOM of an html file

Input methods: DOM structure, text input, mouse position, clicks on buttons

Output methods: DOM structure and console.log()

Compilation mode: genericjs

Main takeway: webMain specifics and onLoad callback pattern, possibility to listen and respond to any DOM event with cheerp::Callback

Task: counting the number of elements in the DOM and modifying the DOM

### Pong: mixed mode JavaScript + Wasm -> [link](Cheerp-Tutorial%3A-Mixed-mode-C++-to-WebAssembly-and-JavaScript)
Here we draw on a Canvas, on the Wasm side, while taking care of the input in the JavaScript side

Input methods: key pressed

Output methods: canvas element

Compilation mode: genericjs + Wasm

Main takeway: event loop structure: input -> updating state -> drawing state

Task: a Pong replica from scratch

### Webworkers -> [link](Using-WebWorkers-with-Cheerp)
Here we see the basics of initializating a webWorker and some message passing

Input methods: none

Output methods: console.log()

Compilation mode: genericjs

Main takeway: WebWorkers basics

Task: basic message passing
