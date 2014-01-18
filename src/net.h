
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

  static uv_buf_t Alloc(uv_handle_t* handle, size_t size);
  static void OnConnected(uv_connect_t *socket, int status);
  static void ReadConnection(uv_stream_t *handle, ssize_t nread, uv_buf_t buf);
};

#endif