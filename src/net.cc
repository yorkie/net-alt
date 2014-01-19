
#include "net.h"
#include "uv.h"

using namespace std;
using namespace v8;

Persistent<Function> Net::constructor;

void Net::Init(Handle<Object> exports) {
  Local<FunctionTemplate> tpl = FunctionTemplate::New(Net::NewInstance);
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

Handle<Value> Net::NewInstance(const Arguments& args) {
  HandleScope scope;
  int port;
  string hostname;

  if (!args[0]->IsNumber() || !args[1]->IsString())
    return ThrowTypeError("Bad parameter");
  
  v8::String::Utf8Value phost(args[1]->ToString());
  port = args[0]->Int32Value();
  hostname = string(*phost);

  if (args.IsConstructCall()) {
    Net* net_wrap = new Net(hostname, port);
    net_wrap->Wrap(args.This());
    net_wrap->object_ = Persistent<Object>::New(args.This());
    return args.This();
  } else {
    return Undefined();
  }
}

Net::Net(string hostname, int port) {
  hostname_ = const_cast<char*>(hostname.c_str());
  port_ = port;
}

Net::Net() {
  // TODO
}

Net::~Net() {
  delete uv_handle_;
  delete uv_socket_;
}

Handle<Value> Net::Connect(const Arguments& args) {
  HandleScope scope;

  Net *net_wrap = Unwrap<Net>(args.This());
  net_wrap->uv_handle_ = (uv_tcp_t *)malloc(sizeof(uv_tcp_t));
  net_wrap->uv_socket_ = (uv_connect_t *)malloc(sizeof(uv_connect_t));
  net_wrap->callback_ = Persistent<Function>::New(Handle<Function>::Cast(args[0]));
  struct sockaddr_in dest = uv_ip4_addr(net_wrap->hostname_, net_wrap->port_);

  net_wrap->uv_socket_->data = net_wrap;
  net_wrap->uv_handle_->data = net_wrap;
  uv_tcp_init(uv_default_loop(), net_wrap->uv_handle_);
  uv_tcp_connect(net_wrap->uv_socket_, net_wrap->uv_handle_, dest, AfterConnection);
  return args.This();
}

Handle<Value> Net::Write(const Arguments& args) {
  HandleScope scope;
  Net *net_wrap = Unwrap<Net>(args.This());

  if (args.Length() == 0)
    ThrowTypeError("Bad Arguments");

  v8::String::Utf8Value pdata(args[0]->ToString());
  string data = string(*pdata);

  uv_buf_t buf = uv_buf_init(const_cast<char*>(data.c_str()), data.length());
  uv_write_t *writer = (uv_write_t *)malloc(sizeof(uv_write_t));
  writer->data = net_wrap;
  uv_write(writer, (uv_stream_t*)net_wrap->uv_handle_, &buf, 1, AfterWrite);

  return args.This();
}

Handle<Value> Net::End(const Arguments& args) {
  HandleScope scope;
  Net *net_wrap = Unwrap<Net>(args.This());

  uv_close((uv_handle_t*)net_wrap->uv_handle_, AfterEnd);
  return Undefined();
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
  uv_read_start((uv_stream_t*)net_wrap->uv_handle_, Alloc, Read);
}

void Net::AfterWrite(uv_write_t *writer, int status) {
  HandleScope scope;
  Net *net_wrap = static_cast<Net*>(writer->data);
  free(writer);

  uv_read_start((uv_stream_t*)net_wrap->uv_handle_, Alloc, Read);
}

void Net::AfterEnd(uv_handle_t *handle) {
  HandleScope scope;

  Net* net_wrap = static_cast<Net*>(handle->data);
  net_wrap->handle_->SetPointerInInternalField(0, NULL);
  net_wrap->handle_.Dispose();
  net_wrap->handle_.Clear();

  delete net_wrap;
}

void Net::Read(uv_stream_t *handle, ssize_t nread, uv_buf_t buf) {
  HandleScope scope;
  Net *net_wrap = (Net*) handle->data;

  buf.base[nread] = 0;
  Local<Value> argv[2] = {
    Integer::New(0),
    String::New(buf.base)
  };
  MakeCallback(net_wrap->object_, 
    Handle<Function>::Cast(net_wrap->callback_), 2, argv);

  free(buf.base);
  uv_read_stop(handle);
}

NODE_MODULE(net, Net::Init);
