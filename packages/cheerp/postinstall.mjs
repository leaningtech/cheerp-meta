// Sanity check: make sure that cheerp (clang++) reports the expected version

import { resolve } from "path";
import { execa } from "execa";

const dirname = new URL(".", import.meta.url).pathname;

try {
  const { stdout } = await execa(resolve(dirname, "bin", "cheerp.mjs"), [
    "--version",
  ]);
  if (!stdout.startsWith("Cheerp ")) {
    console.error(`Unexpected cheerp version: ${stdout}`);
    process.exit(126);
  }
} catch (error) {
  if (error.exitCode === 127) {
    // "executable not found" message
    console.warn(error.stderr);
  } else {
    console.error(error);
  }
}
