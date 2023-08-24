const { join } = require("path");

console.log("win32 dirname: ", __dirname);

module.exports = { path: join(__dirname, "cheerp") };
