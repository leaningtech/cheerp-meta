#include <cheerp/clientlib.h>
#include <cheerp/client.h>

// This function is tagged with [[cheerpj::genericjs]] and will be compiled to standard JavaScript
// Functions tagged in this way can be called from code compiled to WebAssembly
[[cheerp::genericjs]] void domOutput(const char* str)
{
	// Since this function is compiled to plain JavaScript it has direct access to all DOM APIs.
	// In this case we use console.log to output a debug message.
	client::console.log(str);
}

// This function is the entry point of the program. Since we will be compiling this with the -target cheerp-wasm option, it will
// be compiled to WebAssembly by default.
void webMain()
{
	// From WebAssembly it is not possible to directly use DOM APIs, but you can call C++ functions tagged with [[cheerp::genericjs]]
	// which can interoperate easily with DOM APIs and JavaScript libraries.
	domOutput("Hello World");
}
