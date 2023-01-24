//
//  expr.cpp
//  MSDScript
//
//  Created by Lakshay Santosh Kucheriya on 1/23/23.
//

#include "expr.hpp"

#include "catch.h"

Num::Num(int val)
{
    this->val = val;
}

bool Num::equals(Expr *e)
{
    Num *n = dynamic_cast<Num*>(e);
    if (n == NULL)
        return false;
    else
        return val == n->val;
}

Add::Add(Expr *lhs, Expr *rhs)
{
    this->lhs = lhs;
    this->rhs = rhs;
}
    
bool Add::equals(Expr *e)
{
    Add *a = dynamic_cast<Add*>(e);
    if (a == NULL)
        return false;
    else
        return (lhs->equals(a->lhs)
                && rhs->equals(a->rhs));
    }

Mult::Mult(Expr *lhs, Expr *rhs)
{
    this->lhs = lhs;
    this->rhs = rhs;
}

bool Mult::equals(Expr* e) {
  Mult* m = dynamic_cast<Mult*>(e);
  if (m == NULL)
    return false;
  else
    return (lhs->equals(m->lhs)
            && rhs->equals(m->rhs));
}

Var::Var(std::string name)
{
    this->name = name ;
}

bool Var::equals(Expr *e)
{
    Var* v = dynamic_cast<Var*>(e);
    if (v == nullptr)
        return false;
    else
        return name == v->name;
}


//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
TEST_CASE( "equals" ) {
    CHECK( (new Num(1))->equals(new Num(1)) );
    CHECK( ! (new Num (1))->equals(new Num(2)) );
    CHECK( ! (new Num (1))->equals(new Mult(new Num(2), new Num(4))) );
    CHECK(!(new Num (8))->equals(new Mult(new Num(2), new Num(4))) );
    CHECK(!(new Num (1))->equals(new Mult(new Num(1), new Num(1))) );


    CHECK( (new Var("x"))->equals(new Var("x")) );
    CHECK( ! (new Var("x"))->equals(new Num(5)) );
    CHECK( !(new Var("x"))->equals(new Var("y")) );

    
    CHECK( (new Add(new Num(8), new Num(9)))
          ->equals(new Add(new Num(8), new Num(9))) );
    CHECK( ! (new Add (new Num(8), new Num(9)))
          ->equals(new Add(new Num(8), new Num(10))) );
    CHECK( ! (new Add (new Num(8), new Num(9)))
          ->equals(new Add(new Num(10), new Num(9))) );
    CHECK( ! (new Add(new Num(8), new Num(9)))
          ->equals(new Num(8)) );
}
