
#ifndef __ALT_NET_H__
#define __ALT_NET_H__

#include <iostream>
#include <v8.h>
#include <uv.h>
#include <node.h>
#include <node_object_wrap.h>
#include <node_internals.h>

using namespace std;
using namespace node;
using namespace v8;

class Net : public ObjectWrap {

public:
  static void Init(Handle<Object> exports);
  static Handle<Value> NewInstance(const Arguments& args);

private:
  Net(string hostname, int port);
  Net();
  ~Net();

  char *hostname_;
  int   port_;

  uv_tcp_t *uv_handle_;
  uv_connect_t *uv_socket_;

  static Handle<Value> Connect(const Arguments& args);
  static Handle<Value> Write(const Arguments& args);
  static Handle<Value> End(const Arguments& args);

  static Persistent<Function> constructor;
  Persistent<Function> callback_;
  Persistent<Object> object_;

  static uv_buf_t Alloc(uv_handle_t* handle, size_t size);
  static void AfterConnection(uv_connect_t *socket, int status);
  static void AfterWrite(uv_write_t *writer, int status);
  static void AfterEnd(uv_handle_t *handle);
  static void Read(uv_stream_t *handle, ssize_t nread, uv_buf_t buf);

};

#endif