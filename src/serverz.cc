#include "nan.h"
#include <iostream>
#include <vector>
#include <thread>


#include "helpers.hpp"

using namespace std;
using namespace v8;


using LocalArray = v8::Local<v8::Array>;
using LocalFunction =  v8::Local<v8::Function>;
using Arguments = const Nan::FunctionCallbackInfo<v8::Value>&;


const string COMPILE_ERROR = "Error compiling: ";


template <typename T>
Local<T>
compileScript(Arguments args) {

    Local<Value> value;
    ScriptCompiler::Source source(args[0]->ToString());

    TryCatch try_catch(args.GetIsolate());
    auto script = ScriptCompiler::Compile(Nan::GetCurrentContext(), &source);

    if(script.IsEmpty()) {
        args.GetReturnValue().SetUndefined();

        throw Helper::getMessageFromTry(COMPILE_ERROR, &try_catch);
    }

    auto compiled = script
                .ToLocalChecked()->Run(Nan::GetCurrentContext());

    value = compiled.ToLocalChecked().As<T>();

    return value.As<T>();
}

void toFunction(Arguments args) {

    Local<Function> func;

    try{
        func = compileScript<Function>(args);
    }catch(std::string& error){
        Nan::ThrowError(error.c_str());
    }

    if(!func.IsEmpty() && func->IsFunction()) {
        args.GetReturnValue().Set(func);
    }
}


Local<Value> runFunction(Arguments arg){
  
    if(!arg[0]->IsFunction() && !arg[1]->IsArray())
        throw "Invalid parameters";
    
    auto funk = arg[0].As<v8::Function>();
    auto arguments = arg[1].As<v8::Array>();
    auto argc = arguments->Length();
    auto funk_argument = Helper::getArray(std::move(arguments));
 
    /*
     cout << "@@@ argc ->" << argc
          << "\n# funk allocated->"
          << Helper::PB(funk_argument.get() != nullptr) << endl
          << "@@ isObject this-> " << Helper::PB(arg[2]->IsObject())
          << endl;
    */
    
    Local<Object> object;

    MaybeLocal<Value> returnMaybe = funk->Call(Nan::GetCurrentContext(),
                                               arg[2],
                                               argc,
                                               funk_argument.get());
    
    if(returnMaybe.IsEmpty())
        return Local<Value>();
    else
        return returnMaybe.ToLocalChecked();
}

void RunIt(Arguments arg){
 
    try{
        arg.GetReturnValue().Set(runFunction(arg));
    }catch(string err) {
        Nan::ThrowError(err.c_str());
    }
}



void Init(v8::Local<v8::Object> exports) {

  exports->Set(Nan::New("compile").ToLocalChecked(),
                 Nan::New<v8::FunctionTemplate>(toFunction)->GetFunction());

  exports->Set(Nan::New("run").ToLocalChecked(),
                 Nan::New<v8::FunctionTemplate>(RunIt)->GetFunction());


}

NODE_MODULE(hello, Init)
