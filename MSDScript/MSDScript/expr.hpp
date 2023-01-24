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

class Expr
{
public:
    virtual bool equals(Expr *e) = 0;
};

class Num : public Expr
{
public:
    int val;
    
    Num(int val);
    bool equals(Expr *e) ;
};

class Add : public Expr
{
public:
    Expr *lhs;
    Expr *rhs;
    
    Add(Expr *lhs, Expr *rhs);
    bool equals(Expr *e) ;
};

class Mult : public Expr
{
public:
    Expr *lhs;
    Expr *rhs;
    
    Mult(Expr *lhs, Expr *rhs);
    bool equals(Expr *e) ;
};

class Var : public Expr
{
public:

      std::string name;
      Var(std::string name);
      bool equals(Expr *e);
};

#endif /* expr_hpp */
