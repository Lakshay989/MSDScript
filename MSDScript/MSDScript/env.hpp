//
//  env.hpp
//  MSDScript
//
//  Created by Lakshay Santosh Kucheriya on 4/4/23.
//

#ifndef env_hpp
#define env_hpp

#include <stdio.h>

#include "pointer.h"
#include <string>

class Val;

CLASS(Env) {
public:
    static PTR(Env) empty;
    virtual PTR(Val) lookup(std::string find_name) = 0;
    virtual ~Env() = default;
};

class EmptyEnv : public Env {
public:
    EmptyEnv() = default;

    PTR(Val) lookup(std::string matcher);
};

class ExtendedEnv : public Env {
public:
    std::string name;
    PTR(Val) val;
    PTR(Env) rest;

    ExtendedEnv(std::string name, PTR(Val) val, PTR(Env) rest);

    PTR(Val) lookup(std::string matcher);
};

#endif /* env_hpp */
