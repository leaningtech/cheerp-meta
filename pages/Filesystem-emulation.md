As part of the ```libposixcompat``` library, Cheerp includes functionality to emulate filesystem access for ported applications. This library is only available to commercial users of Cheerp.

## Usage

When running code in the main thread of the browser it is not possible to make any blocking call, which means that traditional POSIX APIs such as ```open/read/close``` cannot be directly implemented. To work around this browser limitation it is necessary to preload all the additional data files that will be used at runtime. 

You can achieve this by using the following API

```
#include <cheerpfs.h>

// Allocate a global object of type cheerp::FilePreloader
// The first parameter is a callback that will be invoked when the preloading is completed.
// After the first parameter an arbitrary number of files can be passed
// The files will be loaded over HTTP and made available using the standard ```open/read/close``` API.

void preloadCallback()
{
    // Files are ready
    int fd = open("data1.xml");
    ...
}

cheerp::FilePreloader files(preloadCallback, "data1.xml", "images/asset1.png");

```