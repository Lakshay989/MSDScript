//
//  expr.cpp
//  MSDScript
//
//  Created by Lakshay Santosh Kucheriya on 1/23/23.
//

/**
* \file expr.cpp
* \brief Contains expression class definition
*
* \author Lakshay Santosh Kucheriya
*/

#include "val.hpp"
#include "expr.hpp"
#include "env.hpp"
#include <utility>
/**
* \brief Constructor
*/
NumExpr::NumExpr(int val)
{
    this->val = val;
}


/**
* \brief Checks for the equality between the left hand side and the right hand side
* \param e Expression
* \return boolean value of LHS = RHS
*/
bool NumExpr::equals(PTR(Expr) e)
{
    auto other = CAST(NumExpr)(e);
    if (other == nullptr)
        return false;
    else
        return val == other->val;
}


/**
* \brief This function interprets the value of the integer
* \return integer value of the number
*/
PTR(Val) NumExpr::interp(PTR(Env) env)
{
    return NEW(NumVal)(this->val);
}

/**
* \brief Prints the expression
* \param [out] out output stream
*/
void NumExpr::print(std::ostream &out) {
    out << std::to_string(val);
}

void NumExpr::pretty_print_at(std::ostream &out, precedence_t precedence, bool parenthesis_let_or_fun, bool parenthesis_eq, int position) {
    out << std::to_string(val);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/**
* \brief Constructor
*/
Add::Add(PTR(Expr) lhs, PTR(Expr) rhs)
{
    this->lhs = lhs;
    this->rhs = rhs;
}
    

/**
* \brief Checks for the equality between the left hand side and the right hand side
* \param e Expression
* \return boolean value of LHS = RHS
*/
bool Add::equals(PTR(Expr) e) // Add Equals
{
    auto other= CAST(Add)(e);
    if (other == NULL)
        return false;
    else
        return (lhs->equals(other->lhs)
                && rhs->equals(other->rhs));
}


/**
* \brief This function interprets the value of the expression
* \return integer value of the computed expression
*/
PTR(Val) Add::interp(PTR(Env) env)
{
    if(env == nullptr) {
         env = Env::empty;
     }
    return this->lhs->interp(env)->add_to(this->rhs->interp(env));
    
}

/**
* \brief Prints the expression
* \param [out] out output stream
*/
void Add::print(std::ostream &out) {
    out << "(";
    this->lhs->print(out);
    out << "+";
    this->rhs->print(out);
    out << ")";
}


void Add::pretty_print_at(std::ostream &out, precedence_t precedence, bool parenthesis_let_or_fun, bool parenthesis_eq, int position) {
    if (precedence >= precedence_add) {
        out << "(";
    }
    this->lhs->pretty_print_at(out, precedence_add, true, true, position);
        out << " + ";
        this->rhs->pretty_print_at(out, precedence_none, false, true, position);
        if (precedence >= precedence_add) {
            out << ")";
        }
    }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* \brief Constructor
*/
Mult::Mult(PTR(Expr) lhs, PTR(Expr) rhs)
{
    this->lhs = lhs;
    this->rhs = rhs;
}


/**
* \brief Checks for the equality between the left hand side and the right hand side
* \param e Expression
* \return boolean value of LHS = RHS
*/
bool Mult::equals(PTR(Expr) e) { // Mult equals
  auto other = CAST(Mult)(e);
  if (other == NULL)
    return false;
  else
    return (lhs->equals(other->lhs)
            && rhs->equals(other->rhs));
}


/**
* \brief This function interprets the value of the expression
* \return integer value of the computed expression
*/
PTR(Val) Mult::interp(PTR(Env) env)
{
    if(env == nullptr)
    {
        env = Env::empty ;
    }
    return this->lhs->interp(env)->mult_with(this->rhs->interp(env));
    
}

/**
* \brief Prints the expression
* \param [out] out output stream
*/
void Mult::print(std::ostream &out) {
    out << "(";
    this->lhs->print(out);
    out << "*";
    this->rhs->print(out);
    out << ")";
}


void Mult::pretty_print_at(std::ostream &out, precedence_t precedence, bool parenthesis_let_or_fun, bool parenthesis_eq,  int position) {
    if (precedence >= precedence_mult) {
        out << "(";
    }
    //out <<
    this->lhs->pretty_print_at(out, precedence_mult, true, true, position);
    out << " * " ;
    this->rhs->pretty_print_at(out, precedence_add, !(precedence >= precedence_mult) && parenthesis_let_or_fun, true, position);
    if (precedence >= precedence_mult) {
        out << ")";
    }
}
     
     
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* \brief Constructor
*/
Var::Var(std::string name)
{
    this->name = name ;
}


/**
* \brief Checks for the equality between the left hand side and the right hand side
* \param e Expression
* \return boolean value of LHS = RHS
*/
bool Var::equals(PTR(Expr) e)  // Var equals
{
    auto other = CAST(Var)(e);
    if (other == nullptr)
        return false;
    else
        return name == other->name;
}


/**
* \brief This function interprets the value of the Variable
* \return error message as value of the variable cannot be iterpreted
*/
PTR(Val) Var::interp(PTR(Env) env)
{
    if(env == nullptr)
    {
        env = Env::empty ;
    }
    return env->lookup(this->name);
}

/**
* \brief Prints the expression
* \param [out] out output stream
*/
void Var::print(std::ostream &out) {
    out << this->name;
}


void Var::pretty_print_at(std::ostream &out, precedence_t precedence, bool parenthesis_let_or_fun, bool parenthesis_eq, int position) {
    out << this->name;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* \brief Constructor
*/
Let::Let(std::string lhs, PTR(Expr) rhs, PTR(Expr) body)
{
    this->lhs = lhs ;
    this->rhs = rhs ;
    this->body = body ;
}


/**
* \brief Checks for the equality between the left hand side and the right hand side
* \param e Expression
* \return boolean value of LHS = RHS
*/
bool Let::equals(PTR(Expr) e)
{
    auto other = CAST(Let)(e);
    
    if(other == nullptr)
    {
        return false;
    }
    else
    {
        return (other->lhs == this->lhs && other->rhs->equals(this->rhs) && other->body->equals(this->body) ) ;
    }
}


/**
* \brief This function interprets the value of the Variable
* \return value of the variable assigned using Let
*/
PTR(Val) Let::interp( PTR(Env) env) // More Testing rhs->interp()
{
    if(env == nullptr) {
    env = Env::empty;
    }
    PTR(Val) rhs_val = this->rhs->interp(env);
    PTR(Env) new_env = NEW(ExtendedEnv)(lhs, rhs_val, env);
    return body->interp(new_env);

}

/**
* \brief Prints the expression
* \param [out] out output stream
*/
void Let::print(std::ostream &out)
{
    out<< "(_let " << this->lhs << "=" << this->rhs->to_string() << " _in " << this->body->to_string() << ")" ;
}


void Let::pretty_print_at(std::ostream &out, precedence_t precedence, bool parenthesis_let_or_fun, bool parenthesis_eq, int position)
{
    if (parenthesis_let_or_fun) {
            out << "(";
            //position += 1;
        }
    int spaces = (int) out.tellp() - position ;
    out << "_let " << this->lhs << " = " ;
    this->rhs->pretty_print_at(out, precedence, false, false, position) ;
    out << "\n" ;
    position = out.tellp() ;
    out << std::string(spaces, ' ') << "_in  " ;
    this->body->pretty_print_at(out, precedence_none, false, false, position);
    if (parenthesis_let_or_fun)
    {
        out << ")";
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


BoolExpr::BoolExpr(bool rep)
{
    this->rep = rep ;
}
    
bool BoolExpr::equals(PTR(Expr) e)
{
    auto other = CAST(BoolExpr)(e) ;
    if(other == nullptr)
    {
        return false;
    }
    return  this->rep == other->rep;
}

PTR(Val) BoolExpr::interp(PTR(Env) env)
{
    return NEW (BoolVal)(this->rep);
}


void BoolExpr::print(std::ostream &out)
{
    if(this->rep)
    {
        out << "_true" ;
    }
    else
    {
        out << "_false" ;
    }
}

void BoolExpr::pretty_print_at(std::ostream &out, precedence_t precedence, bool parenthesis_let_or_fun, bool parenthesis_eq, int position)
{
    if(this->rep)
    {
        out << "_true" ;
    }
    else
    {
        out << "_false" ;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


IfExpr::IfExpr(PTR(Expr) condition, PTR(Expr) then_expr, PTR(Expr) else_expr)
{
    this->condition = condition;
    this->then_expr = then_expr;
    this->else_expr = else_expr;
    }
    
bool IfExpr::equals(PTR(Expr) e)
{
    auto other = CAST(IfExpr)(e);
    if (other == nullptr) {
        return false;
    }
    return this->condition->equals(other->condition) && this->then_expr->equals(other->then_expr) &&
           this->else_expr->equals(other->else_expr);
}

PTR(Val) IfExpr::interp(PTR(Env) env)
{
    if(env == nullptr) {
        env = Env::empty;
    }
    PTR(Val) condition_val = this->condition->interp(env);
    
    if (condition_val->is_true()) {
        return this->then_expr->interp(env);
    }
    else
    {
        return this->else_expr->interp(env);
    }
}

void IfExpr::print(std::ostream &out)
{
    out << "(_if ";
    //auto *condition = NEW BoolVal(this->condition);
    out << this->condition->to_string();
    out << " _then ";
    this->then_expr->print(out);
    out << " _else ";
    this->else_expr->print(out);
    out << ")";
}


void IfExpr::pretty_print_at(std::ostream &out, precedence_t precedence, bool parenthesis_let_or_fun, bool parenthesis_eq, int position)
{

    int blank_spaces_backoff = (int) out.tellp() - position;
        out << "_if ";
        this->condition->pretty_print_at(out, precedence_none, false, false, position);
        out << "\n";
        position = out.tellp();
        out << std::string(blank_spaces_backoff, ' ') << "_then ";
        this->then_expr->pretty_print_at(out, precedence_none, false, false, position);
        out << "\n";
        position = out.tellp();
        out << std::string(blank_spaces_backoff, ' ') << "_else ";
        this->else_expr->pretty_print_at(out, precedence_none, false, false, position);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EqExpr::EqExpr(PTR(Expr) lhs, PTR(Expr) rhs)
{
    this->lhs = lhs ;
    this->rhs = rhs ;
}

bool EqExpr::equals(PTR(Expr) e)
{
    auto other = CAST(EqExpr)(e);
        if (other == nullptr) {
            return false;
        }
        return this->lhs->equals(other->lhs) && this->rhs->equals(other->rhs);
}

PTR(Val) EqExpr::interp(PTR(Env) env)
{
    if(env == nullptr)
    {
        env = Env::empty;
    }
    
    PTR(Val) lhs = this->lhs->interp(env);
    PTR(Val) rhs = this->rhs->interp(env);
    
    bool result = lhs->equals(rhs);
    return NEW (BoolVal)(result);
}

void EqExpr::print(std::ostream &out)
{
    out << "(";
    this->lhs->print(out);
    out << "==";
    this->rhs->print(out);
    out << ")";
}

void EqExpr::pretty_print_at(std::ostream &out, precedence_t precedence, bool parenthesis_let_or_fun, bool parenthesis_eq, int position)
{
    if (parenthesis_let_or_fun) {
        out << "(";
    }
    this->lhs->pretty_print_at(out, precedence_none, true, true, position);
    out << " == ";
    this->rhs->pretty_print_at(out, precedence_none, false, false, position);
    if (parenthesis_eq)
    {
        out << ")";
    }
    
}


FunExpr::FunExpr(std::string formal_arg, PTR(Expr) body)
{
    this->formal_arg = formal_arg ; // std::move()
    this->body = body ;
    
}
    
bool FunExpr::equals(PTR(Expr) e)
{
    auto other = CAST(FunExpr)(e);
        if (other == nullptr) {
            return false;
        }
        return this->formal_arg == other->formal_arg && this->body->equals(other->body);
}
    
PTR(Val) FunExpr::interp(PTR(Env) env)
{
    if(env == nullptr)
    {
        env = Env::empty ;
    }
    
    return NEW (FunVal)(this->formal_arg, this->body, env);
}
    
void FunExpr::print(std::ostream &out)
{
    out << "(_fun(" << this->formal_arg << ")";
    this->body->print(out);
    out << ")";
}
    
void FunExpr::pretty_print_at(std::ostream &out, precedence_t precedence, bool parenthesis_let_or_fun, bool parenthesis_eq, int position)
{
    
    if (parenthesis_let_or_fun)
    {
        out << "(";
    }
    int blank_spaces = (int) out.tellp() - position + 2;
        out << "_fun (" << this->formal_arg << ")\n";
        position = out.tellp();
        out << std::string(blank_spaces, ' ');
        this->body->pretty_print_at(out, precedence_none, false, false, position);
        if (parenthesis_let_or_fun) {
            out << ")";
        }
}


CallExpr::CallExpr(PTR(Expr) to_be_called, PTR(Expr) actual_arg)
{
    this->to_be_called = to_be_called ;
    this->actual_arg = actual_arg ;
}
    
bool CallExpr::equals(PTR(Expr) e)
{
    auto other = CAST(CallExpr)(e);
        if (other == nullptr) {
            return false;
        }
        return this->to_be_called->equals(other->to_be_called) && this->actual_arg->equals(other->actual_arg);
}
    
PTR(Val) CallExpr::interp(PTR(Env) env)
{
    if(env == nullptr)
    {
        env = Env::empty ;
    }
    return this->to_be_called->interp(env)->call(this->actual_arg->interp(env));
}
    
void CallExpr::print(std::ostream &out)
{
    out << "(";
    this->to_be_called->print(out);
    out << ") (";
    this->actual_arg->print(out);
    out << ")";
}
    
void CallExpr::pretty_print_at(std::ostream &out, precedence_t precedence, bool parenthesis_let_or_fun, bool parenthesis_eq, int position)
{
    this->to_be_called->pretty_print_at(out, precedence_none, true, false, position);
        out << "(";
    this->actual_arg->pretty_print_at(out, precedence_none, false, false, position);
        out << ")";
}
    
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////








