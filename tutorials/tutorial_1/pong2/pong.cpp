#include <cheerp/clientlib.h>
#include <cheerp/client.h>

// All the graphics code should stay on the JS side. It is possible to tag whole classes with the [[cheerp::genericjs]] tag.
// All members and methods of this class will be compiled to standard JavaScript.
class [[cheerp::genericjs]] Graphics
{
private:
	// When compiling to standard JavaScript it is possible to use DOM objects like any other C++ object.
	static client::HTMLCanvasElement* canvas;
	static client::CanvasRenderingContext2D* canvasCtx;
	static int width;
	static int height;
public:
	// These methods are here to provide access to DOM and Canvas APIs to code compiled to WebAssembly
	static void initializeCanvas(int w, int h)
	{
		width = w;
		height = h;
		canvas = (client::HTMLCanvasElement*)client::document.getElementById("pongcanvas");
		canvas->set_width(w);
		canvas->set_height(h);
		client::document.get_body()->appendChild(canvas);
		canvasCtx = (client::CanvasRenderingContext2D*)canvas->getContext("2d");
	}
	static void debugOutput(const char* str)
	{
		canvasCtx->set_font("24px sans-serif");
		canvasCtx->fillText(str, 0, height - 24);
	}
};

// This function is the entry point of the program. Since we will be compiling this with the -target cheerp-wasm option, it will
// be compiled to WebAssembly by default.
void webMain()
{
        Graphics::initializeCanvas(400, 400);
        Graphics::debugOutput("Hello Canvas");
}
