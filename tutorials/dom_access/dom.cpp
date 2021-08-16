#include <cheerp/client.h> //Misc client side stuff
#include <cheerp/clientlib.h> //Complete DOM/HTML5 interface

void outputNumberOfElementsToTheConsole()
{
        double number=client::document.getElementsByTagName("*")->get_length();
        client::console.log("Live elements = ", number);
}

//This function will be called only after the DOM is fully loaded
void loadCallback()
{
        client::console.log("Hi from loadCallback!");

        //Do something with the document
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
