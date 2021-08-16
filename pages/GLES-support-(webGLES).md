With Cheerp we ship a GLES2 implementation based on WebGL which is designed to jump start porting an existing GLES application/game to the browser.

# Usage

The GLES2 API is declared in the `GLES2/gl2.h` header file, while webGLES specific methods are declared in the `GLES2/webgles.h` header. webGLES needs to be initialized using the following function

```cpp
void webGLESInit(const client::String& canvasName);
```

which accept as the only argument the name of an existing HTML5 canvas element which should already exists at the time of webGLES initialization. An example HTML tag for such canvas:

```html
<canvas width=1024 height=768 id="glcanvas"></canvas>
```

# Raw WebGL access

`webGLESInit` creates a globally accessible WebGL context, you are free to use such context and you can mix and match between GLES and WebGL APIs. The context object is called `webGLES` and is accessible by including the `GLES2/webgles.h` header. For example:

```cpp
#include <GLES2/webgles.h>
...
webGLES->createShader(GL_VERTEX_SHADER);
```

Helpers methods are available to convert GLES2 resources id to resource objects as used by WebGL:

```cpp
client::WebGLProgram* webGLESLookupWebGLProgram(int objId);
client::WebGLShader* webGLESLookupWebGLShader(int objId);
client::WebGLBuffer* webGLESLookupWebGLBuffer(int objId);
client::WebGLFramebuffer* webGLESLookupWebGLFramebuffer(int objId);
client::WebGLRenderbuffer* webGLESLookupWebGLRenderbuffer(int objId);
client::WebGLTexture* webGLESLookupWebGLTexture(int objId);
client::WebGLUniformLocation* webGLESLookupWebGLUniformLocation(int objId);
```

Although webGLES implement the `glTexImage2D` API to load texture data, we only intend it to be used for data which is generated at run-time, like procedurally generated textures. We strongly advise using the WebGL provided `texImage2D` to load image data in JPEG or PNG format since the browser can decompress the images natively.