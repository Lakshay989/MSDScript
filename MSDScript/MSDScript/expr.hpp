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

#include <stdio.h>
#include <string>
#include <iostream>
#include <stdexcept>
#include <sstream>


/*! \brief Base Class for representing a expression
*/
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


/*! \brief Derived Class from Expr for representing a number
*/
class Num : public Expr
{
public:
    int val; //!< Value of the number
    
    Num(int val);
    bool equals(Expr *e) ;
    int interp();
    bool has_variable();
    Expr* subst(std::string s, Expr* e) ;
    void print(std::ostream &out);
    void pretty_print(std::ostream &out);
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
    int interp();
    bool has_variable();
    Expr* subst(std::string s, Expr* e) ;
    void print(std::ostream &out);
    void pretty_print(std::ostream &out);
    
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
    int interp();
    bool has_variable();
    Expr* subst(std::string s, Expr* e) ;
    void print(std::ostream &out);
    void pretty_print(std::ostream &out);
};


/*! \brief Derived Class from Expr for representing a expression involving Variable
*/
class Var : public Expr
{
public:

      std::string name; //!< name of the variable
      Var(std::string name);
    
      bool equals(Expr *e);
      int interp();
      bool has_variable();
      Expr* subst(std::string s, Expr* e) ;
      void print(std::ostream &out);
      void pretty_print(std::ostream &out);
};


/*! \brief Enumeration for assigning the precedence of the operators
*/
typedef enum {
  precedence_none = 0,
  precedence_add = 1,
  precedence_mult = 2,     
} precedence_t;

precedence_t pretty_print_at(Expr *e);

#endif /* expr_hpp */
