import { $ } from "execa";
import { writeFile, stat, constants } from "fs/promises";

const dest = ".";
const url =
  "https://output.circle-artifacts.com/output/job/38375ec3-45be-4ac9-8662-8f2c777ab578/artifacts/0/~/project/packages/cheerp-macosx-1692034395.tar.bz2";
const tarPath = "cheerp.tar.bz2";

await $`rm -rf cheerp`

console.log(`downloading ${url}`)
const response = await fetch(url);
await writeFile(tarPath, new Uint8Array(await response.arrayBuffer()));

console.log(`extracting ${tarPath}`)
await $`tar -xjf ${tarPath} -C ${dest}`;

// Check that the binary is executable
const { mode } = await stat(`cheerp/bin/clang++`)
if (mode & constants.S_IXUSR) {
  console.log("clang++ ok!")
} else {
  process.exit(1)
}
