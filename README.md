## Functor


This is just my attempt to create something like Serverless(AWS Lambda), using only Node.js.

### Starting the server

For now I have only tested in Mac OS X but it should work with little tune up at binding.gyp in Linux + GCC, to get it working on Mac you just need to do the following.

```

git clone https://github.com/cesarvr/functorz
npm install
npm install node-gyp

node-gyp rebuild --debug  ## this compile the native-bindings

node application.js ## run the server

```

After that just visit the localhost:8080 and you would see a rudimentary IDE to writing your functions.


### Modules

 - lib/cpp/serverz.cc
  - This class take care of interfacing with v8 capabilities, for now it just compiles javascript source code and pass it to the Javascript VM at runtime and is capable to execute javascript functions passed from runtime, this can be cool for experimentation like execute JS functions using another v8::Isolate, thread or try other tricks in the future.

 - application.js
   - For demo purposes this is just a simple class that creates a express.js server, and bind hot compiled functions to routers with the same function name.

   ```
     example:
     var fib = functor.compile(fib_code_string);    
     app.router('fib', fib);
   ```  

   - Also it parse the request using body parser and inject it to the function, then function result is delegated to [res.send](https://expressjs.com/en/api.html#res).
