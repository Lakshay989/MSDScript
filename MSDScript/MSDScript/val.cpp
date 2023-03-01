//
//  val.cpp
//  MSDScript
//
//  Created by Lakshay Santosh Kucheriya on 2/28/23.
//

//#include "val.hpp"

#include "expr.hpp"
#include "val.hpp"

NumVal::NumVal(int rep) {
    this->rep = rep;
}

Val *NumVal::add_to(Val *other_val) {
    NumVal *other_num = dynamic_cast<NumVal *>(other_val);
    if (other_num == nullptr) {
        throw std::runtime_error("add of non-number");
    }
    int new_val = this->rep + other_num->rep;
    return new NumVal(new_val);
}

Val *NumVal::mult_with(Val *other_val) {
    auto *other_num = dynamic_cast<NumVal *>(other_val);
    if (other_num == nullptr) {
        throw std::runtime_error("mult with non-number");
    }
    int new_val = this->rep * other_num->rep;
    return new NumVal(new_val);
}

bool NumVal::equals(Val *other_val) {
    auto *other_num = dynamic_cast<NumVal *>(other_val);
    if(other_num == nullptr) {
        return false;
    }
    return this->rep == other_num->rep;
}

std::string NumVal::to_string() {
    return std::to_string(this->rep);
}

Expr *NumVal::to_expr() {
    return new NumExpr(this->rep);
}
