---
title: Cheerp memory model
---

Cheerp compiles at the same time to 2 diffent languages: either pure JavaScript or WebAssembly (or Wast or AsmJS). They come with 2 different memory models, JavaScript-like and WebAssembly-like. Understanding them is a key in understanding Cheerp approach, allowing to use Cheerp optimally.

WebAssembly-like or Linear Memory is the simpler to understand coming from the world of C++: there is an array of bytes, and every object is stored somewhere in that array. That maps fairly easily to the standard memory model, and allows the same kind of operations:
* accessing a random point of the array (both for writing and reading).
* pointers (that becomes just index into the array).

This model is relatively straightforward to transpile to, but has 2 main problems:
* it does not allow to pass objects to code that is not been compiled by us (think other JavaScript libraries, or the DOM, or browser resources)
* it is very brittle when it requires significant amount of memory in a single chunck (since the browsers are not optimized for this use-case)

Cheerp then works also with a second memory model: native JavaScript objects.
Cheerp maps each C++ object to a JavaScript object, allowing the created objects to be garbage collected AND to be used while interfacing with external JavaScript code.

Cheerp used with the standard command line options compiles the browser APIs (think console.log()) in JavaScript-like memory, while compiling all the rest in Linear Memory mode.
It is also possible to compile classes and functions with either memory model in a fine-grained way.

Here is a in-depth explanation of our unique JavaScript-like memory model.

# JavaScript types used in Cheerp

## JavaScript Objects

```js
var obj={x: 0, y: 1}
```
A JavaScript object is a map from strings (property names) to references. Objects do not have a definite type and the set of properties is allowed to change over time, although this is highly discouraged for performance reasons.

Cheerp uses JavaScript objects to represent struct and class instances, or more technically any instance of an LLVM StructType. Cheerp creates the objects and set all the properties at creation time since C++ does not allow them to change anyway. This is also the best from a JavaScript engine point of view to generate efficient code.

## Primitive values: Numbers

```js
var num = 3.4;
var int = 42;
```
In JavaScript there is a single numerical type: ```Number``` which is IEEE 64-bit floating point (aka double). Since a double has 52 bit of mantissa it's possible to store all valid 32-bit integers into a double. On the other hand it's not possible to represent the whole numeric range of 64-bit integers, which are supported by Cheerp using two 32-bit integers.

```Numbers``` instances are immutable, it's never possible to change the internal value of a ```Number```, it's only possible to overwrite the reference (e.g. ```var num``` in the example above) with a new value of Number.

## Primitive values: String

```js
var str = "SomeText";
````
JavaScript Strings are not used directly by Cheerp, but they are accessible to the user and are necessary to pass data to many DOM/HTML APIs. JavaScript Strings are encoded using UTF-16 (2 bytes per character) and are immutable. It's never possible to change a specific character of an existing string. ASCII strings used by C++ need to be slowly converted to String when necessary since they have a different memory layout (1 byte per character versus 2 bytes per character).

## JavaScript Array and TypedArrays

```js
var a = [{x: 0, y: 1},{x: 2, y: 3}]
var typedA = new Int32Array(10);
```
JavaScript arrays are used for various purposes by Cheerp, mostly to represent C++ arrays. They are mutable, as it is possible to change the contained values. They are also dynamic since it's possible to add and remove any element and even create holes. This behaviour is not internally used by Cheerp. Users have access to the ```Array``` type and are free to use it.

## Passing by reference

```js
var obj={x: 0, y: 1}
var num=42
var str="SomeText";
weirdFunc(obj,num,str);
```
In JavaScript all parameters are passed by reference. So inside ```weirdFunc``` it's possible to change the content of the passed objects, like ```obj```. Also ```num``` and ```str``` are passed by reference, but since they are immutable they can be considered more similar to const references or even as copies. The main effect is that a called function has no way to change the value of the number in a manner which is visible by the caller. This means that we need to use a trick to support C++ pointers to integers and floating point values.

# C++ pointers in Cheerp

C++ pointers need to support the following operations:

* Loading or storing a value from them
* Doing pointer arithmetic, which is fully defined by the standard only for C++ arrays
* Comparing for equality
* Ordering, which is fully defined for C++ arrays and left unspecified for unrelated objects

JavaScript references support equality comparison, but they have no ordering and do not support any arithmetic. Moreover, although Numbers are passed by reference as well, they are immutable and behave like being passed by copy.

```js
var a = [{x: 0, y: [1]},{x: 2, y: [3]}];
var typedA = new Int32Array(10);
var objPtr = {d: a, o: 1};
var objMemberPtr = {d: a[1].y, o: 0};
var numPtr = {d: typedA, o: 3};
```
The solution is to conceptually represent pointers as a pair of a container object and an offset into the object. If the container object is not an array an additional wrapper array must be created around the member. This solution provide:

* Pointer arithmetic, by operating on the offset field
* Pointer ordering in arrays, by comparing the offset field
* Read/Write access to numerical values, by accessing the container object at the right offset

This solution is also inefficient if used every time. Moreover wrapper arrays are required for pointer-to-members that escapes an object and top-level objects created on the stack or using ```new```.

It's possible to remove this overhead in most cases by analyzing code at compile time. The idea is: if we observe that the pointer is not used for pointer arithmetic, but only used to load or store data or to access members, it's possible to use it directly as a JavaScript object.

# Objects kinds

Pointers which are not used for pointer arithmetics can be represented as pure JavaScript references and are as cheap as regular JavaScript objects.  This style of handling pointers is marked as ```COMPLETE_OBJECT``` in the Cheerp compiler.

```js
var obj1={x: 0, y: 1};
// func1 does not do pointer arithmetic on the passed pointer
func1(obj1);
var obj2=[{x: 0, y: 1}];
//func2 does use pointer arithmetic on the passed pointer
func2(obj2, 0);
```

If a pointer does not fall in the ```COMPLETE_OBJECT``` category then is represented as a pair of an object and an offset. Those pointers are called ```REGULAR```. The Cheerp compiler tries very hard to avoid creating these object in two ways:

* If a given pointer can be proven to always have the same offset, only the container is stored
* In most cases instead of creating a JavaScript object with two members we can store the container and offset as separate variables.

# Optimizations

What we want to do is to inspect code at compile time to be able to avoid using ```REGULAR``` pointers as much as possible. The idea is that We can avoid creating a pointer temporary object if a pointer is:

* Not used for arithmetic
* Only directly loaded
* Only used to access members

This property needs to be computed for pointers being passed to every method to be able to propagate it correctly through the call graph. It is also necessary to take special care for virtual methods and indirect method calls. Still, since Cheerp works as an whole program optimizer and it has full knowledge about the call graph it is possible to use this optimizations in most contexts.
