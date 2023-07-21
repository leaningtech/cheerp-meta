/** @type {Record<string, Record<string, { package: string, bin: string }>} */
const platforms = require("../platforms.json");

const path = require("path");
let { platform, arch } = process;

// TEMP until we have Apple Silicon support
if (platform === "darwin" && arch === "arm64") {
	arch = "x64";
}

const INSTALL_DOCS_URL = `https://docs.leaningtech.com/cheerp/Getting-started#install`; // TODO: point to labs once live

const manifest = platforms?.[platform]?.[arch];
const bin = path.join(manifest.package, manifest.bin);

if (bin) {
	try {
		require.resolve(bin);
	} catch {
		console.warn(
`The Cheerp npm package postinstall script failed to resolve the binary file "${bin}".
Running Cheerp from the npm package will not work correctly.
Your platform: ${platform}-${arch}
Please open an issue:
	https://github.com/leaningtech/cheerp-meta/issues/new
In the meantime, you can install Cheerp with another method.
See the following instructions for more information:
	${INSTALL_DOCS_URL}`
		);
	}
} else {
	console.warn(
`The Cheerp npm package doesn't ship with prebuilt binaries for your platform yet.
Your platform: ${platform}-${arch}
Please see the following instructions for more information:
	${INSTALL_DOCS_URL}`
	);
}
