//
//  test.cpp
//  MSDScript
//
//  Created by Lakshay Santosh Kucheriya on 2/2/23.
//

#include "catch.h"
#include "expr.hpp"


#include <stdio.h>



//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
TEST_CASE( "equals" ) {
    CHECK( (new Num(1))->equals(new Num(1)) );
    CHECK( (new Num(0))->equals(new Num(-0)) );
    CHECK( (new Num(5))->equals(new Num(-5)) == false);
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

    CHECK( (new Mult(new Num(1),new Num(2)))->equals(new Add(new Num(1),new Num(2)))==false );

    CHECK( (new Num(1))->equals(new Num(1)) == true );
    CHECK( (new Var("x"))->equals(new Var("y")) == false );
    CHECK( (new Add(new Num(2),new Num(3)))->equals(new Add(new Num(2),new Num(3)))==true );
    CHECK( (new Add(new Num(2),new Num(3)))->equals(new Add(new Num(3),new Num(2)))==false );
    CHECK( (new Mult(new Num(2),new Num(2)))->equals(new Add(new Num(1),new Num(2)))==false );

}


// ------------------------------------------------------------------------------------------------------------

TEST_CASE("Tests for checking interp")
{
    CHECK_THROWS_WITH( (new Var("x"))->interp(), "interp does not work with Variable Expressions !!" );
    CHECK_THROWS_WITH( (new Var("x+y"))->interp(), "interp does not work with Variable Expressions !!" );
    CHECK_THROWS_WITH( (new Var("1"))->interp(), "interp does not work with Variable Expressions !!" );

    CHECK( (new Add(new Add(new Num(10), new Num(15)),new Add(new Num(20),new Num(20))))->interp()==65);
    CHECK( (new Add(new Add(new Num(5), new Num(5)),new Add(new Num(20),new Num(20))))->interp()!=49);
    CHECK( (new Add(new Add(new Num(-50), new Num(15)),new Add(new Num(-15),new Num(50))))->interp()==0);
    CHECK( (new Add(new Add(new Num(1000), new Num(500)),new Add(new Num(500),new Num(-1000))))->interp()==1000);
    CHECK( (new Add(new Mult(new Num(5), new Num(10)),new Mult(new Num(-5),new Num(-10))))->interp()==100);
    CHECK( (new Add(new Mult(new Num(50), new Num(1)),new Add(new Num(30),new Num(20))))->interp()==100);

    CHECK( (new Mult(new Num(3), new Num(2)))->interp()==6 );
    CHECK( (new Mult(new Num(3), new Num(0)))->interp()==0 );
    CHECK( (new Mult(new Num(6), new Num(9)))->interp()==54 );
    CHECK( (new Mult(new Num(3), new Num(-30)))->interp()==-90 );
    CHECK( (new Mult(new Add(new Num(5), new Num(5)),new Add(new Num(27),new Num(23))))->interp()==500);
    CHECK( (new Mult(new Add(new Num(-50), new Num(-50)),new Mult(new Num(0),new Num(50))))->interp()==0);
    CHECK( (new Mult(new Mult(new Num(5), new Num(5)),new Mult(new Num(-5),new Num(-5))))->interp()==625);
}

// ------------------------------------------------------------------------------------------------------------

TEST_CASE("Test for has_variable")
{
    CHECK( (new Num(1))->has_variable() == false );
    CHECK( (new Var("x"))->has_variable() == true );
    CHECK( (new Add(new Num(5),new Num(6)))->has_variable() == false );
    CHECK( (new Add(new Var("x"),new Var("y")))->has_variable() == true );
    CHECK( (new Mult(new Num(2),new Var("x")))->has_variable() == true );

}

// ------------------------------------------------------------------------------------------------------------

TEST_CASE("Test for subst")
{
    CHECK( (new Add(new Var("x"), new Num(7)))
          ->subst("x", new Var("y"))
          ->equals(new Add(new Var("y"), new Num(7))) );

    CHECK( (new Var("x"))
          ->subst("x", new Add(new Var("y"),new Num(7)))
          ->equals(new Add(new Var("y"),new Num(7))) );

    CHECK( (new Num(7))->subst("x", new Var("y"))->equals(new Num(7))==true );
    CHECK( (new Var("x"))->subst("W", new Var("y"))->equals(new Var("x"))==true );
    CHECK( (new Add(new Num(8), new Num(2)))->subst("x", new Var("y"))
          ->equals(new Add(new Num(8), new Num(2)))==true );

    CHECK( (new Num(10))->subst("MSD", new Num(3))
          ->equals(new Num(10)) );
    CHECK( (new Var("CS6014"))->subst("MSD", new Num(3))
          ->equals(new Var("CS6014")) );
    CHECK( (new Var("MSD"))->subst("MSD", new Num(3) )
          ->equals(new Num(3) ) );
    CHECK( (new Add(new Num(2), new Var("MSD")))->subst("MSD", new Num(3))
          ->equals(new Add(new Num(2), new Num(3))) );
    CHECK( (new Mult(new Num(2), new Var("MSD")))->subst("MSD", new Num(3))
          ->equals(new Mult(new Num(2), new Num(3))) );
}

// ------------------------------------------------------------------------------------------------------------

