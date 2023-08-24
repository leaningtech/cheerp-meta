import { URL } from "url";
import { resolve } from "path";

export const path = resolve(new URL(".", import.meta.url).pathname, "cheerp");
