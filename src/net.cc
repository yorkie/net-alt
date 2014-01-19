
#include "net.h"
#include "uv.h"

using namespace std;
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
  int port;
  string hostname;

  if (!args[0]->IsNumber() || !args[1]->IsString())
    return ThrowTypeError("Bad parameter");
  
  v8::String::Utf8Value phost(args[1]->ToString());
  port = args[0]->Int32Value();
  hostname = string(*phost);

  if (args.IsConstructCall()) {
    Net* netobj = new Net(port, hostname);
    netobj->Wrap(args.This());
  }
  return args.This();
}

Net::Net(int port, string hostname) {
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

  Net *netobj = Unwrap<Net>(args.This());
  netobj->handle_ = (uv_tcp_t *)malloc(sizeof(uv_tcp_t));
  netobj->socket_ = (uv_connect_t *)malloc(sizeof(uv_connect_t));
  struct sockaddr_in dest = uv_ip4_addr(netobj->hostname_, netobj->port_);

  netobj->socket_->data = netobj->handle_;
  uv_tcp_init(uv_default_loop(), netobj->handle_);
  uv_tcp_connect(netobj->socket_, netobj->handle_, dest, OnConnected);
  return args.This();
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
  free(buf.base);
  uv_read_stop(handle);
}

NODE_MODULE(net, Net::Init);
