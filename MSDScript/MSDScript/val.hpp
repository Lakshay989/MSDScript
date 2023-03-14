//
//  val.hpp
//  MSDScript
//
//  Created by Lakshay Santosh Kucheriya on 2/28/23.
//

#ifndef val_hpp
#define val_hpp

class Expr ;

#include <stdio.h>
#include <string>

class Val
{
public:
    virtual Val *add_to(Val *other_val) = 0 ;
    virtual Val *mult_with(Val *other_val) = 0 ;
    virtual bool equals(Val *other_val) = 0 ;
    virtual std::string to_string() = 0;
    virtual Expr *to_expr() = 0 ;
    virtual bool is_true() = 0;

} ;

class NumVal : public Val
{
private:
    int rep ;

public:
    
    NumVal(int rep) ;
    
    Val *add_to(Val *other_val) ;
    Val *mult_with(Val *other_val) ;
    bool equals(Val *other_val) ;
    std::string to_string() ;
    Expr *to_expr() ;
    bool is_true() ;
} ;

class BoolVal : public Val
{
private:
    bool rep ;
    
public:
    
    BoolVal(bool rep) ;

    Val *add_to(Val *other_val) ;
    Val *mult_with(Val *other_val) ;
    bool equals(Val *other_val) ;
    std::string to_string() ;
    Expr *to_expr() ;
    bool is_true() ;
};

#endif /* val_hpp */

