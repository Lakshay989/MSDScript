//
//  env.cpp
//  MSDScript
//
//  Created by Lakshay Santosh Kucheriya on 4/4/23.
//

#include <iostream>
#include <utility>
#include "val.hpp"
#include "env.hpp"

PTR(Env) Env::empty = NEW(EmptyEnv)();


PTR(Val) EmptyEnv::lookup(std::string matcher) {
    throw std::runtime_error("free variable: "
                             + matcher);
}


ExtendedEnv::ExtendedEnv(std::string name, PTR(Val) val, PTR(Env) rest) {
    this->name = std::move(name);
    this->val = std::move(val);
    this->rest = std::move(rest);
}

PTR(Val) ExtendedEnv::lookup(std::string matcher) {
    if (matcher == this->name) {
        return this->val;
    } else {
        return this->rest->lookup(matcher);
    }
}
