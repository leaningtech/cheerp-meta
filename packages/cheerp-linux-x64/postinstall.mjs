import { brotliDecompress } from "zlib";
import { promisify } from "util";
import { readFile, rmdir } from "fs/promises";
import { Readable } from "stream";
import { extract } from "tar-fs";

rmdir("cheerp", { recursive: true }).catch(() => {});

let buffer = await readFile("cheerp.tar.br");
buffer = await promisify(brotliDecompress)(buffer);
await new Promise(resolve => Readable.from(buffer).pipe(extract(".")).once("close", resolve));
