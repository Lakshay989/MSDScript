//
//  val.cpp
//  MSDScript
//
//  Created by Lakshay Santosh Kucheriya on 2/28/23.
//

//#include "val.hpp"

#include "expr.hpp"
#include "val.hpp"

NumVal::NumVal(int rep)
{
    this->rep = rep;
}

Val *NumVal::add_to(Val *other_val)
{
    NumVal *other_num = dynamic_cast<NumVal *>(other_val);
    if (other_num == nullptr) {
        throw std::runtime_error("add of non-number");
    }
    int new_val = (unsigned int) this->rep + other_num->rep;
    return new NumVal(new_val);
}

Val *NumVal::mult_with(Val *other_val)
{
    auto *other_num = dynamic_cast<NumVal *>(other_val);
    if (other_num == nullptr) {
        throw std::runtime_error("mult with non-number");
    }
    int new_val = (unsigned int) this->rep * other_num->rep;
    return new NumVal(new_val);
}

bool NumVal::equals(Val *other_val)
{
    auto *other_num = dynamic_cast<NumVal *>(other_val);
    if(other_num == nullptr)
    {
        return false;
    }
    else
    {
        return this->rep == other_num->rep;
    }
}

std::string NumVal::to_string() {
    return std::to_string(this->rep);
}

Expr *NumVal::to_expr() {
    return new NumExpr(this->rep);
}

bool NumVal::is_true()
{
    throw std::runtime_error("A num val can not be a bool val");
    
}

Val *NumVal::call(Val *actual_arg)
{
    throw std::runtime_error("Cannot call a num val");
}


//--------------------------------------------------------------------------------------------------------------

BoolVal::BoolVal(bool rep)
{
    this->rep = rep ;
}

Val *BoolVal::add_to(Val *other_val)
{
    throw std::runtime_error("Addition to a boolean is not possible") ;
}

Val *BoolVal::mult_with(Val *other_val)
{
    throw std::runtime_error("Multiplication with a boolean is not possible") ;
}

bool BoolVal::equals(Val *other_val)
{
    auto other_num = dynamic_cast<BoolVal *>(other_val);
    if(other_num == nullptr)
    {
        return false ;
    }
    else
    {
        return this->rep == other_num->rep;
    }
}

std::string BoolVal::to_string()
{
    if(rep)
    {
        return "_true" ; 
    }
    else
    {
        return "_false" ;
    }
}

Expr *BoolVal::to_expr()
{
    return new BoolExpr(this->rep) ;
}

bool BoolVal::is_true()
{
    return this->rep;
}

Val *BoolVal::call(Val *actual_arg)
{
    throw std::runtime_error("Cannot call a bool val");
}

//--------------------------------------------------------------------------------------------------------------


FunVal::FunVal(std::string formal_arg, Expr *body)
{
    this->formal_arg = formal_arg;
    this->body = body;
}


Val *FunVal::add_to(Val *other_val)
{
    throw std::runtime_error("Addition to a boolean is not possible");
}

Val *FunVal::mult_with(Val *other_val)
{
    throw std::runtime_error("Multiplication with a boolean is not possible");
}

bool FunVal::equals(Val *other_val)
{
    auto *other_fun = dynamic_cast<FunVal *>(other_val);
        if (other_fun == nullptr) {
            return false;
        }
        return this->formal_arg == other_fun->formal_arg && this->body->equals(other_fun->body);
    }

std::string FunVal::to_string()
{
    return this->to_expr()->to_string();
}

Expr *FunVal::to_expr()
{
    return new FunExpr(this->formal_arg, this->body);
}

bool FunVal::is_true()
{
    throw std::runtime_error("a fun val cannot be interpreted as a bool val");
}

Val *FunVal::call(Val *actual_arg)
{
    return this->body->subst(this->formal_arg, actual_arg->to_expr())->interp();
}
