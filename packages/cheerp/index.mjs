/**
 * @returns {Promise<{ path: string }>}
 */
async function importCheerp() {
  let { platform, arch } = process;

  // We don't support Apple Silicon natively yet
  if (platform === "darwin" && arch === "arm64") {
    arch = "x64";
  }

  try {
    return await import(`@leaningtech/cheerp-${platform}-${arch}`);
  } catch (error) {
    if (error?.code !== "ERR_MODULE_NOT_FOUND") {
      throw error;
    }
    return {
      path: platform === "win32" ? "C:\\cheerp\\" : "/opt/cheerp",
    };
  }
}

const impl = await importCheerp();

export const { path } = impl;
