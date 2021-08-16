---
title: WebGL Troubleshooting
---

## Passing data to WebGL uniform* APIs

```uniform*``` APIs and, more in general, WebGL APIs which expects buffer requires typed arrays of a suitable type.

If you have data stored in a C/C++ array or ```std::vector``` or any other continguos memory storage, you can use the [```cheerp::MakeTypedArray```](https://github.com/leaningtech/cheerp-wiki/wiki/Conversion-between-arrays-and-Typed-Arrays#maketypedarray) to convert the data into a JS typed array object. Please note that the conversion happens without copying, so it's very efficient.
