
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

  uv_tcp_t *socket = (uv_tcp_t *)malloc(sizeof(uv_tcp_t));
  uv_connect_t *conn = (uv_connect_t *)malloc(sizeof(uv_connect_t));
  struct sockaddr_in dest = uv_ip4_addr("163.177.65.209", 143);

  uv_tcp_init(uv_default_loop(), socket);
  uv_tcp_connect(conn, socket, dest, OnConnected);
  return args.This();
}

Handle<Value> Net::Connect(int port, String hostname) {
  HandleScope scope;
  return scope.Close(Null());
}

void Net::OnConnected(uv_connect_t *socket, int status) {
  printf("connected\n");
}


NODE_MODULE(net, Net::Init);