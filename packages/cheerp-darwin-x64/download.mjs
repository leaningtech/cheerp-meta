import { $ } from "execa";
import { mkdir, writeFile, stat, constants } from "fs/promises";

const dest = "cheerp/";
const dmgUrl =
  "https://github.com/leaningtech/cheerp-meta/releases/download/cheerp-3.0/cheerp-macosx-3.0.dmg";
const dmgPath = "cheerp.dmg";

await $`rm -rf ${dest}`
await mkdir(dest);

console.log(`downloading ${dmgUrl}`)
const response = await fetch(dmgUrl);
await writeFile(dmgPath, new Uint8Array(await response.arrayBuffer()));

console.log(`mounting ${dmgPath}`)
const mountResult = await $`hdiutil attach ${dmgPath}`;
const mountPath = mountResult.stdout.split("\t").pop().trim();
try {
  console.log(`copying files to ${dest}`)
  await $`cp -R ${mountPath}/cheerp/ ${dest}`;
} finally {
  await $`hdiutil detach ${mountPath}`;
}

// Check that the binary is executable
const { mode } = await stat(`${dest}/bin/clang++`)
if (mode & constants.S_IXUSR) {
  console.log("clang++ ok!")
} else {
  process.exit(1)
}
