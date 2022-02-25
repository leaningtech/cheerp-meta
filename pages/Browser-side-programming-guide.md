---
title: Tutorial - DOM & the Browser
---

# Input and output
In the [first](Getting-started#hello-world) [two](Tutorial-Hello-Wasm) tutorials we used no input methods and only printing to the console as the output method.

Now we will expore a more powerful interface: accessing and modifying the DOM.

# Accessing the DOM

## The client namespace

Browser APIs are declared inside headers which are provided with Cheerp, namely

```c++
#include <cheerp/client.h> //Misc client side stuff
#include <cheerp/clientlib.h> //Complete DOM/HTML5 interface
#include <cheerp/webgl.h> //WebGL interface
```

All classes, global variables and methods exposed by the browser are declared into the ```client``` namespace. It's a regular C++ namespace, so it's always possible to write more terse code with:

```c++
using namespace client;
```

## The document object

You can access the ```document``` global object directly from C++ code. In the next example we will add an event handler to run our code after the DOM is fully loaded. ([dom.cpp](/tutorials/dom_access/dom.cpp))

```c++
#include <cheerp/client.h> //Misc client side stuff
#include <cheerp/clientlib.h> //Complete DOM/HTML5 interface

[[cheerp::genericjs]]
void outputNumberOfElementsToTheConsole()
{
        double number=client::document.getElementsByTagName("*")->get_length();
        client::console.log("Live elements = ", number);
}

//This function will be called only after the DOM is fully loaded
[[cheerp::genericjs]]
void loadCallback()
{
        client::console.log("Hi from loadCallback!");

        //Do something with the document
        outputNumberOfElementsToTheConsole();
        client::console.log("Bye from loadCallback!");
}

[[cheerp::genericjs]]
void webMain()
{
        client::console.log("Hi from webMain!");
        outputNumberOfElementsToTheConsole();

        client::document.addEventListener("DOMContentLoaded",cheerp::Callback(loadCallback));
        client::console.log("Bye from webMain!");
}

```

Compiling with:
```/opt/cheerp/bin/clang++ -O3 dom.cpp -o dom.js```

Now we need a [html file](/tutorials/dom_access/dom.html):
```html
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <title>Cheerp example 2: DOM manipulation</title>
    <script src="dom.js"></script>
  </head>
  <body>
    <img src="{{site.baseurl}}/assets/Diagram_browser.png"></img>
  </body>
</html>
```

And opening it (eg. `firefox dom.html`) and checking the console log, the output should be something like:
```
Hi from webMain!
Live elements =  5
Bye from webMain!
Hi from loadCallback!
Live elements =  7
Bye from loadCallback!
```

There are two important things to notice:
+ the program outlived `webMain()` (while a normal C++ program terminates when the `main` returns) ([why?](Cheerp-basics#the-webmain-entry-point))
+ running code directly from inside `webMain()` can lead to race conditions, where depending on the execution order we may perform invalid operations (since the DOM is not fully formed). The pattern of invoking a callback on `DOMContentLoaded` is very important and will be used also in all the following examples, and so it is the more general pattern of using `cheerp::Callback`, that we will now examinate.

## The ```cheerp::Callback``` adapter function

```Callback``` is a function defined in the ```cheerp``` namespace which is required to use C++ functions, functors and lambads as callbacks for browser events. For example.

```
cheerp::Callback(regularCXXFunc);
cheerp::Callback(CXXFunctor());
cheerp::Callback([](client::Event*){ ... });
cheerp::Callback([capturedVariable](client::Event*){ ... });
```

For more in depth information you can take a loop at our [API documentation](http://leaningtech.com/API/namespacecheerp.html#a3dd35fdcd0dd20e953ca993dcbbd34af)

The general usage is:

```
element->addEventListener("event_kind", cheerp::Callback(callback));
```

cheerp::Callbacks are a very powerful instrument:
* they can be attached to any pair of DOM element + [`client::Event`](https://developer.mozilla.org/en-US/docs/Web/Events) to capture mouse movements, clicks, change of focus, button clicks, keyboard buttons pressed, etc.
* Callbacks can be attached also to `requestAnimationFrame` or `setTimeout`, see expecially [Pong](Cheerp-Tutorial-Mixed-mode-C++-to-WebAssembly-and-JavaScript)
* Callback could be attached to `DOMContentLoaded` or equivalent events to do initializations (for example, of other callbacks)

## Manipulating the DOM

Cheerp works at the same level as JavaScript. It is designed to complement or replace JavaScript as a programming language for the Web. It does not attempt to replace other Web technologies such as HTML and CSS. When using Cheerp you still have to design your page using HTML, CSS and the tools you already know.

If you want to manipulate the DOM at run-time you can use the same APIs you would use when writing JavaScript. In the following example we will create two DOM elements and set up event handling using the DOM APIs exposed by the browser.

```c++
[[cheerp::genericjs]]
void setupInputAndDisplay()
{
        using namespace client;

	//Retrieve the <body> element
	client::HTMLElement * body = client::document.get_body();

	//Create a text input element <input type="text">
	HTMLInputElement * inputBox = static_cast<HTMLInputElement*>(document.createElement("input") );
	inputBox->setAttribute("type", "text");

	//This sets the default value
	inputBox->setAttribute("value", "Insert anything in here");

	//We can also style it here, but CSS would be better
	inputBox->setAttribute("style", "width:200px");

	//Create a new <h1> element
	HTMLElement * textDisplay = document.createElement("h1");

	//use a C++11 lambda to capture the variables we need
	auto mirrorText = [textDisplay, inputBox]() -> void {
		//Update the <h1> element with whatever is written in the <input> element
		String * text = inputBox->get_value();
		textDisplay->set_textContent( text );
	};  

	//Call the lambda to set the textDisplay to the initial value
	mirrorText();

	//Set up the handler for the input event. Use a C++11 lambda to capture the variables we need
	inputBox->addEventListener("input", cheerp::Callback(mirrorText));

	//Add the new elements to the <body>
	body->appendChild( textDisplay );
	body->appendChild( inputBox );
}
```

Now placing a call to `setupInputAndDisplay()` from inside `loadCallback()` will add the two elements to the body.

(feel free to modify the previous `dom.cpp`. The `dom.html` file will remain the same, [link](/tutorials/dom_manipulation/dom.cpp))

## Even more `cheerp::Callback`: buttons & mouse
[buttons.cpp](/tutorials/dom_buttons/buttons.cpp) is an example of various uses for the `cheerp::Callback` to generate buttons, text elements, check mouse position, etc. Compile it to JavaScript, keep using the same `dom.html` as before, click on a few buttons.

# Recap

We saw how to work with callbacks, a very general instrument that can be used to capture any kind of event.

You can now start experimenting on your own or follow our other tutorials.
