//
//  expr.cpp
//  MSDScript
//
//  Created by Lakshay Santosh Kucheriya on 1/23/23.
//

#include "expr.hpp"


Num::Num(int val)
{
    this->val = val;
}

bool Num::equals(Expr *e)  // Num Equals
{
    Num *n = dynamic_cast<Num*>(e);
    if (n == NULL)
        return false;
    else
        return val == n->val;
}

int Num::interp()
{
    return int(val);
}

bool Num::has_variable()
{
    return false;
}

Expr* Num::subst(std::string s, Expr* e)
{
    return this ;
}

void Num::print(std::ostream &out) {
    out << std::to_string(val);
}

void Num::pretty_print(std::ostream &out) {
    out << std::to_string(val);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



Add::Add(Expr *lhs, Expr *rhs)
{
    this->lhs = lhs;
    this->rhs = rhs;
}
    
bool Add::equals(Expr *e) // Add Equals
{
    Add *a = dynamic_cast<Add*>(e);
    if (a == NULL)
        return false;
    else
        return (lhs->equals(a->lhs)
                && rhs->equals(a->rhs));
}

int Add::interp()
{
    return (int)lhs->interp() + (int)rhs->interp() ;
}

bool Add::has_variable()
{
    return (lhs->has_variable() || rhs->has_variable()) ;
}

Expr* Add::subst(std::string s, Expr* e)
{
    return new Add(lhs->subst(s, e), rhs->subst(s, e));
}

void Add::print(std::ostream &out) {
    out << "(";
    this->lhs->print(out);
    out << "+";
    this->rhs->print(out);
    out << ")";
}

void Add::pretty_print(std::ostream &out) {
    operator_precedence lhs_precedence = pretty_print_at(this->lhs);
    if (lhs_precedence == precedence_add) {
        out << "(";
        this->lhs->pretty_print(out);
        out << ")";
    } else {
        out << this->lhs->to_pretty_string();
    }
    out << " + ";
    out << this->rhs->to_pretty_string();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


Mult::Mult(Expr *lhs, Expr *rhs)
{
    this->lhs = lhs;
    this->rhs = rhs;
}

bool Mult::equals(Expr* e) { // Mult equals
  Mult* m = dynamic_cast<Mult*>(e);
  if (m == NULL)
    return false;
  else
    return (lhs->equals(m->lhs)
            && rhs->equals(m->rhs));
}

int Mult::interp()
{
    return (int)lhs->interp() * (int)rhs->interp();
}

bool Mult::has_variable()
{
    return (lhs->has_variable() || rhs->has_variable());
}


Expr* Mult::subst(std::string s, Expr* e)
{
    return new Mult(lhs->subst(s, e), rhs->subst(s, e));
}

void Mult::print(std::ostream &out) {
    out << "(";
    this->lhs->print(out);
    out << "*";
    this->rhs->print(out);
    out << ")";
}

void Mult::pretty_print(std::ostream &out) {
    operator_precedence lhs_precedence = pretty_print_at(this->lhs);
    operator_precedence rhs_precedence = pretty_print_at(this->rhs);
    if (lhs_precedence >= precedence_add) {
        out << "(";
        this->lhs->pretty_print(out);
        out << ")";
    } else {
        this->lhs->pretty_print(out);
    }
    out << " * ";
    if (rhs_precedence == precedence_add) {
        out << "(";
        this->rhs->pretty_print(out);
        out << ")";
    } else {
        this->rhs->pretty_print(out);
    }
}
     
     
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

     

Var::Var(std::string name)
{
    this->name = name ;
}

bool Var::equals(Expr *e)  // Var equals
{
    Var* v = dynamic_cast<Var*>(e);
    if (v == nullptr)
        return false;
    else
        return name == v->name;
}

int Var::interp()
{
    throw std::runtime_error("interp does not work with Variable Expressions !!");
}

bool Var::has_variable()
{
    return true;
}

Expr* Var::subst(std::string s, Expr* e)
{
    if(name == s)
    {
        return e ;
    }
    else
    {
        return this ;
    }
}

void Var::print(std::ostream &out) {
    out << this->name;
}

void Var::pretty_print(std::ostream &out) {
    out << this->name;
}


operator_precedence pretty_print_at(Expr *e) {
    Add *add = dynamic_cast<Add *>(e);
    if (add != NULL) {
        return precedence_add;
    }
    Mult *mult = dynamic_cast<Mult *>(e);
    if (mult != NULL) {
        return precedence_mult;
    }
    return precedence_none;
}



