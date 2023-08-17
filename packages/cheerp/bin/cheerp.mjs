#!/usr/bin/env node

import { path } from "../index.mjs";
import { resolve } from "path";
import { spawnSync } from "child_process";

const suffix = process.platform === "win32" ? ".exe" : "";
const binPath = resolve(path, "bin", "clang++" + suffix);

const { error, status } = spawnSync(binPath, process.argv.slice(2), {
  shell: false,
  stdio: "inherit",
});
if (error) {
  if (error.code === "ENOENT") {
    console.error(
      `executable not found: ${binPath}. please install Cheerp to "${path}": https://labs.leaningtech.com/cheerp/installation`,
    );
    process.exit(127);
  }
  throw error;
}
process.exitCode = status;
