# WebWorkers vs. PThreads

PThreads is the main threading API on posix platforms, such as Linux. It makes it possible to run concurrent threads of execution that have the same view of memory (i.e. they run in the same address space).

WebWorkers are a message based concurrency API for the Web platform. A WebWorker is a "thread" which executes a JavaScript file which is completely independent and isolated from the main page code and other WebWorkers. Since the script inside a WebWorker has an isolated view of memory a WebWorker is more similar to a native process, than to a native thread.

# PThread in Cheerp?

Cheerp does not support yet PThread APIs, but it's a work in progress. Please get in touch should you want to be kept informed.

JavaScript, compiled or hand-written, miss feature that allow actual threading, while the Web Assembly part of the program could benefit from multithreading. Browser support for now it's only experimental, but that will change in the near future.

# WebWorkers in Cheerp!

Cheerp is designed to give you full access to all browser APIs, including WebWorkers.

## Using WebWorkers with Cheerp

You can use Cheerp to generate JavaScript running in the Worker, JavaScript in the main page that run a Worker, or both. Below you find a trivial example ported from http://www.html5rocks.com/en/tutorials/workers/basics/.

```cpp
// cheerpWorkerHost.cpp: Code to include in the HTML page
#include <cheerp/client.h>
#include <cheerp/clientlib.h>

using namespace client;

void webMain()
{
        Worker* w = new Worker("cheerpWorker.js");
        w->addEventListener("message", cheerp::Callback([](MessageEvent* e) {
                                        console.log((String*)(e->get_data())); }));
        w->postMessage("Hello World");
}
```

```cpp
// cheerpWorker.cpp: Code that run inside the worker
#include <cheerp/clientlib.h>
#include <cheerp/client.h>

using namespace client;

void webMain()
{
       addEventListener("message", cheerp::Callback([](MessageEvent* e) {
                               postMessage(e->get_data());
                               postMessage(e->get_data());
                               }));
}
```

You can build each files using the following command line (for more information see [[Getting Started]])
```
/opt/cheerp/bin/clang++ -target cheerp cheerpWorkerHost.cpp -o cheerpWorkerHost.js
/opt/cheerp/bin/clang++ -target cheerp cheerpWorker.cpp -o cheerpWorker.js
```
For your convenience here is the needed HTML to execute the code

```html
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <title>Cheerp test</title>
    <script src="cheerpWorkerHost.js"></script>
  </head>
  <body>
  </body>
</html>
```

Start `http-server`, navigate to the right html, and check the console log.
You should see a greeting echoed back **twice** through a Worker.
