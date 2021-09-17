#include <cheerp/clientlib.h>
#include <cheerp/client.h>

// Forward declaration for the main loop of the game, compiled to WebAssembly
void mainLoop();

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
	// This method is the handler for requestAnimationFrame. The browser will call this
	// in sync with its graphics loop, usually at 60 fps.
	static void rafHandler()
	{
		mainLoop();
		client::requestAnimationFrame(cheerp::Callback(rafHandler));
	}
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
		client::requestAnimationFrame(cheerp::Callback(rafHandler));
	}
	static void drawRect(int x, int y, int w, int h, int rgb)
	{
		int r = rgb&0xff;
		int g = (rgb>>8)&0xff;
		int b = (rgb>>16)&0xff;
		canvasCtx->set_fillStyle(client::String("").concat("rgb(", r, ",", g, ",", b, ")"));
		canvasCtx->fillRect(x, y, w, h);
	}
	static void debugOutput(const char* str)
	{
		canvasCtx->set_font("24px sans-serif");
		canvasCtx->set_fillStyle("rgb(255,255,255)");
		canvasCtx->fillText(str, 0, height - 24);
	}
};

// This whole class will be compiled to Wasm code by default since we are using the -target cheerp-wasm
// command line option. This is a game entity so it's better to get as much performance as we can.
class Platform
{
private:
	int x;
	int y;
	int width;
	int height;
public:
	Platform(int x, int y, int width, int height):x(x),y(y),width(width),height(height)
	{
	}
	int getX() const
	{
		return x;
	}
	int getY() const
	{
		return y;
	}
	int getWidth() const
	{
		return width;
	}
	int getHeight() const
	{
		return width;
	}
	void render() const
	{
		Graphics::drawRect(x, y, width, height, 0xffffff);
	}
};

// Define a global instance for the platform object. A more serious game
// would manage these objects dynamically
Platform platform(185, 380, 30, 7);

void mainLoop()
{
	// Reset the background to black
	Graphics::drawRect(0, 0, 400, 400, 0x000000);
	// Draw the platform
	platform.render();
}

// This function is the entry point of the program. Since we will be compiling this with the -target cheerp-wasm option, it will
// be compiled to WebAssembly by default.
void webMain()
{
	Graphics::initializeCanvas(400, 400);
}
