# Using DOM APIs

The following example shows how to write a simple Web application that uses the Browser's DOM functionalities. Check our [online API reference](http://leaningtech.com/API) for extensive documentation of Web APIs in the client namespace using Cheerp.

```c++
#include <cheerp/clientlib.h>

void webMain()
{
    // client::document represent the Javascript document object
    client::Element* titleElement = \ 
                client::document.getElementById("pagetitle");
    // Wide strings can also be implicitly converted to JavaScript Strings
    titleElement->set_textContent(L"Exciting dynamic title,\
                 with non-Latin letters ΩЯÅ");
}
```
Save it as `dom1.cpp` and compile it (how? `/opt/cheerp/bin/clang++ -target cheerp dom1.cpp -o dom1.js`)

Now embed it in a web page like:

```html
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <title>Cheerp test</title>
    <script defer src="dom1.js"></script>
  </head>
  <body>
  <h1 id="pagetitle">Boring static text</h1>
  </body>
</html>
```

and open it with your favourite browser.

As you can see the text should have been `Boring static text` but has been replaced by our script.

![](https://raw.githubusercontent.com/wiki/leaningtech/cheerp-wiki/Dom1.png)

## Using callbacks <a name="callbackuse"></a>

Let's extend the previous program to revert the original text after 3 seconds.

```c++
#include <cheerp/client.h>
#include <cheerp/clientlib.h>

// client is a regular C++ namespace, so we can reduce verbosity by using it
using namespace client;

void webMain()
{
    Element* titleElement=document.getElementById("pagetitle");
    // Save the old text, it is a JavaScript String
    String* oldText=titleElement->get_textContent();
    titleElement->set_textContent(L"Exciting dynamic title, with non-Latin letters ΩЯÅ");
    // setTimeout is included in the client namespace like other browser APIs
    // cheerp::Callback is bridge function to invoke C++ lambdas and functions as
    // JavaScript callbaks. We use C++11 lambda in this case.
    setTimeout(cheerp::Callback([titleElement,oldText](client::Event* e)
        {
            titleElement->set_textContent(oldText);
        }), 3000);
}
```

# Interoperability with JavaScript code

Cheerp is designed to offer lightweight, bidirectional interoperability between C++ and JavaScript code. Let's see how you call JavaScript code from C++

Modify ```example.html``` to include and use jQuery

```html
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <title>Cheerp test</title>
    <script defer src="example.js"></script>
    <script src="http://ajax.googleapis.com/ajax/libs/jquery/1.7.1/jquery.min.js"></script>
    <script>
        // Use jQuery to make a (trivial) change to the page
        function changeTitle(str)
        {
                $("#pagetitle").text(str);
        }
    </script>
  </head>
  <body>
  <h1 id="pagetitle">Boring static text</h1>
  </body>
</html>
```

Let's use the JavaScript ```changeTitle``` function from C++

```c++
#include <cheerp/client.h>
#include <cheerp/clientlib.h>

// We need to extend the client namespace to declare our
// custom JavaScript function
namespace client
{
    // The name should be the same as the JavaScript one
    // The parameters needs to be a const client::String reference
    // so that implicit conversion from const char* is supported
    void changeTitle(const String& str);
}

using namespace client;

void webMain()
{
    Element* titleElement=document.getElementById("pagetitle");
    String* oldText=titleElement->get_textContent();
    changeTitle("Literal C++ string");
}
```

![](https://raw.githubusercontent.com/wiki/leaningtech/cheerp-wiki/Dom2.png)

When declaring JavsScript methods in C++ you can use the following data types:

* **client::String** as const references or pointers when using them as parameters and as pointers when returning them (e.g. ```client::String* concatStrings(const String& a, const String& b)```)
* **float**, **double** and integer types
* Pointers to C++ objects, to use them you still need to pass them back to C++ code.
* C++ lambdas and functions as callback parameters. You need to use the ```client::EventListener*``` as the parameter type. (e.g. ```void addCallback(client::EventListener* callback)```). In this case you need to use the ```cheerp::Callback``` bridge function as shown in the [example above](#callbackuse).

## Directly invoking JavaScript code

You can also use the ```__asm__``` keyword to inline JavaScript code in the middle of C++ code

```c++
void webMain()
{
    __asm__("console.log('Inline JS example')");
}
```

# Invoking C++ code from JavaScript using the ```[[jsexport]]``` attribute

Cheerp makes it possible to allocate and use whole C++ classes from JavaScript. This is done using the ```[[jsexport]]``` class attribute.

```c++
#include <cheerp/client.h>
#include <cheerp/clientlib.h>

using namespace client;

// The class can of course have any name
// The [[cheerp::jsexport]] attribute tells Cheerp to make
// the class available to JavaScript code
class [[cheerp::jsexport]] JsBridge
{
private:
    // The class is allowed to have member variables
    // but they should all be trivially destructible
    int callCount;
public:
    JsBridge():callCount(0)
    {
    }
    int addAndReturn(int a)
    {
        console.log("Called C++ code");
        callCount+=a;
        return callCount;
    }
};

// An entry point, even if empty, is still required
void webMain()
{
}
```

We also need to modify the HTML page to interact with the jsexport-ed class

```html
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <title>Cheerp test</title>
    <script defer src="example.js"></script>
    <script src="http://ajax.googleapis.com/ajax/libs/jquery/1.7.1/jquery.min.js"></script>
    <script>
        var jsBridge=null;
        function callCPPCode()
        {
            if(!jsBridge)
                jsBridge=new JsBridge();
            var ret=jsBridge.addAndReturn(3);
            $("#clickText").text("C++ code returned "+ret);
            return false;
        }
    </script>
  </head>
  <body>
  <a id="clickText" href="#" onclick="callCPPCode()">Click to call C++ code</a>
  </body>
</html>
```

As you can see you are free to allocate a new JsBridge object in JavaScript and use it directly.

# Using complex browser APIs

We will show how to use ```XMLHttpRequest``` to retrieve a file. Please note that Chrome blocks XMLHttpRequests to local files so you need to run the example using Firefox or start a local web server.

```c++
#include <cheerp/client.h>
#include <cheerp/clientlib.h>

using namespace client;

void webMain()
{
    XMLHttpRequest* xhr=new XMLHttpRequest();
    // Let's load the C++ source code for this program
    xhr->open("GET","example.cpp");
    // Use a C++11 lambda to show the result on the page
    xhr->set_onload(cheerp::Callback([](Event* e) {
          XMLHttpRequest* target=(XMLHttpRequest*)e->get_target();
          String* ret=target->get_responseText();
          Element* newParagraph=document.createElement("p");
          newParagraph->set_textContent(ret);
          document.get_body()->appendChild(newParagraph);
      }));
  xhr->send();
}
```

![](https://raw.githubusercontent.com/wiki/leaningtech/cheerp-wiki/xhr.png)
