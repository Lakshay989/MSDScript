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

class Expr
{
public:
    virtual bool equals(Expr *e) = 0 ;
    virtual int interp() = 0 ;
    virtual bool has_variable() = 0 ;
    virtual Expr* subst(std::string s, Expr* e) = 0;
    virtual void print(std::ostream &out) = 0;
    virtual void pretty_print(std::ostream &out) = 0;
    
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

enum operator_precedence {
    precedence_none = 0,
    precedence_add = 1,
    precedence_mult = 2
};

operator_precedence pretty_print_at(Expr *e);


#endif /* expr_hpp */
