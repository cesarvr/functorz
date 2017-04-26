#include "nan.h"
#include <iostream>
#include <vector>
#include <thread>


#include "helpers.hpp"

using namespace std;
using namespace v8;

using LocalValue = v8::Local<v8::Value>;
using LocalArray = v8::Local<v8::Array>;
using LocalFunction =  v8::Local<v8::Function>;
using Arguments = const Nan::FunctionCallbackInfo<v8::Value>&;


const string COMPILE_ERROR = "Error compiling: ";

// Just a helper function to log v8 MaybeValues.
template <typename T>
void V8Log(T& maybeValue){



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



template <typename T>
Local<T>
compileScript(const Nan::FunctionCallbackInfo<v8::Value>& args) {
   
    Local<Value> value;
    ScriptCompiler::Source source(args[0]->ToString());
    
    TryCatch try_catch(args.GetIsolate());
    auto script = ScriptCompiler::Compile(Nan::GetCurrentContext(), &source);
    
    if(script.IsEmpty()) {
        args.GetReturnValue().SetUndefined();
       
        auto msg = Helper::getMessageFromTry(COMPILE_ERROR, &try_catch);
        
        Nan::ThrowError(msg.c_str());
        return value.As<T>();
    }
    
    auto compiled = script
                .ToLocalChecked()->Run(Nan::GetCurrentContext());

    compiled.FromMaybe(value);
    
    return value.As<T>();
}

void toFunction(const Nan::FunctionCallbackInfo<v8::Value>& args) {
    
    Local<Function> func = compileScript<Function>(args);
    
    if(func.IsEmpty()){
        args.GetReturnValue().SetUndefined();
    }else {
        args.GetReturnValue().Set(func);
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
                 Nan::New<v8::FunctionTemplate>(toFunction)->GetFunction());

  exports->Set(Nan::New("run").ToLocalChecked(),
                 Nan::New<v8::FunctionTemplate>(RunIt)->GetFunction());


}

NODE_MODULE(hello, Init)
