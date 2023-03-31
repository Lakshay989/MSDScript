//
//  val.hpp
//  MSDScript
//
//  Created by Lakshay Santosh Kucheriya on 2/28/23.
//

#ifndef val_hpp
#define val_hpp

class Expr ;

#include "pointer.h"
#include <stdio.h>
#include <string>

CLASS(Val)
{
public:
    virtual PTR(Val) add_to(PTR(Val) other_val) = 0 ;
    virtual PTR(Val) mult_with(PTR(Val) other_val) = 0 ;
    virtual bool equals(PTR(Val) other_val) = 0 ;
    virtual std::string to_string() = 0;
    virtual PTR(Expr) to_expr() = 0 ;
    virtual bool is_true() = 0;
    virtual PTR(Val) call(PTR(Val) actual_arg) = 0 ;
    
    virtual ~Val() = default;;
    
} ;

class NumVal : public Val
{
private:
    int rep ;

public:
    
    NumVal(int rep) ;
    
    PTR(Val) add_to(PTR(Val) other_val) ;
    PTR(Val) mult_with(PTR(Val) other_val) ;
    bool equals(PTR(Val) other_val) ;
    std::string to_string() ;
    PTR(Expr) to_expr() ;
    bool is_true() ;
    PTR(Val) call(PTR(Val) actual_arg);
    
} ;

class BoolVal : public Val
{
private:
    bool rep ;
    
public:
    
    BoolVal(bool rep) ;

    PTR(Val) add_to(PTR(Val) other_val) ;
    PTR(Val) mult_with(PTR(Val) other_val) ;
    bool equals(PTR(Val) other_val) ;
    std::string to_string() ;
    PTR(Expr) to_expr() ;
    bool is_true() ;
    PTR(Val) call(PTR(Val) actual_arg);
};

class FunVal : public Val
{
private:
    std::string formal_arg;
    PTR(Expr) body;
    
public:
    
    FunVal(std::string formal_arg, PTR(Expr) body);

    PTR(Val) add_to(PTR(Val) other_val) ;
    PTR(Val) mult_with(PTR(Val) other_val) ;
    bool equals(PTR(Val) other_val) ;
    std::string to_string() ;
    PTR(Expr) to_expr() ;
    bool is_true() ;
    PTR(Val) call(PTR(Val) actual_arg);
};


#endif /* val_hpp */

