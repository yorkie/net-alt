
#ifndef __ALT_NET_H__
#define __ALT_NET_H__

#include <stdio.h>
#include <v8.h>
#include <uv.h>
#include <node.h>
#include <node_object_wrap.h>

using namespace node;
using namespace v8;

class Net : public ObjectWrap {

public:
  static void Init(Handle<Object> exports);
  static Handle<Value> New(const Arguments& args);

private:
  Net();
  ~Net();

  static Handle<Value> Connect(const Arguments& args);
  static Handle<Value> Connect(int port, String hostname);
  static Persistent<Function> constructor;

  static void OnConnected(uv_connect_t *socket, int status);

};

#endif