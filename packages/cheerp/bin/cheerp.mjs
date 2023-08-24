#!/usr/bin/env node

import { path } from "../index.mjs";
import { resolve } from "path";
import { spawnSync } from "child_process";

const [, , binName, ...args] = process.argv;
let exitCode = 0;

switch (binName) {
  case "clang":
  case "clang++":
    runCommand(binName, args);
    break;
  case "prefix":
    console.log(path);
    break;
  default:
    console.error(`unknown command: ${binName}`);
    console.log("");
    exitCode = 1;
    // fallthrough
  case undefined:
  case "":
  case "help":
    showHelp();
    break;
}

process.exit(exitCode);

function showHelp() {
  console.log(`Cheerp 3.0 (https://labs.leaningtech.com/cheerp)`);
  console.log(``);
  console.log(`Usage:`)
  console.log(`  $ cheerp [command] [options]`);
  console.log(``);
  console.log(`Commands:`);
  console.log(`  clang         C compiler`);
  console.log(`  clang++       C++ compiler`);
  console.log(`  prefix        Print installation path prefix`);
  console.log(`  help          Display this message`);
}

function runCommand(binName, args) {
  const suffix = process.platform === "win32" ? ".exe" : "";
  const binPath = resolve(path, "bin", binName + suffix);

  const { error, status } = spawnSync(binPath, args, {
    shell: false,
    stdio: "inherit",
  });
  if (error) {
    if (error.code === "ENOENT") {
      console.error(`executable not found: ${binPath}`);
      console.error("installation has failed, please report this issue: https://github.com/leaningtech/cheerp-meta/issues");
      process.exit(127);
    }
    throw error;
  }

  exitCode = status;
}
