Cheerp includes a wrapper script that sets up a proper environment to let autoconf-based (i.e. configure) build systems work with Cheerp. 

Let's consider the common build instructions:

```
./configure --prefix=/some/system/path
make
```
If you have Cheerp installed on your system you can take advantage of cross compilation support by using the ```--host``` command line switch and the ```cheerpwrap``` script.

```
/opt/cheerp/bin/cheerpwrap ./configure --prefix=/some/system/path --host=cheerp-unknown-none
/opt/cheerp/bin/cheerpwrap make
```

The ```--host=cheerp-unknown-none``` option tells configure that you are doing a cross compilation build, so that the system will use prefixed tools such as ```cheerp-unknown-none-gcc``` to compile the project. The ```cheerpwrap``` script sets environment variable such as ```PATH``` to make such executables available to ```configure``` and ```make```. The prefixed executables are installed by default in the ```/opt/cheerp/libexec/``` directory.