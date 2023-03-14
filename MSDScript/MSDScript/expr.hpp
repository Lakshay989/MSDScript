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
class Expr  // Base class
{
public:
    virtual bool equals(Expr *e) = 0 ;  // This function checks for the equality for the LHS and the RHS.
    
    virtual Val* interp() = 0 ; // This function interprets the value of the expression/variable.
    
    virtual bool has_variable() = 0 ; // This function determines if the the expression consists of a variable or not.
    
    virtual Expr* subst(std::string s, Expr* e) = 0; // This function substitutes the expression with the combination of sub-expressions if possible.
    
    virtual void print(std::ostream &out) = 0; // This function prints the expression.
    
   // virtual void pretty_print(std::ostream &out) = 0; // This is an extenstion of the print function with minor changes.
    
    virtual void pretty_print_at(std::ostream &out, precedence_t precedence, bool parenthesis_let, bool parenthesis_eq, int position = 0) = 0; // Helper for pretty_print()
    
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
    };
    
};


/*! \brief Derived Class from Expr for representing a number
*/
class NumExpr : public Expr
{
public:
    int val; //!< Value of the number
    
    NumExpr(int val);
    bool equals(Expr *e) ;
    Val* interp();
    bool has_variable();
    Expr* subst(std::string s, Expr* e) ;
    void print(std::ostream &out);
    //void pretty_print(std::ostream &out);
    void pretty_print_at(std::ostream &out, precedence_t precedence, bool parenthesis_let, bool parenthesis_eq, int position);
};


/*! \brief Derived Class from Expr for representing a expression involving addition
*/
class Add : public Expr
{
public:
    Expr *lhs; //!< Left hand side Expression
    Expr *rhs; //!< Right hand side Expression
    
    Add(Expr *lhs, Expr *rhs);
    bool equals(Expr *e) ;
    Val* interp();
    bool has_variable();
    Expr* subst(std::string s, Expr* e) ;
    void print(std::ostream &out);
   // void pretty_print(std::ostream &out);
    void pretty_print_at(std::ostream &out, precedence_t precedence, bool parenthesis_let, bool parenthesis_eq, int position);
    
};


/*! \brief Derived Class from Expr for representing a expression involving addition
*/
class Mult : public Expr
{
public:
    Expr *lhs; //!< Left hand side Expression
    Expr *rhs; //!< Right hand side Expression
    
    Mult(Expr *lhs, Expr *rhs);
    bool equals(Expr *e) ;
    Val* interp();
    bool has_variable();
    Expr* subst(std::string s, Expr* e) ;
    void print(std::ostream &out);
    //void pretty_print(std::ostream &out);
    void pretty_print_at(std::ostream &out, precedence_t precedence, bool parenthesis_let, bool parenthesis_eq, int position);
};


/*! \brief Derived Class from Expr for representing a expression involving Variable
*/
class Var : public Expr
{
public:
    
    std::string name; //!< name of the variable
    Var(std::string name);
    
    bool equals(Expr *e);
    Val* interp();
    bool has_variable();
    Expr* subst(std::string s, Expr* e) ;
    void print(std::ostream &out);
    //void pretty_print(std::ostream &out);
    void pretty_print_at(std::ostream &out, precedence_t precedence, bool parenthesis_let, bool parenthesis_eq, int position);
};


/*! \brief Derived Class from Expr for the purpose of assigning a value to the objects of the variable class
*/
class Let : public Expr
{
public:
    
    std::string lhs ; //!< string on the lhs of the expression
    Expr *rhs ; //!< Expression on the rhs of the variable
    Expr *body ; //!< body of the rhs
    
    Let(std::string lhs, Expr *rhs, Expr *body) ;
    
    bool equals(Expr *e);
    Val* interp();
    bool has_variable();
    Expr* subst(std::string s, Expr* e) ;
    void print(std::ostream &out);
    //void pretty_print(std::ostream &out);
    void pretty_print_at(std::ostream &out, precedence_t precedence, bool parenthesis_let, bool parenthesis_eq, int position);
    
};

class BoolExpr : public Expr {
public:
    bool rep;

    BoolExpr(bool rep);
    
    bool equals(Expr *e);
    Val *interp();
    bool has_variable();
    Expr *subst(std::string s, Expr *e);
    void print(std::ostream &out);
    //void pretty_print(std::ostream &out);
    void pretty_print_at(std::ostream &out, precedence_t precedence, bool parenthesis_let, bool parenthesis_eq, int position);
};


class IfExpr : public Expr {
public:
    Expr *condition; //bool ?
    Expr *then_expr;
    Expr *else_expr;

    //IfExpr(bool condition, Expr *then_expr, Expr *else_expr);
    IfExpr(Expr *condition, Expr *then_expr, Expr *else_expr);
    
    bool equals(Expr *e);

    Val *interp();
    bool has_variable();
    Expr *subst(std::string s, Expr *e);
    void print(std::ostream &out);
    //void pretty_print(std::ostream &out);
    void pretty_print_at(std::ostream &out, precedence_t precedence, bool parenthesis_let, bool parenthesis_eq, int position);
};


class EqExpr : public Expr {
public:
    Expr *lhs;
    Expr *rhs;

    EqExpr(Expr *lhs, Expr *rhs);

    bool equals(Expr *e);
    Val *interp();
    bool has_variable();
    Expr *subst(std::string s, Expr *e);
    void print(std::ostream &out);
    //void pretty_print(std::ostream &out);
    void pretty_print_at(std::ostream &out, precedence_t precedence, bool parenthesis_let, bool parenthesis_eq, int position);
};

#endif /* expr_hpp */
