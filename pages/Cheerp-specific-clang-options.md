---
title: Cheerp specific clang options
---

  `-cheerp-asmjs-mem-file=<file>`       Write asm.js initialized memory to `<file>`

  `-cheerp-asmjs-symbolic-globals`      Compile global variable addresses as js variables in the asm.js module

  `-cheerp-avoid-wasm-traps`            Avoid traps from WebAssembly by generating more verbose code

  `-cheerp-bounds-check`                Generate debug code for bounds-checking array and object members accesses

  `-cheerp-dump-bc`                     Output the final BC file

  `-cheerp-fix-wrong-func-casts`        Generate wrappers for functions casted to types with more arguments

  `-cheerp-force-typed-arrays`          Use typed arrays instead of normal arrays for list of doubles

  `-cheerp-global-prefix=<value>`       Prefix all global names with the given string

  `-cheerp-linear-heap-size=<value>`    Set wasm/asm.js heap size (in MB, default is 8)

  `-cheerp-linear-output=<value>`       Output format for the linear memory part of the program [wasm/wast/asmjs]

  `-cheerp-linear-stack-size=<value>`   Set wasm/asm.js stack size (in MB, default is 1)

  `-cheerp-make-module=<value>`         Expose the compiled code as a [closure/commonjs/es6] module

  `-cheerp-make-module`                 Create a closure around JS to avoid global namespace pollution

  `-cheerp-mode=<value>`                Specify which mode to use by default [genericjs/asmjs]

  `-cheerp-no-credits`                  Disable Cheerp credits in JS

  `-cheerp-no-icf`                      Disable identical code folding on wasm/asmjs

  `-cheerp-no-lto`                      Disable final optimization step at link time

  `-cheerp-no-math-fround`              Disable JavaScript Math.fround

  `-cheerp-no-math-imul`                Disable JavaScript Math.imul

  `-cheerp-no-native-math`              Disable native JavaScript math functions

  `-cheerp-no-pointer-scev`             Disable scalar evolution for pointers

  `-cheerp-preexecute-main`             Run main/webMain in the PreExecuter step. Needs -cheerp-preexecute.

  `-cheerp-preexecute`                  Enable execution of run-time init at compile time

  `-cheerp-pretty-code`                 Generate human-readable JS

  `-cheerp-registerize-legacy`          Use the legacy algorithm for assigning registers

  `-cheerp-reserved-names=<value>`      A list of JS identifiers that should not be used by Cheerp

  `-cheerp-secondary-output-file=<file>`Write the secondary output file (wasm module or asm.js memory file) to `<file>`

  `-cheerp-secondary-output-path=<path>`Assume the secondary output file (wasm module or asm.js memory file) to be in path `<path>` at runtime

  `-cheerp-sourcemap-prefix=<prefix>`   Remove `<prefix>` from sourcemap paths

  `-cheerp-sourcemap-standalone`        Generate a standalone sourcemap by including _all_ sources in the map file

  `-cheerp-sourcemap=<file>`            Write sourcemap to `<file>`

  `-cheerp-strict-linking=<value>`      Enable link time checks for undefined symbols [warning/error]

  `-cheerp-use-bigints`                 Use the BigInt type in JS to represent i64 values

  `-cheerp-wasm-disable=<value>`        Comma separated list of WebAssembly features to disable [sharedmem/growmem/exportedtable/anyref/returncalls]

  `-cheerp-wasm-enable=<value>`         Comma separated list of WebAssembly features to enable [sharedmem/growmem/exportedtable/anyref/returncalls]

  `-cheerp-wasm-externref`              Enable wasm anyref and relax some ffi checks

  `-cheerp-wasm-file=<file>`            Assume wasm file is `<file>`

  `-cheerp-wasm-loader=<file>`          Write wasm loader to `<file>`
