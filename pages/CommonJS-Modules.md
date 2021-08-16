Cheerp supports the CommonJS standard for JavaScript modules via the `-cheerp-make-module=commonjs` option.
With this option an `exports` object will be automatically populated with all the exported functions.
This allows JavaScript code from other files to use `require()` and import them.

The actual object returned by `require()` is a `Promise`, since the code produced by Cheerp may contain WebAssembly (which is only loaded and compiled asynchronously).

For example, suppose that you have a project in which you need to sort a big array, and you want to replace the slow `Array.sort()` with a faster version. Instead of writing your own sorting function, you can use `std::sort()` from the C++ STL:

```c++
#include <cheerp/client.h>
#include <algorithm>

[[cheerp::jsexport]]
extern "C" void sort(client::Int32Array* a)
{
	int* begin = &(*a)[0];
	int* end = begin + (int)a->get_length();
	std::sort(begin, end);
}
```

You can compile the above code with `/opt/cheerp/bin/clang++ -target cheerp sort.cpp -o sort.js -cheerp-make-module=commonjs` and put the resulting `sort.js` in your project source folder.
Than, using it is as easy as:
```js
let cheerplib = require("./sort");
cheerplib.then(cl => {
	let arr = new Int32Array([4,2,3,1]);
	cl.sort(arr);
	console.log("Sorted with std::sort: "+arr);
});
```

This option is useful to those who wish to use code compiled with Cheerp from Node.js, but it also allows Cheerp code to be used with JavaScript bundlers such as [Webpack](https://webpack.js.org/).