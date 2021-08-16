---
title: Custom allocators support
---

One common issue when porting code to Cheerp is the lack of support for custom allocators that return void pointers.

Let's take this code for example:
```
void* myalloc(size_t s)
{
    return malloc(s);
}
```

Cheerp will emit a compile time error:
```
Cheerp: Legacy C memory allocation must be immediately casted to the actual type. Any other use of the return value is invalid
```
The problem is that allocations in cheerp are translated to the creation of a JS object with the right set of properties. This code:
```
class Foo {
    int var1;
    int var2;
};
...
Foo foo* = (Foo*)malloc(sizeof(Foo));
```
translates to the following JS code:
```
tmp0={i0:0, i1:0};
```
As you can see, the type information provided by the cast operator is needed to provide the right object layout. An equivalent (and better) solution is to use the C++ `new` operator whenever possible.

If for some reason you really need the custom allocator function, consider using a template:
```
template<class T>
T* myalloc(size_t size)
{
        return (T*)malloc(size);
}
...
Foo* foo = myalloc<Foo>(sizeof(Foo));
```
