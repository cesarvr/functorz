#include "nan.h"
#include <iostream>

using namespace std;

template <typename T>
void WhatIsThis(T& value){
    cout << "Is empty: " << value->IsNull() << endl;
    cout << "Is function: " << value->IsFunction() << endl;
    cout << "Is error: " << value->IsNativeError() << endl;
    cout << "Is object: " << value->IsObject() << endl;
    cout << "Is error: " << value->IsNativeError() << endl;
    
}

void Method(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  info.GetReturnValue().Set(Nan::New("world").ToLocalChecked());
}

void Compile(const Nan::FunctionCallbackInfo<v8::Value>& args) {
   // v8::Script::Compile(args[0]->ToString());
  
    // Create a new context.
    v8::Local<v8::Context> _ctx = v8::Context::New(args.GetIsolate()); 

    v8::ScriptCompiler::Source source(args[0]->ToString());
    auto script = v8::ScriptCompiler::Compile(args.GetIsolate(), &source);
    auto object = Nan::New<v8::Object>();
    
    auto value = script->Run(_ctx).ToLocalChecked();
    
    WhatIsThis(value);
    
    object->Set(Nan::New("result").ToLocalChecked(), value);
    args.GetReturnValue().Set(object);
}


void Init(v8::Local<v8::Object> exports) {
  exports->Set(Nan::New("hello").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(Method)->GetFunction());
    
  exports->Set(Nan::New("compile").ToLocalChecked(),
                 Nan::New<v8::FunctionTemplate>(Compile)->GetFunction());
}

NODE_MODULE(hello, Init)
