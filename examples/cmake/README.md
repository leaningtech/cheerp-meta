# Cheerp + CMake

[![Discord server](https://img.shields.io/discord/988743885121548329?color=%237289DA&logo=discord&logoColor=ffffff)](https://discord.gg/3UfTSbWdYy)

This is a minimal example of a CMake project that uses [Cheerp](https://labs.leaningtech.com/cheerp) to compile [main.cpp](/main.cpp) to WebAssembly and JavaScript.

## Building

Set the `CHEERP` environment variable to the path of your Cheerp installation. For example:
- On Linux: `export CHEERP=/opt/cheerp`
- On macOS: `export CHEERP=/Applications/cheerp`
- On Windows: `set CHEERP=C:\cheerp`

Then run the following commands to configure and build the project:

```bash
mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=$CHEERP/share/cmake/Modules/CheerpWasmToolchain.cmake ..
make
```

This will build `app.js` and `app.wasm` in the `build` directory.

## Running

### In the browser

Copy [index.html](/index.html) into the `build` directory, and serve it with your favourite web server.

For example:

```bash
cd build
cp ../index.html .
npx vite
```

### With Node.js

Because we only use the `console.log` function, we can run the WebAssembly module with Node.js.

```bash
cd build
node app.wasm
```
