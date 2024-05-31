# 'Hello, world' with Cheerp

This example shows how to compile [hello.cpp](hello.cpp) into JavaScript and WebAssembly using Cheerp.

## Build

```sh
$ make
```

This will produce `hello.js` and `hello.wasm` files as output.

## Usage (Node.js)

You can run `hello.js` from your favourite JavaScript runtime, such as Node.js:

```
$ node hello.js
Hello, world!
```

## Usage (Browser)

Host `hello.js`, `hello.wasm`, and `index.html` on any web server, then open the page in your browser.

```
npx http-server
```

> [!INFO]
> An HTTP server is required because it is against browser security policies to load WebAssembly from a the `file://` URL scheme.

