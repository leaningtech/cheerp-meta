import * as fs from "node:fs"; // TODO: promisify
import { resolve } from "node:path"
import { fileURLToPath } from "node:url";
import { pipeline } from "node:stream/promises";
import { exec } from "node:child_process";

const ROOT = resolve(fileURLToPath(import.meta.url), "../..");
const PACKAGE_JSON = resolve(ROOT, "package.json");
const PLATFORMS_JSON = resolve(ROOT, "platforms.json");

/** @type {{ [platform: string]: { [arch: string]: { package: string, bin: string } } }} */
const platforms = JSON.parse(fs.readFileSync(PLATFORMS_JSON, "utf-8"));

/** @type {{ version: string, license: string }} */
const { version, license } = JSON.parse(fs.readFileSync(PACKAGE_JSON, "utf-8"));

// TODO: get current git tag
const tag = "cheerp-3.0";
const suffix = tag.split("-")[1];

for (const [platform, arches] of Object.entries(platforms)) {
	for (const [arch, manifest] of Object.entries(arches)) {
		await generatePackage(platform, arch, manifest)
	}
}

async function generatePackage(platform, arch, manifest) {
	console.log(`Generating package ${manifest.package}@${version}`)

	const dir = resolve(ROOT, `packages/${manifest.package}`);
	console.log(dir)

	fs.rmSync(dir, { recursive: true, force: true });
	fs.mkdirSync(dir, { recursive: true });

	const packageJson = {
		name: manifest.package,
		version,
		license,
		os: [platform],
		cpu: platform === "darwin" ? [arch, "arm64"] : [arch], // TEMP until we have Apple Silicon support
	};
	fs.writeFileSync(resolve(dir, "package.json"), JSON.stringify(packageJson, null, 2));

	if (platform === "darwin") {
		const bins = await getDarwinBinaries(dir);
		console.log(bins)
	} else {
		throw new Error(`Don't know how to download binaries for platform "${platform}"`)
	}
}

/** @arg {string} targetDir */
/** @returns {Promise<string[]>} */
async function getDarwinBinaries(targetDir) {
	const dmg = resolve(targetDir, "cheerp.dmg");
	console.log(`Downloading dmg to ${dmg}`)
	{
		const url = `https://github.com/leaningtech/cheerp-meta/releases/download/${tag}/cheerp-macosx-${suffix}.dmg`;

		const res = await fetch(url);
		if (!res.ok) {
			throw new Error(`Failed to download "${url}"; status ${res.status}`);
		}

		const stream = fs.createWriteStream(dmg);
		//res.body.pipeTo(stream);

		await pipeline(res.body, stream);

		await new Promise((resolve, reject) => {
			stream.on("finish", resolve);
			stream.on("error", reject);
		});
		stream.close();
	}

	console.log("Mounting dmg")
	const { stdout } = await new Promise((resolve, reject) => exec(`hdiutil attach ${dmg}`, (err, stdout, stderr) => {
		if (err) reject(err);
		else resolve({ stdout, stderr });
	}));
	console.log(stdout);

	fs.rmSync(dmg);

	return bins;
}
