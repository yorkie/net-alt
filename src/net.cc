
#include "net.h"
#include "uv.h"

using namespace std;
using namespace v8;

Persistent<Function> Net::constructor;
Persistent<Function> Net::callback;

void Net::Init(Handle<Object> exports) {
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol("SimpleNet"));
  tpl->InstanceTemplate()->SetInternalFieldCount(3);
  // Prototype
  tpl->PrototypeTemplate()->Set(String::NewSymbol("connect"),
      FunctionTemplate::New(Connect)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("write"),
      FunctionTemplate::New(Write)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("end"),
      FunctionTemplate::New(End)->GetFunction());
  constructor = Persistent<Function>::New(tpl->GetFunction());
  exports->Set(String::NewSymbol("SimpleNet"), constructor);
}

Handle<Value> Net::New(const Arguments& args) {
  HandleScope scope;
  int port;
  string hostname;

  if (!args[0]->IsNumber() || !args[1]->IsString())
    return ThrowTypeError("Bad parameter");
  
  v8::String::Utf8Value phost(args[1]->ToString());
  port = args[0]->Int32Value();
  hostname = string(*phost);

  if (args.IsConstructCall()) {
    Net* netobj = new Net(args.This(), hostname, port);
    netobj->Wrap(args.This());
  }
  return args.This();
}

Net::Net(Handle<Object> object, string hostname, int port) {
  object_ = v8::Persistent<Object>::New(object);
  hostname_ = const_cast<char*>(hostname.c_str());
  port_ = port;
}

Net::Net() {
  // TODO
}

Net::~Net() {
  // TODO
}

Handle<Value> Net::Connect(const Arguments& args) {
  HandleScope scope;
  callback = Persistent<Function>::New(Handle<Function>::Cast(args[0]));

  Net *netobj = Unwrap<Net>(args.This());
  netobj->handle_ = (uv_tcp_t *)malloc(sizeof(uv_tcp_t));
  netobj->socket_ = (uv_connect_t *)malloc(sizeof(uv_connect_t));
  struct sockaddr_in dest = uv_ip4_addr(netobj->hostname_, netobj->port_);

  netobj->socket_->data = netobj;
  netobj->handle_->data = netobj;
  uv_tcp_init(uv_default_loop(), netobj->handle_);
  uv_tcp_connect(netobj->socket_, netobj->handle_, dest, AfterConnection);
  return args.This();
}

Handle<Value> Net::Write(const Arguments& args) {
  HandleScope scope;
  return args.This();
}

Handle<Value> Net::End(const Arguments& args) {
  HandleScope scope;
  return args.This();
}

uv_buf_t Net::Alloc(uv_handle_t* handle, size_t size) {
  uv_buf_t buf;
  buf.base = (char*) malloc(size);
  buf.len  = size;
  return buf;
}

void Net::AfterConnection(uv_connect_t *socket, int status) {
  HandleScope scope;
  Net *net_wrap = static_cast<Net*>(socket->data);
  uv_read_start((uv_stream_t*)net_wrap->handle_, Alloc, ReadConnection);
}

void Net::ReadConnection(uv_stream_t *handle, ssize_t nread, uv_buf_t buf) {
  HandleScope scope;
  Net *net_wrap = static_cast<Net*>(handle->data);

  buf.base[nread] = 0;
  Local<Value> argv[2] = {
    Integer::New(0),
    String::New(buf.base)
  };
  MakeCallback(net_wrap->object_, Handle<Function>::Cast(callback), 2, argv);

  free(buf.base);
  uv_read_stop(handle);
}

NODE_MODULE(net, Net::Init);
