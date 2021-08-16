---
title: Source Maps
---

Cheerp takes advantage of the [SourceMaps web standard](http://www.html5rocks.com/en/tutorials/developertools/sourcemaps/) to provide in-browser debugging of C++ source code. Generating a source map allows the browser to know the location of the C++ source files. Most modern browsers also support showing the C++ source code while debugging a Cheerp-compiled application, both when an exception is raised or on a manually set breakpoint.

# Enabling source map support

## Firefox

1. Open the debug tool window (Ctrl+Shift+I)
2. Select the debugging tab (Circled in red)
3. Click the settings button (Circled in green)
4. Make sure "Show Original Sources" is selected

![](https://raw.githubusercontent.com/wiki/leaningtech/cheerp-wiki/debug1.png)

## Chrome

1. Open the debug tool window (Ctrl+Shift+I)
2. Select the "Sources" tab (Circled in red)
3. Click the settings button (Circled in green)
4. Make sure "Enable JavaScript source maps" is selected

![](https://raw.githubusercontent.com/wiki/leaningtech/cheerp-wiki/debug3.png)

![](https://raw.githubusercontent.com/wiki/leaningtech/cheerp-wiki/debug2.png)

# How to generate Source Maps

Cheerp can generate Source Maps together with the final compiled JavaScript file using a single command.
For example see the following command (valid for **Linux** and **Mac OS X**, on **Windows** the Cheerp executable is located in c:\cheerp\bin\clang++.exe)

```
/opt/cheerp/bin/clang++ -target cheerp file.cpp -o file.js -g -cheerp-sourcemap=file.js.map
```

The standard ```-g``` option is used to generate debugging information in the frontend.
The Cheerp specific ```-cheerp-sourcemap``` option instructs Cheerp to output the source map file in the given file. You can of course use any name for the source map file.

# Practical example

Follow the instruction in the [Cheerp Tutorial](https://github.com/leaningtech/cheerp-wiki/wiki/Cheerp_Tutorial#example1) and create the ```example.cpp``` and ```example.html``` files.

Build ```example.js``` and ```example.js.map``` using the following command line

```
/opt/cheerp/bin/clang++ -target cheerp example.cpp -o example.js -g -cheerp-sourcemap=example.js.map
```

To see Source Maps in action, please start a Web server in the current directory and then access it using your favorite browser.

For example, you could use python to start a local Web server

```
python -m SimpleHTTPServer &
firefox http://127.0.0.1:8000/example.html
```

If Source Maps are enabled (as described above) you should see the C++ source code in the debugging tab of the developers tools.

![](https://raw.githubusercontent.com/wiki/leaningtech/cheerp-wiki/debug4.png)

# Command line options

* ```-g``` Required to get the compiler to generate debugging metadata
* ```-cheerp-sourcemap=filename``` Tells Cheerp what file should contain the generates Source Maps
* ```-cheerp-sourcemap-prefix=path``` Tells Cheerp to remove the given prefix from all the source files paths that are stored inside the Source Maps file.
* ```-cheerp-sourcemap-standalone``` Tells Cheerp to include all sources into the sourcemap file. This makes it easier to debug using Source Maps since you only need to put the map file on the Web Server, instead of the whole source code. Since the map contains the sources you should remember not to deploy this file in production.

# What you can do with Source Maps?

When Source Maps are enabled you can:

* Set break points in C++ code
* Single-step through C++ code
* See C++ code when an exception is raised

# Limitations

The Source Map standard does not allow any kind of variable inspection or mapping source variable to compiled variables.
This means that it is not possible to Watch the values of C++ variables or change them. We plan to enable this possibility when the standard will support it.
