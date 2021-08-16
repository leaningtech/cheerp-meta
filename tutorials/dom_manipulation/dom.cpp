#include <cheerp/client.h> //Misc client side stuff
#include <cheerp/clientlib.h> //Complete DOM/HTML5 interface

void outputNumberOfElementsToTheConsole()
{
        double number=client::document.getElementsByTagName("*")->get_length();
        client::console.log("Live elements = ", number);
}

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

//This function will be called only after the DOM is fully loaded
void loadCallback()
{
        client::console.log("Hi from loadCallback!");

        //Do something with the document
        outputNumberOfElementsToTheConsole();
	setupInputAndDisplay();
        outputNumberOfElementsToTheConsole();
	setupInputAndDisplay();
        outputNumberOfElementsToTheConsole();
        client::console.log("Bye from loadCallback!");
}

void webMain()
{
        client::console.log("Hi from webMain!");
        outputNumberOfElementsToTheConsole();

        client::document.addEventListener("DOMContentLoaded",cheerp::Callback(loadCallback));
        client::console.log("Bye from webMain!");
}
