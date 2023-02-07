//
//  expr.hpp
//  MSDScript
//
//  Created by Lakshay Santosh Kucheriya on 1/23/23.
//

#ifndef expr_hpp
#define expr_hpp

#include <stdio.h>
#include <string>
#include <iostream>
#include <stdexcept>
#include <sstream>



class Expr  // Base class
{
public:
    virtual bool equals(Expr *e) = 0 ;  // This function checks for the equality for the LHS and the RHS.
    virtual int interp() = 0 ; // This function interprets the value of the expression/variable.
    virtual bool has_variable() = 0 ; // This function determines if the the expression consists of a variable or not.
    virtual Expr* subst(std::string s, Expr* e) = 0; // This function substitutes the expression with the combination of sub-expressions if possible.
    virtual void print(std::ostream &out) = 0; // This function prints the expression.
    virtual void pretty_print(std::ostream &out) = 0; // This is an extenstion of the print function with minor changes.
    
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
    
};


//Derived Class from Expr for representing a number

class Num : public Expr
{
public:
    int val;
    
    Num(int val);
    bool equals(Expr *e) ;
    int interp();
    bool has_variable();
    Expr* subst(std::string s, Expr* e) ;
    void print(std::ostream &out);
    void pretty_print(std::ostream &out);
};


//Derived Class from Expr to represent an expression that involves addition
class Add : public Expr
{
public:
    Expr *lhs;
    Expr *rhs;
    
    Add(Expr *lhs, Expr *rhs);
    bool equals(Expr *e) ;
    int interp();
    bool has_variable();
    Expr* subst(std::string s, Expr* e) ;
    void print(std::ostream &out);
    void pretty_print(std::ostream &out);
    
};


//Derived Class from Expr to represent an expression that involves multiplication
class Mult : public Expr
{
public:
    Expr *lhs;
    Expr *rhs;
    
    Mult(Expr *lhs, Expr *rhs);
    bool equals(Expr *e) ;
    int interp();
    bool has_variable();
    Expr* subst(std::string s, Expr* e) ;
    void print(std::ostream &out);
    void pretty_print(std::ostream &out);
};


//Derived Class from Expr for representing a variable in an expression

class Var : public Expr
{
public:

      std::string name;
      Var(std::string name);
    
      bool equals(Expr *e);
      int interp();
      bool has_variable();
      Expr* subst(std::string s, Expr* e) ;
      void print(std::ostream &out);
      void pretty_print(std::ostream &out);
};


// Enumeration for assigning the precedence of the operators
enum operator_precedence {
    precedence_none = 0,
    precedence_add = 1,
    precedence_mult = 2
};

operator_precedence pretty_print_at(Expr *e);


#endif /* expr_hpp */
