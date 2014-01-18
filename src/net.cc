
#include "net.h"
#include "uv.h"

using namespace v8;

Persistent<Function> Net::constructor;

void Net::Init(Handle<Object> exports) {
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol("SimpleNet"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  // Prototype
  tpl->PrototypeTemplate()->Set(String::NewSymbol("connect"),
      FunctionTemplate::New(Connect)->GetFunction());
  constructor = Persistent<Function>::New(tpl->GetFunction());
  exports->Set(String::NewSymbol("SimpleNet"), constructor);
}

Handle<Value> Net::New(const Arguments& args) {
  HandleScope scope;

  if (args.IsConstructCall()) {
    Net* netobj = new Net();
    netobj->Wrap(args.This());
  }
  return args.This();
}

Net::Net() {
  // TODO
}

Net::~Net() {
  // TODO
}

Handle<Value> Net::Connect(const Arguments& args) {
  HandleScope scope;

  uv_tcp_t *handle = (uv_tcp_t *)malloc(sizeof(uv_tcp_t));
  uv_connect_t *sock = (uv_connect_t *)malloc(sizeof(uv_connect_t));
  struct sockaddr_in dest = uv_ip4_addr("163.177.65.209", 143);

  sock->data = handle;
  uv_tcp_init(uv_default_loop(), handle);
  uv_tcp_connect(sock, handle, dest, OnConnected);
  return args.This();
}

Handle<Value> Net::Connect(int port, String hostname) {
  HandleScope scope;
  return scope.Close(Null());
}

uv_buf_t Net::Alloc(uv_handle_t* handle, size_t size) {
  uv_buf_t buf;
  buf.base = (char*) malloc(size);
  buf.len  = size;
  return buf;
}

void Net::OnConnected(uv_connect_t *sock, int status) {
  HandleScope scope;

  uv_stream_t *handle = (uv_stream_t*)sock->data;
  uv_read_start(handle, Alloc, ReadConnection);
}

void Net::ReadConnection(uv_stream_t *handle, ssize_t nread, uv_buf_t buf) {
  printf("S: %s\n", buf.base);
  free(buf.base);
  uv_read_stop(handle);
}

NODE_MODULE(net, Net::Init);