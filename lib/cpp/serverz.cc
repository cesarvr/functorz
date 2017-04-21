#include "nan.h"
#include <iostream>
#include <vector>
#include <thread>
#include <ctime>


using namespace std;
using LocalValue = v8::Local<v8::Value>;
using LocalArray = v8::Local<v8::Array>;
using LocalFunction =  v8::Local<v8::Function>;


struct Jail {
    
    template <typename Caller>
    auto call(Caller& fn) -> decltype(fn()){
        start = std::clock();
        auto ret = fn();
        duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
        std::cout<<"printf: "<< duration <<'\n';
        
        return ret;
    }
    
private:
    std::clock_t start;
    double duration;
};

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

template <typename v8Arguments, typename v8Value>
void returnToJavascript(v8Arguments& v8argument, v8Value&& value, string message = "result"){
    
    auto jsRetObject = Nan::New<v8::Object>();
    jsRetObject->Set(Nan::New(message).ToLocalChecked(), value);
    
    v8argument.GetReturnValue().Set(jsRetObject);
}


void Compile(const Nan::FunctionCallbackInfo<v8::Value>& args) {
    
    cout << "compiling...." << endl;
    cout << "===================" << endl;
    
    v8::ScriptCompiler::Source source(args[0]->ToString());
    auto script = v8::ScriptCompiler::Compile(Nan::GetCurrentContext(), &source);
   
    if(script.IsEmpty()) {
        args.GetReturnValue().SetUndefined();
    }else{
        // Create a new context.
        auto value = script.ToLocalChecked()->Run(Nan::GetCurrentContext());
        
        WhatIsThis(value);
    
        if(value.IsEmpty())
            args.GetReturnValue().SetUndefined();
        else
            returnToJavascript(args, value.ToLocalChecked());
    }
}

void RunIt(const Nan::FunctionCallbackInfo<v8::Value>& arg){
    if(arg[0]->IsFunction() && arg[1]->IsArray()) {
        
        cout << " Initializing " << endl;
        LocalFunction fx =  arg[0].As<v8::Function>();
        auto argz =  arg[1].As<v8::Array>();
        
        int argc = argz->Length();
        auto _argv = unique_ptr<LocalValue>(new LocalValue[3]);
        
        cout << " Initializing (end)" << endl;
        
        cout << " Transforming V8::Array to vector" << endl;
        
        for(int i=0; i< argc; i++) {
       
            cout << "stuff in vector: " << argz->Get(i)->ToInteger()->Value() << endl;
            _argv.get()[i] = argz->Get(i);
        
        }
        
        cout << " Transforming V8::Array to vector (end)" << endl;
        cout << " Locking v8" << endl;
        v8::Locker locker(Nan::GetCurrentContext()->GetIsolate());
        cout << " Locking v8, it Will be released at the end (end)" << endl;
        
        cout << " creating Lambda " << endl;
      
        auto fn = [&fx, &argc, &_argv](){
            cout << "executing...." << endl;
            LocalValue rec = Nan::New("Hellow").ToLocalChecked();
            
            cout << "calling..." << endl;
            return fx->Call(Nan::GetCurrentContext(),
                            rec, argc, _argv.get());
        };
        
      
    
        Jail jail;
        
        auto ret = jail.call(fn);
        
        /*
        
        auto ret = fx->Call(Nan::GetCurrentContext(),
                  rec, argc, _argv.get());
        
         */
        
        cout << " creating Lambda (end) " << endl;
        
        cout << " running in new thread " << endl;
    
        /*
        thread th{fn};
        th.join();
        */
        cout << " running in new thread (end)" << endl;
        
        cout << " Calling Method (end) " << endl;
        
        if(ret.IsEmpty()) {
             arg.GetReturnValue().SetUndefined();
        } else {
            arg.GetReturnValue().Set(ret.ToLocalChecked());
        }
    }else
        Nan::ThrowError("Missing Parameters: run(Function, Array) signature is expected.");
}



void Init(v8::Local<v8::Object> exports) {
    
  exports->Set(Nan::New("compile").ToLocalChecked(),
                 Nan::New<v8::FunctionTemplate>(Compile)->GetFunction());
    
  exports->Set(Nan::New("run").ToLocalChecked(),
                 Nan::New<v8::FunctionTemplate>(RunIt)->GetFunction());
    
    
}

NODE_MODULE(hello, Init)
