---
title: Interoperability with Web frameworks
---

Let's say, for the sake of example, that you have a JavaScript function that expects the following syntax:

```js
jsFuncWithObjectParam({requiredParam1:42, requiredParam2:"String", optionalParam1:null});
```

In C++ you can do the following:

```cpp
namespace client
{
struct ParamObject
{
    void set_requiredParam1(int);
    void set_requiredParam2(const client::String&);
    void set_optionalParam1(client::Object*);
};

void jsFuncWithObjectParam(ParamObject*);
};

void webMain()
{
    client::ParamObject* p = new client::ParamObject();
    p->set_requiredParam1(42);
    p->set_requiredParam2("String");
    //p->set_optionalParam1(nullptr);
    client::jsFuncWithObjectParam(p);
}
```

We recommend this solution since it's more type safe. You can also use the shorter code example:

```cpp
client::Object* p = new client::Object();
p->set_("requiredParam1",new client::Number(42));
p->set_("requiredParam2",new client::String("String"));
p->set_("optionalParam1",nullptr);
```
