#include "nan.h"
#include <iostream>

using namespace std;

template <typename T>
void WhatIsThis(T& maybeValue){
    
    cout << "checking value type" << endl;
    cout << "===================" << endl;
    
    if(!maybeValue.IsEmpty()) {
        auto value = maybeValue.ToLocalChecked();
        cout << "Is empty: " << value->IsNull() << endl;
        cout << "Is function: " << value->IsFunction() << endl;
        cout << "Is error: " << value->IsNativeError() << endl;
        cout << "Is object: " << value->IsObject() << endl;
        cout << "Is error: " << value->IsNativeError() << endl;
    }else
        cout << "the value is null: maybe compiler error or empty source code" << endl;
}

void Method(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  info.GetReturnValue().Set(Nan::New("world").ToLocalChecked());
}

template <typename v8Arguments, typename v8Value>
void returnToJavascript(v8Arguments& v8argument, v8Value&& value, string message = "result"){
    
    auto jsRetObject = Nan::New<v8::Object>();
    jsRetObject->Set(Nan::New(message).ToLocalChecked(), value);
    
    v8argument.GetReturnValue().Set(jsRetObject);
}


void Compile(const Nan::FunctionCallbackInfo<v8::Value>& args) {
    
    v8::ScriptCompiler::Source source(args[0]->ToString());
    auto script = v8::ScriptCompiler::Compile(Nan::GetCurrentContext(), &source);
    //auto script = v8::ScriptCompiler::CompileUnboundScript(args.GetIsolate(), &source);

    if(script.IsEmpty()) {
        args.GetReturnValue().SetUndefined();
    }else{
        // Create a new context.
      //auto value = script.ToLocalChecked()->BindToCurrentContext()->Run(Nan::GetCurrentContext());
        auto value = script.ToLocalChecked()->Run(Nan::GetCurrentContext());
        
        WhatIsThis(value);
    
        if(value.IsEmpty())
            args.GetReturnValue().SetUndefined();
        else
            returnToJavascript(args, value.ToLocalChecked());
    }
}


void Init(v8::Local<v8::Object> exports) {
  exports->Set(Nan::New("hello").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(Method)->GetFunction());
    
  exports->Set(Nan::New("compile").ToLocalChecked(),
                 Nan::New<v8::FunctionTemplate>(Compile)->GetFunction());
}

NODE_MODULE(hello, Init)
