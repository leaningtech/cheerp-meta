# Publishing (for maintainers)

1. Update x.y version number in [cheerp.mjs](/packages/cheerp/bin/cheerp.mjs) `showHelp`
2. Update package versions: `npm version <version> --workspaces -f`
3. Update `optionalDependencies` of [cheerp package.json](/packages/cheerp/package.json) to match the new version
4. Publish: `npm publish --access=public --workspaces`
