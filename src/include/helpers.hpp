//
//  helpers.hpp
//  Functorx
//
//  Created by Cesar Valdez on 24/04/2017.
//  Copyright © 2017 Cesar Valdez. All rights reserved.
//

#ifndef helpers_hpp
#define helpers_hpp

#include "nan.h"
#include <iostream>
#include <ctime>

using namespace v8;
using namespace std;

struct Jail {
    
    template <typename Caller>
    auto call(Caller& fn) -> decltype(fn()){
        start = std::clock();
        auto ret = fn();
        duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
        std::cout<<"duration: "<< duration <<'\n';
        
        return ret;
    }
    
private:
    std::clock_t start;
    double duration;
};

struct Helper {
public:
    static std::string getMessageFromTry(TryCatch* try_catch) {
        String::Utf8Value error(try_catch->Exception());
        std::string errorMessage;

        if(*error != nullptr)
            errorMessage.append(*error);
        else
            errorMessage.append("unknown error.");
        
        return errorMessage;
    }
    
    static std::string getMessageFromTry(std::string message, TryCatch* try_catch) {
    
        return message + getMessageFromTry(try_catch);
    }
};





#endif /* helpers_hpp */
