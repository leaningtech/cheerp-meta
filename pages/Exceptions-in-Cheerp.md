---
title: Exceptions (C++ and JavaScript) in Cheerp
---

Cheerp supports C++ exceptions, enabled when passing `-fexceptions` to the compiler. Cheerp also supports dealing with JavaScript exceptions, whether catching JavaScript exceptions raised by external JavaScript code, or throwing JavaScript exceptions from C++ code.

'Internal' C++ exception caught by C++ code
---

This is the simplest case of using exceptions in Cheerp. In this case, your C++ code behaves in the same way that it would do when compiling natively, and any exceptions-enabled C++ code will work out of the box.

Example: 
```c++
#include <iostream>
using namespace std;

int main () {
  try
  {
    throw 20;
  }
  catch (int e)
  {
    cout << "An exception occurred. Exception Nr. " << e << '\n';
  }
  return 0;
}
```

C++ exceptions caught in C++ work as expected, whether compiling to WebAssembly, JavaScript, or both. As long as both sides are present on the C++ side it will follow the 'regular' rules for C++ exceptions, (eg. catch(...) included).

'External' exception caught by C++ code
---

In this case, C++ code invokes some JavaScript method that, in turn, generates an exception. The exception is caught in C++.

Example:
```c++
#include <iostream>
#include <cheerp/jsexception.h>
#include <cheerp/clientlib.h>
using namespace std;

namespace client{
    void someUndefinedExternalFunc();
}

int main () {
  try
  {
          client::someUndefinedExternalFunc();
  }
  catch (cheerp::JSException& ex)
  {
          client::Error* error = (client::Error*)ex.get();
          client::console.log(error->get_name());
          client::console.log(error->get_stack());
          client::console.log(error->get_message());
  }
  return 0;
}
```

Here say that we don't implement `someUndefinedExternalFunc`. Executing it will throw a JavaScript Error, that can be caught using the cheerp-specific syntax:

`catch (cheerp::JSException& ex)`

Note that using the throw value requires: calling `get()` + casting to the appropriate value.

Here we casted to a pointer to `client::Error`, but see for example here: [cheerp-utils/tests](https://github.com/leaningtech/cheerp-utils/blob/0b3bce1a73be2eec5437df3fa4747f954863c0c7/tests/unit/exceptions/test1.cpp#L381) for catching an integer value.

'Internal' (C++) exception catched by external JavaScript code
---

If your code (eg. a `jsexport`ed function) lets a C++ exception bubble up, you can still catch it on the JavaScript side by wrapping the call of the relevant jsexported-method in a `try {} catch {}` statement. This is 'just' regular JavaScript.

```c++
[[cheerp::jsexport]]
void throwIfOdd(int n)
{
  if (n % 1)
    throw n;
}
```
```js
try{
  throwIfOdd(3);
}
catch(e){
  console.log("Catched C++ exception ", e);
}
```

This is expecially useful during development.


Throw builtin
---

Also possibly useful for testing (and for internal usage) there is a `__builtin_cheerp_throw( someObj )` that codegen to the equivalent JavaScript throw.

```c++
#include <cheerpintrin.h>
client::Object* obj = new client::Object();

__builtin_cheerp_throw(obj);
```

Throws via the builtin behave exactly as the object was thrown via the JavaScript throw mechanism (so it will be considered as 'external' by the Cheerp compiler).



`__asm__` sections can't throw
---

This is the very last 'catch', there can't be throws (either directly or indirectly) in `__asm__` sections (see [asm sections](JavaScript-interoperability.md)).
