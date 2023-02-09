//
//  test.cpp
//  MSDScript
//
//  Created by Lakshay Santosh Kucheriya on 2/2/23.
//

/**
* \file tests.cpp
* \brief contains all the tests forl the methods present in this project
*
* \author Lakshay Santosh Kucheriya
*/

#include "catch.h"
#include "expr.hpp"


#include <stdio.h>


// This file contains all the tests for the functions in the base class as well as each sub class.


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



TEST_CASE("to_string") {
    SECTION("Test Num to_string") {
        REQUIRE((new Num(10))->to_string() == "10");
        REQUIRE((new Num(-1))->to_string() == "-1");
    }
    
    SECTION("Test Add to_string") {
        Add *add_test1 = new Add(new Num(1), new Num(-1)); // Add(Num, Num)
        Add *add_test2 = new Add(add_test1, new Num(3)); // Add(Add, Num)
        Add *add_test3 = new Add(new Num(1), new Mult(new Num(1), new Num(2))); // Add(Num , Mult)
        Add *add_test4 = new Add(new Num(1), new Var("x")); //Add (Num, Var)
        
        REQUIRE(add_test1->to_string() == "(1+-1)");
        REQUIRE(add_test2->to_string() == "((1+-1)+3)");
        REQUIRE(add_test3->to_string() == "(1+(1*2))");
        REQUIRE(add_test4->to_string() == "(1+x)");
    }
    
    SECTION("Test Mult to_string") {
        Mult *mult_test1 = new Mult(new Num(1), new Num(-2)); // Mult (Num , Num)
        Mult *mult_test2 = new Mult(mult_test1, new Num(3)); // Mult(Mult , Num)
        Mult *mult_test3 = new Mult(new Num(1), new Add(new Num(2), new Num(3))); // Mult(Num , Add)
        Mult *mult_test4 = new Mult(new Num(1), new Var("x")); // Mult(Num , Var)
        
        
        REQUIRE(mult_test1->to_string() == "(1*-2)");
        REQUIRE(mult_test2->to_string() == "((1*-2)*3)");
        REQUIRE(mult_test3->to_string() == "(1*(2+3))");
        REQUIRE(mult_test4->to_string() == "(1*x)");
        
    }
}
    
TEST_CASE("to_pretty_string") {
    SECTION("Test Num to_pretty_string") {
        REQUIRE((new Num(10))->to_pretty_string() == "10");
    }
}

TEST_CASE("to_pretty_string again") {
    SECTION("Test Num to_pretty_string") {
        REQUIRE((new Num(10))->to_pretty_string() == "10");
    }

    SECTION("Test Add to_pretty_string") {
        // lhs: num

        // num + num
        Add *add_num_num = new Add(new Num(1), new Num(2));
        REQUIRE(add_num_num->to_pretty_string() == "1 + 2");

        // num + variable
        Add *add_num_variable = new Add(new Num(1), new Var("x"));
        REQUIRE(add_num_variable->to_pretty_string() == "1 + x");

        // num + add
        Add *add_num_add = new Add(new Num(1), add_num_num);
        REQUIRE(add_num_add->to_pretty_string() == "1 + 1 + 2");

        // num + mult
        Mult *mult_num_num = new Mult(new Num(1), new Num(2));
        Add *add_num_mult = new Add(new Num(1), mult_num_num);
        REQUIRE(add_num_mult->to_pretty_string() == "1 + 1 * 2");

        // lhs: variable

        // variable + num
        Add *add_variable_num = new Add(new Var("x"), new Num(1));
        REQUIRE(add_variable_num->to_pretty_string() == "x + 1");

        // variable + variable
        Add *add_variable_variable = new Add(new Var("x"), new Var("y"));
        REQUIRE(add_variable_variable->to_pretty_string() == "x + y");

        // variable + add
        Add *add_num_num_2 = new Add(new Num(1), new Num(2));
        Add *add_variable_add = new Add(new Var("x"), add_num_num_2);
        REQUIRE(add_variable_add->to_pretty_string() == "x + 1 + 2");

        // variable + mult
        Mult *mult_num_num_2 = new Mult(new Num(1), new Num(2));
        Add *add_variable_mult = new Add(new Var("x"), mult_num_num_2);
        REQUIRE(add_variable_mult->to_pretty_string() == "x + 1 * 2");

        // lhs: add
        Add *add_num_num_lhs = new Add(new Num(1), new Num(2));

        // add + num
        Add *add_add_num = new Add(add_num_num_lhs, new Num(3));
        REQUIRE(add_add_num->to_pretty_string() == "(1 + 2) + 3");

        // add + variable
        Add *add_add_variable = new Add(add_num_num_lhs, new Var("x"));
        REQUIRE(add_add_variable->to_pretty_string() == "(1 + 2) + x");

        // add + add
        Add *add_add_add = new Add(add_num_num_lhs, add_num_num_lhs);
        REQUIRE(add_add_add->to_pretty_string() == "(1 + 2) + 1 + 2");

        // add + mult
        Mult *mult_num_num_3 = new Mult(new Num(2), new Num(3));
        Add *add_add_mult = new Add(add_num_num_lhs, mult_num_num_3);
        REQUIRE(add_add_mult->to_pretty_string() == "(1 + 2) + 2 * 3");

        // lhs: mult
        Mult *mult_num_num_lhs = new Mult(new Num(2), new Num(3));

        // mult + num
        Add *add_mult_num = new Add(mult_num_num_lhs, new Num(1));
        REQUIRE(add_mult_num->to_pretty_string() == "2 * 3 + 1");

        // mult + variable
        Add *add_mult_variable = new Add(mult_num_num_lhs, new Var("x"));
        REQUIRE(add_mult_variable->to_pretty_string() == "2 * 3 + x");

        // mult + add
        Add *add_num_num_3 = new Add(new Num(1), new Num(2));
        Add *add_mult_add = new Add(mult_num_num_lhs, add_num_num_3);
        REQUIRE(add_mult_add->to_pretty_string() == "2 * 3 + 1 + 2");

        // mult + mult
        Add *add_mult_mult = new Add(mult_num_num_lhs, mult_num_num_lhs);
        REQUIRE(add_mult_mult->to_pretty_string() == "2 * 3 + 2 * 3");
    }

    SECTION("Test Mult to_pretty_string") {
        // lhs: num
        // num * num
        Mult *mult_num_num = new Mult(new Num(1), new Num(2));
        REQUIRE(mult_num_num->to_pretty_string() == "1 * 2");

        // num * variable
        Mult *mult_num_variable = new Mult(new Num(1), new Var("x"));
        REQUIRE(mult_num_variable->to_pretty_string() == "1 * x");

        // num * add
        Add *add_num_num = new Add(new Num(1), new Num(2));
        Mult *mult_num_add = new Mult(new Num(3), add_num_num);
        REQUIRE(mult_num_add->to_pretty_string() == "3 * (1 + 2)");

        // num * mult
        Mult *mult_num_mult = new Mult(new Num(3), mult_num_num);
        REQUIRE(mult_num_mult->to_pretty_string() == "3 * 1 * 2");

        // lhs: variable
        // variable * num
        Mult *mult_variable_num = new Mult(new Var("x"), new Num(1));
        REQUIRE(mult_variable_num->to_pretty_string() == "x * 1");

        // variable * variable
        Mult *mult_variable_variable = new Mult(new Var("x"), new Var("y"));
        REQUIRE(mult_variable_variable->to_pretty_string() == "x * y");

        // variable * add
        Add *add_num_num_2 = new Add(new Num(2), new Num(3));
        Mult *mult_variable_add = new Mult(new Var("x"), add_num_num_2);
        REQUIRE(mult_variable_add->to_pretty_string() == "x * (2 + 3)");

        // variable * mult
        Mult *mult_variable_mult = new Mult(new Var("x"), mult_num_num);
        REQUIRE(mult_variable_mult->to_pretty_string() == "x * 1 * 2");

        // lhs: add
        Add *add_num_num_lhs = new Add(new Num(1), new Num(2));

        // add * num
        Mult *mult_add_num = new Mult(add_num_num_lhs, new Num(3));
        REQUIRE(mult_add_num->to_pretty_string() == "(1 + 2) * 3");

        // add * variable
        Mult *mult_add_variable = new Mult(add_num_num_lhs, new Var("x"));
        REQUIRE(mult_add_variable->to_pretty_string() == "(1 + 2) * x");

        // add * add
        Mult *mult_add_add = new Mult(add_num_num_lhs, add_num_num_lhs);
        REQUIRE(mult_add_add->to_pretty_string() == "(1 + 2) * (1 + 2)");

        // add * mult
        Mult *mult_num_num_3 = new Mult(new Num(2), new Num(3));
        Mult *mult_add_mult = new Mult(add_num_num_lhs, mult_num_num_3);
        REQUIRE(mult_add_mult->to_pretty_string() == "(1 + 2) * 2 * 3");

        // lhs: mult
        Mult *mult_num_num_lhs = new Mult(new Num(2), new Num(3));

        // mult * num
        Mult *mult_mult_num = new Mult(mult_num_num_lhs, new Num(4));
        REQUIRE(mult_mult_num->to_pretty_string() == "(2 * 3) * 4");

        // mult * variable
        Mult *mult_mult_variable = new Mult(mult_num_num_lhs, new Var("x"));
        REQUIRE(mult_mult_variable->to_pretty_string() == "(2 * 3) * x");

        // mult * add
        Add *add_num_num_4 = new Add(new Num(1), new Num(2));
        Mult *mult_mult_add = new Mult(mult_num_num_lhs, add_num_num_4);
        REQUIRE(mult_mult_add->to_pretty_string() == "(2 * 3) * (1 + 2)");

        // mult * mult
        Mult *mult_mult_mult = new Mult(mult_num_num_lhs, mult_num_num_lhs);
        REQUIRE(mult_mult_mult->to_pretty_string() == "(2 * 3) * 2 * 3");
    }

    SECTION("Test Variable to_pretty_string") {
        REQUIRE((new Var("x"))->to_pretty_string() == "x");
    }
    
    SECTION("Few other Examples")
    {
        CHECK((new Mult( new Mult(new Num(10), new Mult(new Mult(new Num(10), new Num(10)), new Num(10))), new Mult(new Num(10), new Num(10))))
                      ->to_pretty_string()  == "(10 * (10 * 10) * 10) * 10 * 10");
    
    
    CHECK((new Add(new Mult(new Num(9), new Add(new Num(4), new Num(3))), new Add(new Mult(new Num(2), new Num(4)), new Num(1))))
                    ->to_pretty_string() == "9 * (4 + 3) + 2 * 4 + 1");
    
   // new Mult( new Mult(new Num(10), new Num(10), new Mult(new Num(10), new Num(10)))->to_pretty_string() =  "(10 * 10) * 10 * 10"

        }
    
}

