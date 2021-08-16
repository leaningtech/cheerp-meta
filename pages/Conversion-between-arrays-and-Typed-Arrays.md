---
title: Conversion between arrays and Typed Arrays
---

In cheerp, C++ arrays of basic data types such as ```char```, ```unsigned char```, ```short```, ```unsigned short```, ```int```, ```unsigned int```, ```float``` and ```double``` are backed by JavaScript typed array of corresponding types.

# From C++ data to Typed Array

Cheerp provides a few APIs to convert C++ arrays to the underlying Typed Array objects. This is especially useful to pass C++ data directly to browser APIs such as WebGL.

## TypedArrayForPointerType
```c++
template<typename T> struct TypedArrayForPointerType;
```
Template helper to get the corresponding Typed Array type for a specific C data type. The desired type is available as the ```::type``` member. For example

```c++
TypedArrayForPointerType<unsigned short>::type
```
The following mapping applies:
* **char** -> Int8Array
* **unsigned char** -> Uint8Array
* **short** -> Int16Array
* **unsigned short** -> Uint16Array
* **int** -> Int32Array
* **unsigned int** -> Uint32Array
* **float** -> Float32Array
* **double** -> Float64Array

## MakeTypedArray

There are two implementations of this function. Both accept two arguments: a pointer to an array of any of the aforementioned data types, and an optional size argument. The default value is 0, meaning "until the end of the underlying array". The returned value is a pointer to a typed array instance.

**CAUTION:** The returned typed array references the same memory as the passed pointer, no copies are done to guarantee the efficiency of this operation. Please be careful when modifying the memory as the compiler may or may not understand that the typed array alias the same memory as the pointer. This function is designed to pass C++ data to browser APIs with low overhead.

### Implicitly typed version

```c++
template<typename P,typename T=typename TypedArrayForPointerType<P>::type>
T* MakeTypedArray(const P* ptr, size_t size=0)
```

This function returns a typed array for the underlying memory of the ```ptr``` pointer. The return type depends on the type of ```ptr``` and follows the mapping of the ```TypedArrayForPointerType``` template helper.

### Explicitly typed version

```c++
template<typename T>
T* MakeTypedArray(const void* ptr, size_t size=0)
```

This function returns a typed array for the underlying memory of the ```ptr``` pointer, the return type must be explicitly provided by the user. Any pointer type can be passed as ```ptr```, but the result is undefined behavior if the underlying memory is not an array of a basic data type.

## MakeArrayBufferView

```c++
client::ArrayBufferView* MakeArrayBufferView(const void* ptr, size_t size=0)
```

Returns an untyped ArrayBufferView of the underlying memory of the ```ptr``` pointer. No data copy is performed and the returned object alias the same memory as the ```ptr``` pointer. The safety considerations about memory aliasing which are valid for ```MakeTypedArray``` are valid for ```MakeArrayBufferView``` as well.

# From Typed Array to C++ data

Although Cheerp does not provide an explicit API to achieve this you can directly use an [intrinsic](https://github.com/leaningtech/cheerp-meta/wiki/Cheerp-intrinsics). For example:

```
// Offset might be 0, you can use it create a pointer to a specific element of the typed array
float* createData(client::Float32Array* a, int offset)
{
    return __builtin_cheerp_make_regular<float>(a, offset);
}
```

Make sure to always use the corresponding C++ data type for the typed array, (see the mapping above). In particular, mismatching the signed-ness of integers (e.g. creating a ```unsigned char*``` from a ```client::Int8Array*```) will cause undefined behavior.
