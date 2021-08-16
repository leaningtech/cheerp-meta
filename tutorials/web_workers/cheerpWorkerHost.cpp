// cheerpworkerhost.cpp: Code to include in the HTML page
#include <cheerp/client.h>
#include <cheerp/clientlib.h>

using namespace client;

void webMain()
{
        Worker* w = new Worker("cheerpWorker.js");
        w->addEventListener("message", cheerp::Callback([](MessageEvent* e) {
                                        console.log((String*)(e->get_data())); }));
        w->postMessage("Hello World");
}
