---
title: Tutorial - Pong
---

This tutorial will guide you through the steps to create a simple game of pong in C++, and compile it to a combination of WebAssembly (game logic) and JavaScript (game graphics) with Cheerp 2.0.

# Hello World
Source code: [pong.cpp](tutorials/tutorial_1/pong1/pong.cpp). Link to the example: [link](https://oldsite.leaningtech.com/cheerp/examples/pong1/pong.html).

We will start by building a simple console-based Hello World. To get started, you need to download Cheerp and [install](Linux-build-instructions) it on your machine. In this tutorial, we will assume a Linux installation of Cheerp available on ``/opt/cheerp/bin/clang++``.

Let's create a new C++ file ``pong.cpp`` as follows:

```C++
#include <cheerp/clientlib.h>
#include <cheerp/client.h>

[[cheerp::genericjs]] void domOutput(const char* str)
{
    	client::console.log(str);
}

void webMain()
{
    	domOutput("Hello World");
}
```

``webMain`` represents the main entry point of the compiled application. Since WebAssembly cannot manipulate the DOM directly, we are instructing Cheerp to compile ``domOutput`` in JavaScript with the ``[[cheerp::genericjs]]`` attribute tag. To compile this file to WebAssembly, this simple command will suffice:

```Shell
/opt/cheerp/bin/clang++ -target cheerp-wasm -O2 -o pong.js pong.cpp
```

This command will generate two files: a JavaScript loader (``pong.js``) and a WebAssembly binary (``pong.wasm``). We will need both files to run the application on a browser. When using Cheerp in mixed-output mode, the JavaScript output will also include all the JavaScript output code, which will be normally used to interact with the DOM or any external JavaScript library.

All we need to run this Hello World is a simple HTML page:

```HTML
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <title>Cheerp test</title>
  </head>
  <body>
    <canvas id="pongcanvas"></canvas>
    <script src="pong.js"></script>
  </body>
</html>
```

We only need to reference the JavaScript loader, and the WebAssembly binary will be automatically loaded.

You now need to start a local HTTP server in the directory containing pong.html, pong.js and pong.wasm. Node’s ```http-server -p 8081``` command work well, but any server will do.

Visit your page, for example “http://127.0.0.1:8081” with any browser, and Yyou should see something like this:

<img src="{{site.baseurl}}/tutorials/tutorial_1/pong1/pong1.png" width="600px">

Great! We can now move on to build an Hello World that writes on the Canvas.

# Hello World (Canvas)
Source code: [pong.cpp]({{site.baseurl}}/tutorials/tutorial_1/pong2/pong.cpp). Link to the example: [link](https://oldsite.leaningtech.com/cheerp/examples/pong2/pong.html).

What we will do in this tutorial is keeping all the Canvas manipulation in the JavaScript output, and the rest of the game in WebAssembly. We will be doing our rendering using the Canvas 2D Web API, and we will also use ``requestAnimationFrame'' for best performance. The Canvas 2D API is not directly accessible from WebAssembly, so we will ask the compiler to generate JavaScript code on the relevant sections.

Let's have a look a this example:

```C++
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

// This code execute in wasm, and will automatically call the code in the graphics class which is compiled to JS
void webMain()
{
    	Graphics::initializeCanvas(400, 400);
    	Graphics::debugOutput("Hello Canvas");
}
```

We have created a new ```Graphics``` class and tagged it using the ```[[cheerp::genericjs]]``` attribute. Cheerp will generate all the code for this class in standard JavaScript, so that DOM manipulation can be done easily in C++ with no performance or syntax overhead. The code in webMain is compiled to WebAssembly and will use the WebAssembly imports to call the code compiled to JavaScript. 

Compile this new code like we did before and refresh the browser tab, you should now see something like this:

<img src="{{site.baseurl}}/tutorials/tutorial_1/pong2/pong2.png" width="600px">

Great stuff.

# Drawing on the Canvas
Source code: [pong.cpp]({{site.baseurl}}/tutorials/tutorial_1/pong3/pong.cpp). Link to the example: [link](https://oldsite.leaningtech.com/cheerp/examples/pong3/pong.html).

Let's get started on our game. We will now focus on drawing the paddle.

For this, we will create a ``Platform`` object, which will be the only controllable entity in the game. The object will be moved to the left and to the right using the keyboard and will be rendered as white box on a black background.

Let’s create a new class for the Platform. We will **not** use the [[cheerp::genericjs]] attribute on this class so that all it’s code will be compiled in WebAssembly.

```C++
class Platform
{
private:
    	int x;
    	int h;
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
            	Graphics::drawRect(w, y, width, height, 0xffffff);
    	}                                 	 
};
```

The class has some basic properties and a render function which then delegates the actual rendering to the ```Graphics``` class since on the WebAssembly side we don’t have access to the DOM. Let’s add the drawRect function to the Graphics class:

```C++
    	static void drawRect(int w, int y, int width, int height, int rgb)
    	{
            	int r = rgb&0xff;
            	int g = (rgb>>8)&0xff;
            	int b = (rgb>>16)&0xff;
            	canvasCtx->set_fillStyle(client::String("").concat("rgb("r, ",", g, ",", b, ")"));
            	canvasCtx->fillRect(x, y, width, height);
    	}
```

We now need an instance of the Platform object, let’s put it in the global scope for convenience, and also add a top level function to handle the main loop of the application. This function will clear the background and the render the platform.

Let's recompile, and the result should look like this:
<img src="{{site.baseurl}}/tutorials/tutorial_1/pong3/pong3.png" width="600px">

Looking good.

# Animation and Keyboard events
Source code: [pong.cpp]({{site.baseurl}}/tutorials/tutorial_1/pong4/pong.cpp). Link to the example: [link](https://oldsite.leaningtech.com/cheerp/examples/pong4/pong.html).

We now need to be able to move the platform around. We will add a ```keydown``` event handler for this purpose. Since it’s a DOM interaction, this code will be tagged as ````genericjs```, but it will update the values of the ```Platform``` object which is compiled to WebAssembly.

Let's add two new methods to ``Platform``:

```C++
    	void moveLeft()
    	{
            	x -= 3;
    	}
    	void moveRight()
    	{
            	x += 3;
    	}

```

as well as a keyDown event handler to the Graphics class which lives in ```genericjs``` code.

```C++
void Graphics::keyDownHandler(client::KeyboardEvent* e)
{
    	if(e->get_keyCode() == 37)
            	platform.moveLeft();
    	else
            	platform.moveRight();
}
```

You should now be able to move the paddle around like this:
<img src="{{site.baseurl}}/tutorials/tutorial_1/pong4/pong4.gif" width="600px">

Wow.

# Final steps
Source code: [pong.cpp]({{site.baseurl}}/tutorials/tutorial_1/pong5/pong.cpp). Link to the example: [link](https://oldsite.leaningtech.com/cheerp/examples/pong5/pong.html).

We'll now focus on the ball. We will create a ```Ball``` class, including a basic physical model of position and velocity. We will keep this class in WebAssembly, so no need for a ``[[cheerp::genericjs]]`` tag.

```C++
class Ball
{
private:
    	int x;
    	int y;
    	int vx;
    	int vy;
public:
    	Ball(int x, int y, int vx, int vy):x(x),y(y),vx(vx),vy(vy)
    	{
    	}
    	void update()
    	{
            	x += vx;
            	y += vy;
    	}
    	// Returns true if the ball gets out of the field
    	bool collide(const Platform& platform, int maxX, int maxY)
    	{
            	// If we collided with the bottom side, we lost
            	if(y >= maxY)
                    	return true;
            	// Check left and right side collisions
            	if(x <= 0 || x >= maxX)
                    	vx = -vx;
            	// Check top side collision
            	if(y <= 0)
                    	vy = -vy;
            	// Check collision with the top side of the plaform
            	if(platform.getX() < x && (platform.getX() + platform.getWidth()) > x &&
                    	platform.getY() < y && (platform.getY() + platform.getHeight()) > y)
            	{
                    	vy = -vy;
            	}
            	return false;
    	}
    	void render()
    	{
            	Graphics::drawCircle(x, y, 5, 0xffffff);
    	}
};
```

The ```Ball``` class has methods to update its position, check for collisions and for rendering. 

We will now expand the ```mainLoop``` method to call them in sequence. ```Ball::collide``` also checks if the ball has gone past the bottom of the screen, which is our losing condition, and ```mainLoop``` is going to report that by drawing some text.

```C++
void mainLoop()
{  	 
    	// Reset the background to black
    	Graphics::drawRect(0, 0, 400, 400, 0);
    	// Draw the platform
    	platform.render();
    	// Update the ball state
    	ball.update();
    	// Check for collisions
    	bool hasLost = ball.collide(platform, 400, 400);
    	if(hasLost)
            	Graphics::debugOutput("You lost!");
    	// Render the ball
    	ball.render();
}
```

And that should be it! The game should look like this:
<img src="{{site.baseurl}}/tutorials/tutorial_1/pong5/pong5.gif" width="600px">

Not too bad!

# Conclusions

WebAssembly is an amazing opportunity to truly realize the promise of the Web as a universal platform. 

[Cheerp](https://github.com/leaningtech/cheerp-meta) contributes to the WebAssembly and 'Web as a platform' ecosystem by enabling C/C++ to be compiled to a combination of high-performance WebAssembly code, and high-flexilibity JavaScript code, with full transparent interaction with the DOM and external JavaScript libraries, and dynamic memory / garbage collection.  

You can [download Cheerp](https://leaningtech.com/cheerp/#download) today and start using it in a few minutes. Give it a try!
