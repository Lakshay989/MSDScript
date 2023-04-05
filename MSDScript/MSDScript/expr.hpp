//
//  expr.hpp
//  MSDScript
//
//  Created by Lakshay Santosh Kucheriya on 1/23/23.
//

/**
* \file expr.hpp
* \brief Header file for expr.cpp file
*
* \author Lakshay Santosh Kucheriya
*/

#ifndef expr_hpp
#define expr_hpp

class Val;
class Env;

#include "pointer.h"

#include <stdio.h>
#include <string>
#include <iostream>
#include <stdexcept>
#include <sstream>


/*! \brief Enumeration for assigning the precedence of the operators
*/
typedef enum {
  precedence_none = 0,
//precedence__let = 1,
  precedence_add = 2,
  precedence_mult = 3,
} precedence_t;


/*! \brief Base Class for representing a expression
*/
CLASS(Expr)  // Base class
{
public:
    virtual bool equals(PTR(Expr) e) = 0 ;  // This function checks for the equality for the LHS and the RHS.
    
    virtual PTR(Val) interp(PTR(Env) env = nullptr) = 0 ; // This function interprets the value of the expression/variable.
    
//    virtual bool has_variable() = 0 ; // This function determines if the the expression consists of a variable or not.
    
//    virtual PTR(Expr) subst(std::string s, PTR(Expr) e) = 0; // This function substitutes the expression with the combination of sub-expressions if possible.
    
    virtual void print(std::ostream &out) = 0; // This function prints the expression.
    
   // virtual void pretty_print(std::ostream &out) = 0; // This is an extenstion of the print function with minor changes.
    
    virtual void pretty_print_at(std::ostream &out, precedence_t precedence, bool parenthesis_let_or_fun, bool parenthesis_eq, int position = 0) = 0; // Helper for pretty_print()
    
    std::string to_string() {
        std::stringstream st("");
        this->print(st);
        return st.str();
    }
    
    
    std::string to_pretty_string() {
        std::stringstream st("");
        this->pretty_print(st);
        return st.str();
    }
    
    void pretty_print(std::ostream &out) {
        this->pretty_print_at(out, precedence_none, false, false, out.tellp());
    }
    
    virtual ~Expr() = default;
};


/*! \brief Derived Class from Expr for representing a number
*/
class NumExpr : public Expr
{
public:
    int val; //!< Value of the number
    
    explicit NumExpr(int val);
    bool equals(PTR(Expr) e) ;
    PTR(Val) interp(PTR(Env) env = nullptr);
    //bool has_variable();
    //PTR(Expr) subst(std::string s, PTR(Expr) e) ;
    void print(std::ostream &out);
    //void pretty_print(std::ostream &out);
    void pretty_print_at(std::ostream &out, precedence_t precedence, bool parenthesis_let_or_fun, bool parenthesis_eq, int position);
};


/*! \brief Derived Class from Expr for representing a expression involving addition
*/
class Add : public Expr
{
public:
    PTR(Expr) lhs; //!< Left hand side Expression
    PTR(Expr) rhs; //!< Right hand side Expression
    
    Add(PTR(Expr) lhs, PTR(Expr) rhs);
    bool equals(PTR(Expr) e) ;
    PTR(Val) interp(PTR(Env) env = nullptr);
    //bool has_variable();
    //PTR(Expr) subst(std::string s, PTR(Expr) e) ;
    void print(std::ostream &out);
   // void pretty_print(std::ostream &out);
    void pretty_print_at(std::ostream &out, precedence_t precedence, bool parenthesis_let_or_fun, bool parenthesis_eq, int position);
    
};


/*! \brief Derived Class from Expr for representing a expression involving addition
*/
class Mult : public Expr
{
public:
    PTR(Expr) lhs; //!< Left hand side Expression
    PTR(Expr) rhs; //!< Right hand side Expression
    
    Mult(PTR(Expr) lhs, PTR(Expr)rhs);
    bool equals(PTR(Expr) e) ;
    PTR(Val) interp(PTR(Env) env = nullptr);
    //bool has_variable();
    //PTR(Expr) subst(std::string s, PTR(Expr) e) ;
    void print(std::ostream &out);
    //void pretty_print(std::ostream &out);
    void pretty_print_at(std::ostream &out, precedence_t precedence, bool parenthesis_let_or_fun, bool parenthesis_eq, int position);
};


/*! \brief Derived Class from Expr for representing a expression involving Variable
*/
class Var : public Expr
{
public:
    
    std::string name; //!< name of the variable
    Var(std::string name);
    
    bool equals(PTR(Expr) e);
    PTR(Val) interp(PTR(Env) env = nullptr);
    //bool has_variable();
    //PTR(Expr) subst(std::string s, PTR(Expr) e) ;
    void print(std::ostream &out);
    //void pretty_print(std::ostream &out);
    void pretty_print_at(std::ostream &out, precedence_t precedence, bool parenthesis_let_or_fun, bool parenthesis_eq, int position);
};


/*! \brief Derived Class from Expr for the purpose of assigning a value to the objects of the variable class
*/
class Let : public Expr
{
public:
    
    std::string lhs ; //!< string on the lhs of the expression
    PTR(Expr) rhs ; //!< Expression on the rhs of the variable
    PTR(Expr) body ; //!< body of the rhs
    
    Let(std::string lhs, PTR(Expr) rhs, PTR(Expr) body) ;
    
    bool equals(PTR(Expr) e);
    PTR(Val) interp(PTR(Env) env = nullptr);
    //bool has_variable();
    //PTR(Expr) subst(std::string s, PTR(Expr) e) ;
    void print(std::ostream &out);
    //void pretty_print(std::ostream &out);
    void pretty_print_at(std::ostream &out, precedence_t precedence, bool parenthesis_let_or_fun, bool parenthesis_eq, int position);
    
};

class BoolExpr : public Expr {
public:
    bool rep;

    BoolExpr(bool rep);
    
    bool equals(PTR(Expr) e);
    PTR(Val) interp(PTR(Env) env = nullptr);
    //bool has_variable();
    //PTR(Expr) subst(std::string s, PTR(Expr) e);
    void print(std::ostream &out);
    //void pretty_print(std::ostream &out);
    void pretty_print_at(std::ostream &out, precedence_t precedence, bool parenthesis_let_or_fun, bool parenthesis_eq, int position);
};


class IfExpr : public Expr
{
public:
    PTR(Expr) condition; //bool ?
    PTR(Expr) then_expr;
    PTR(Expr) else_expr;

    //IfExpr(bool condition, Expr *then_expr, Expr *else_expr);
    IfExpr(PTR(Expr) condition, PTR(Expr) then_expr, PTR(Expr) else_expr);
    
    bool equals(PTR(Expr) e);

    PTR(Val) interp(PTR(Env) env = nullptr);
   // bool has_variable();
   // PTR(Expr) subst(std::string s, PTR(Expr) e);
    void print(std::ostream &out);
    //void pretty_print(std::ostream &out);
    void pretty_print_at(std::ostream &out, precedence_t precedence, bool parenthesis_let_or_fun, bool parenthesis_eq, int position);
};


class EqExpr : public Expr
{
public:
    PTR(Expr) lhs;
    PTR(Expr) rhs;

    EqExpr(PTR(Expr) lhs, PTR(Expr) rhs);

    bool equals(PTR(Expr) e);
    PTR(Val) interp(PTR(Env) env = nullptr);
    //bool has_variable();
    //PTR(Expr) subst(std::string s, PTR(Expr) e);
    void print(std::ostream &out);
    //void pretty_print(std::ostream &out);
    void pretty_print_at(std::ostream &out, precedence_t precedence, bool parenthesis_let_or_fun, bool parenthesis_eq, int position);
};

//  Function _fun (variable) <expr>
class FunExpr : public Expr
{
public:
    
    std::string formal_arg;
    PTR(Expr) body;
    
    FunExpr(std::string formal_arg, PTR(Expr) body);
    
    bool equals(PTR(Expr) e);
    
    PTR(Val) interp(PTR(Env) env = nullptr);
    
    //PTR(Expr) subst(std::string s, PTR(Expr) e);
    
    void print(std::ostream &out);
    
    void pretty_print_at(std::ostream &out, precedence_t precedence, bool parenthesis_let_or_fun, bool parenthesis_eq, int position);
};

class CallExpr : public Expr
{
    
public:
    PTR(Expr) to_be_called;
    PTR(Expr) actual_arg;
    
    CallExpr(PTR(Expr) to_be_called, PTR(Expr) actual_arg);
    
    bool equals(PTR(Expr) e);
    
    PTR(Val) interp(PTR(Env) env = nullptr);
    
    //PTR(Expr) subst(std::string s, PTR(Expr) e);
    
    void print(std::ostream &out);
    
    void pretty_print_at(std::ostream &out, precedence_t precedence, bool parenthesis_let_or_fun, bool parenthesis_eq, int position);
    
};
    
#endif /* expr_hpp */
