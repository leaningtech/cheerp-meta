// cheerpworker.cpp: Code that run inside the worker
#include <cheerp/clientlib.h>
#include <cheerp/client.h>

using namespace client;

void webMain()
{
       addEventListener("message", cheerp::Callback([](MessageEvent* e) {
			       postMessage(e->get_data());
			       postMessage(e->get_data());
			       }));
}
