//
//  val.cpp
//  MSDScript
//
//  Created by Lakshay Santosh Kucheriya on 2/28/23.
//

//#include "val.hpp"

#include "expr.hpp"
#include "val.hpp"
#include "env.hpp"

NumVal::NumVal(int rep)
{
    this->rep = rep;
}

PTR(Val) NumVal::add_to(PTR(Val) other_val)
{
    auto other_num = CAST(NumVal)(other_val);
    if (other_num == nullptr) {
        throw std::runtime_error("add of non-number");
    }
    int NEW_val = (unsigned) this->rep + (unsigned) other_num->rep;
    return NEW (NumVal)(NEW_val);
}

PTR(Val) NumVal::mult_with(PTR(Val) other_val)
{
    auto other_num = CAST(NumVal)(other_val);
    if (other_num == nullptr) {
        throw std::runtime_error("mult with non-number");
    }
    int NEW_val = (unsigned) this->rep * (unsigned) other_num->rep;
    return NEW (NumVal)(NEW_val);
}

bool NumVal::equals(PTR(Val) other_val)
{
    auto other_num = CAST(NumVal)(other_val);
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

PTR(Expr) NumVal::to_expr() {
    return NEW (NumExpr)(this->rep);
}

bool NumVal::is_true()
{
    throw std::runtime_error("A num val can not be a bool val");
    
}

PTR(Val) NumVal::call(PTR(Val) actual_arg)
{
    throw std::runtime_error("Cannot call a num val");
}

//--------------------------------------------------------------------------------------------------------------

BoolVal::BoolVal(bool rep)
{
    this->rep = rep ;
}

PTR(Val) BoolVal::add_to(PTR(Val) other_val)
{
    throw std::runtime_error("Addition to a boolean is not possible") ;
}

PTR(Val) BoolVal::mult_with(PTR(Val) other_val)
{
    throw std::runtime_error("Multiplication with a boolean is not possible") ;
}

bool BoolVal::equals(PTR(Val) other_val)
{
    auto other_num = CAST(BoolVal)(other_val);
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

PTR(Expr) BoolVal::to_expr()
{
    return NEW (BoolExpr)(this->rep) ;
}

bool BoolVal::is_true()
{
    return this->rep;
}

PTR(Val) BoolVal::call(PTR(Val) actual_arg)
{
    throw std::runtime_error("Cannot call a bool val");
}

//--------------------------------------------------------------------------------------------------------------

FunVal::FunVal(std::string formal_arg, PTR(Expr) body, PTR(Env) env)
{
    if(env == nullptr)
    {
        env = Env::empty ;
    }
    this->formal_arg = formal_arg;
    this->body = body;
    this->env = std::move(env);
}

PTR(Val) FunVal::add_to(PTR(Val) other_val)
{
    throw std::runtime_error("Addition to a FunVal is not possible");
}

PTR(Val) FunVal::mult_with(PTR(Val) other_val)
{
    throw std::runtime_error("Multiplication with a FunVal is not possible");
}

bool FunVal::equals(PTR(Val) other_val)
{
    auto other_fun = CAST(FunVal)(other_val);
        if (other_fun == nullptr) {
            return false;
        }
        return this->formal_arg == other_fun->formal_arg && this->body->equals(other_fun->body);
    }

std::string FunVal::to_string()
{
    return "function_expr";
}

PTR(Expr) FunVal::to_expr()
{
    return NEW (FunExpr)(this->formal_arg, this->body);
}

bool FunVal::is_true()
{
    throw std::runtime_error("a fun val cannot be interpreted as a bool val");
}

PTR(Val) FunVal::call(PTR(Val) actual_arg)
{
    return this->body->interp(NEW(ExtendedEnv)(this->formal_arg, actual_arg, this->env));
}
