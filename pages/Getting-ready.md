---
title: Getting ready
---

Cheerp in itself has no dependencies, but the recommended workflow and the tutorials make use of: 
+ a command line prompt
+ a JavaScript engine
+ an Http server
+ a browser

How to have everything up and running (useful not just for Cheerp but also generally) in your developer environment of choice?

## Command line prompt
On Linux or Mac OS X, open the Terminal of your choice. On Windows, you can use the command prompt (Start - type 'cmd') or PowerShell (Start - type 'powershell').

For the purpose of the tutorials, the main cosmetic difference between running in a Unix command line and a Windows command line is the `\` vs `/`. Use the one your system impose.

## A JavaScript engine
We will use node.js for simplicity, but Cheerp is completely independent and strive to work with a range as wide as reasonable of JavaScript implementations

#### Node.js installation instructions
Linux users could either install from [source](https://nodejs.org/en/download/) or [use a packet manager for their distribution](https://nodejs.org/en/download/package-manager/).

Windows and macOS users can direcly download from [the official website](https://nodejs.org/en/download).

Whatever the case running `nodejs --version` from the command line should confirm whether it's installed properly or not.

## An http server
Since you have already Nodejs (right?), using Nodejs packet manager `npm` it's easy to install the appropiately named `http-server` package.
Visit [its website](https://www.npmjs.com/package/http-server), and then install it with:

`npm install http-server -g`

And then test it with:
`http-server -o`

Other http servers will also work, just change http-server to invoke your preferred solution.

## A browser
[Firefox](https://www.mozilla.org/en-US/firefox/) or any browser from the [Chrome](https://www.google.com/chrome/)-[family](https://www.chromium.org/) offer good support of the JavaScript and Wasm standards and great development tools (to inspect the code, trace memory usage or debugging bugs). They shine in different things, and are available in any environment.
But any web-browser that comes with your system will do.

Knowing how to launch it from command line could be useful, for example ```firefox file.html``` opens `file.html` in a new tab.

## Optional: a C++ command line compiler
This is not strictly needed but it could be interesting to compare Cheerp compilation results with native compilations and experiment a bit.

## Done
Great, everything is set, now back to the [Tutorials](https://github.com/leaningtech/cheerp-meta/wiki#tutorials).
