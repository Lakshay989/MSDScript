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
#include "parse.hpp"
#include "val.hpp"
#include "pointer.h"


#include <stdio.h>


// This file contains all the tests for the functions in the base class as well as each sub class.


//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
TEST_CASE( "equals" ) {
    CHECK( (new NumExpr(1))->equals(new NumExpr(1)) );
    CHECK( (new NumExpr(0))->equals(new NumExpr(-0)) );
    CHECK( (new NumExpr(5))->equals(new NumExpr(-5)) == false);
    CHECK( ! (new NumExpr (1))->equals(new NumExpr(2)) );

    CHECK( ! (new NumExpr (1))->equals(new Mult(new NumExpr(2), new NumExpr(4))) );
    CHECK(!(new NumExpr (8))->equals(new Mult(new NumExpr(2), new NumExpr(4))) );
    CHECK(!(new NumExpr (1))->equals(new Mult(new NumExpr(1), new NumExpr(1))) );

    CHECK( (new Var("x"))->equals(new Var("x")) );
    CHECK( ! (new Var("x"))->equals(new NumExpr(5)) );
    CHECK( !(new Var("x"))->equals(new Var("y")) );

    CHECK( (new Add(new NumExpr(8), new NumExpr(9)))
          ->equals(new Add(new NumExpr(8), new NumExpr(9))) );
    CHECK( ! (new Add (new NumExpr(8), new NumExpr(9)))
          ->equals(new Add(new NumExpr(8), new NumExpr(10))) );
    CHECK( ! (new Add (new NumExpr(8), new NumExpr(9)))
          ->equals(new Add(new NumExpr(10), new NumExpr(9))) );
    CHECK( ! (new Add(new NumExpr(8), new NumExpr(9)))
          ->equals(new NumExpr(8)) );

    CHECK( (new Mult(new NumExpr(1),new NumExpr(2)))->equals(new Add(new NumExpr(1),new NumExpr(2)))==false );

    CHECK( (new NumExpr(1))->equals(new NumExpr(1)) == true );
    CHECK( (new Var("x"))->equals(new Var("y")) == false );
    CHECK( (new Add(new NumExpr(2),new NumExpr(3)))->equals(new Add(new NumExpr(2),new NumExpr(3)))==true );
    CHECK( (new Add(new NumExpr(2),new NumExpr(3)))->equals(new Add(new NumExpr(3),new NumExpr(2)))==false );
    CHECK( (new Mult(new NumExpr(2),new NumExpr(2)))->equals(new Add(new NumExpr(1),new NumExpr(2)))==false );

}


// ------------------------------------------------------------------------------------------------------------

TEST_CASE("Tests for checking interp")
{
    CHECK_THROWS_WITH( (new Var("x"))->interp(), "interp does not work with variable expressions !!" );
    CHECK_THROWS_WITH( (new Var("x+y"))->interp(), "interp does not work with variable expressions !!" );
    CHECK_THROWS_WITH( (new Var("1"))->interp(), "interp does not work with variable expressions !!" );

    CHECK( (new Add(new Add(new NumExpr(10), new NumExpr(15)),new Add(new NumExpr(20),new NumExpr(20))))->interp()->equals(new NumVal(65))) ;
    CHECK( (new Add(new Add(new NumExpr(5), new NumExpr(5)),new Add(new NumExpr(20),new NumExpr(20))))->interp()->equals(new NumVal(50)));
    CHECK( (new Add(new Add(new NumExpr(-50), new NumExpr(15)),new Add(new NumExpr(-15),new NumExpr(50))))->interp()->equals(new NumVal(0)));
    CHECK( (new Add(new Add(new NumExpr(1000), new NumExpr(500)),new Add(new NumExpr(500),new NumExpr(-1000))))->interp()->equals(new NumVal(1000)));
    CHECK( (new Add(new Mult(new NumExpr(5), new NumExpr(10)),new Mult(new NumExpr(-5),new NumExpr(-10))))->interp()->equals(new NumVal(100)));
    CHECK( (new Add(new Mult(new NumExpr(50), new NumExpr(1)),new Add(new NumExpr(30),new NumExpr(20))))->interp()->equals(new NumVal(100)));

    CHECK( (new Mult(new NumExpr(3), new NumExpr(2)))->interp()->equals(new NumVal(6)));
    CHECK( (new Mult(new NumExpr(3), new NumExpr(0)))->interp()->equals(new NumVal(0)));
    CHECK( (new Mult(new NumExpr(6), new NumExpr(9)))->interp()->equals(new NumVal(54)));
    CHECK( (new Mult(new NumExpr(3), new NumExpr(-30)))->interp()->equals(new NumVal(-90)));
    CHECK( (new Mult(new Add(new NumExpr(5), new NumExpr(5)),new Add(new NumExpr(27),new NumExpr(23))))->interp()->equals(new NumVal(500)));
    CHECK( (new Mult(new Add(new NumExpr(-50), new NumExpr(-50)),new Mult(new NumExpr(0),new NumExpr(50))))->interp()->equals(new NumVal(0)));
    CHECK( (new Mult(new Mult(new NumExpr(5), new NumExpr(5)),new Mult(new NumExpr(-5),new NumExpr(-5))))->interp()->equals(new NumVal(625)));
}

// ------------------------------------------------------------------------------------------------------------

//TEST_CASE("Test for has_variable")
//{
//    CHECK( (new NumExpr(1))->has_variable() == false );
//    CHECK( (new Var("x"))->has_variable() == true );
//    CHECK( (new Add(new NumExpr(5),new NumExpr(6)))->has_variable() == false );
//    CHECK( (new Add(new Var("x"),new Var("y")))->has_variable() == true );
//    CHECK( (new Mult(new NumExpr(2),new Var("x")))->has_variable() == true );
//
//}

// ------------------------------------------------------------------------------------------------------------

TEST_CASE("Test for subst")
{
    CHECK( (new Add(new Var("x"), new NumExpr(7)))
          ->subst("x", new Var("y"))
          ->equals(new Add(new Var("y"), new NumExpr(7))) );

    CHECK( (new Var("x"))
          ->subst("x", new Add(new Var("y"),new NumExpr(7)))
          ->equals(new Add(new Var("y"),new NumExpr(7))) );

    CHECK( (new NumExpr(7))->subst("x", new Var("y"))->equals(new NumExpr(7))==true );
    CHECK( (new Var("x"))->subst("W", new Var("y"))->equals(new Var("x"))==true );
    CHECK( (new Add(new NumExpr(8), new NumExpr(2)))->subst("x", new Var("y"))
          ->equals(new Add(new NumExpr(8), new NumExpr(2)))==true );

    CHECK( (new NumExpr(10))->subst("MSD", new NumExpr(3))
          ->equals(new NumExpr(10)) );
    CHECK( (new Var("CS6014"))->subst("MSD", new NumExpr(3))
          ->equals(new Var("CS6014")) );
    CHECK( (new Var("MSD"))->subst("MSD", new NumExpr(3) )
          ->equals(new NumExpr(3) ) );
    CHECK( (new Add(new NumExpr(2), new Var("MSD")))->subst("MSD", new NumExpr(3))
          ->equals(new Add(new NumExpr(2), new NumExpr(3))) );
    CHECK( (new Mult(new NumExpr(2), new Var("MSD")))->subst("MSD", new NumExpr(3))
          ->equals(new Mult(new NumExpr(2), new NumExpr(3))) );
}

// ------------------------------------------------------------------------------------------------------------



TEST_CASE("to_string") {
    SECTION("Test Num to_string") {
        REQUIRE((new NumExpr(10))->to_string() == "10");
        REQUIRE((new NumExpr(-1))->to_string() == "-1");
    }
    
    SECTION("Test Add to_string") {
        Add *add_test1 = new Add(new NumExpr(1), new NumExpr(-1)); // Add(Num, Num)
        Add *add_test2 = new Add(add_test1, new NumExpr(3)); // Add(Add, Num)
        Add *add_test3 = new Add(new NumExpr(1), new Mult(new NumExpr(1), new NumExpr(2))); // Add(Num , Mult)
        Add *add_test4 = new Add(new NumExpr(1), new Var("x")); //Add (Num, Var)
        
        REQUIRE(add_test1->to_string() == "(1+-1)");
        REQUIRE(add_test2->to_string() == "((1+-1)+3)");
        REQUIRE(add_test3->to_string() == "(1+(1*2))");
        REQUIRE(add_test4->to_string() == "(1+x)");
    }
    
    SECTION("Test Mult to_string") {
        Mult *mult_test1 = new Mult(new NumExpr(1), new NumExpr(-2)); // Mult (Num , Num)
        Mult *mult_test2 = new Mult(mult_test1, new NumExpr(3)); // Mult(Mult , Num)
        Mult *mult_test3 = new Mult(new NumExpr(1), new Add(new NumExpr(2), new NumExpr(3))); // Mult(Num , Add)
        Mult *mult_test4 = new Mult(new NumExpr(1), new Var("x")); // Mult(Num , Var)
        
        
        REQUIRE(mult_test1->to_string() == "(1*-2)");
        REQUIRE(mult_test2->to_string() == "((1*-2)*3)");
        REQUIRE(mult_test3->to_string() == "(1*(2+3))");
        REQUIRE(mult_test4->to_string() == "(1*x)");
        
    }
}

// ------------------------------------------------------------------------------------------------------------

    
TEST_CASE("to_pretty_string") {
    SECTION("Test Num to_pretty_string") {
        REQUIRE((new NumExpr(10))->to_pretty_string() == "10");
    }

    SECTION("Test Add to_pretty_string") {
        // lhs: num

        // num + num
        Add *add_num_num = new Add(new NumExpr(1), new NumExpr(2));
        REQUIRE(add_num_num->to_pretty_string() == "1 + 2");

        // num + variable
        Add *add_num_variable = new Add(new NumExpr(1), new Var("x"));
        REQUIRE(add_num_variable->to_pretty_string() == "1 + x");

        // num + add
        Add *add_num_add = new Add(new NumExpr(1), add_num_num);
        REQUIRE(add_num_add->to_pretty_string() == "1 + 1 + 2");

        // num + mult
        Mult *mult_num_num = new Mult(new NumExpr(1), new NumExpr(2));
        Add *add_num_mult = new Add(new NumExpr(1), mult_num_num);
        REQUIRE(add_num_mult->to_pretty_string() == "1 + 1 * 2");

        // lhs: variable

        // variable + num
        Add *add_variable_num = new Add(new Var("x"), new NumExpr(1));
        REQUIRE(add_variable_num->to_pretty_string() == "x + 1");

        // variable + variable
        Add *add_variable_variable = new Add(new Var("x"), new Var("y"));
        REQUIRE(add_variable_variable->to_pretty_string() == "x + y");

        // variable + add
        Add *add_num_num_2 = new Add(new NumExpr(1), new NumExpr(2));
        Add *add_variable_add = new Add(new Var("x"), add_num_num_2);
        REQUIRE(add_variable_add->to_pretty_string() == "x + 1 + 2");

        // variable + mult
        Mult *mult_num_num_2 = new Mult(new NumExpr(1), new NumExpr(2));
        Add *add_variable_mult = new Add(new Var("x"), mult_num_num_2);
        REQUIRE(add_variable_mult->to_pretty_string() == "x + 1 * 2");

        // lhs: add
        Add *add_num_num_lhs = new Add(new NumExpr(1), new NumExpr(2));

        // add + num
        Add *add_add_num = new Add(add_num_num_lhs, new NumExpr(3));
        REQUIRE(add_add_num->to_pretty_string() == "(1 + 2) + 3");

        // add + variable
        Add *add_add_variable = new Add(add_num_num_lhs, new Var("x"));
        REQUIRE(add_add_variable->to_pretty_string() == "(1 + 2) + x");

        // add + add
        Add *add_add_add = new Add(add_num_num_lhs, add_num_num_lhs);
        REQUIRE(add_add_add->to_pretty_string() == "(1 + 2) + 1 + 2");

        // add + mult
        Mult *mult_num_num_3 = new Mult(new NumExpr(2), new NumExpr(3));
        Add *add_add_mult = new Add(add_num_num_lhs, mult_num_num_3);
        REQUIRE(add_add_mult->to_pretty_string() == "(1 + 2) + 2 * 3");

        // lhs: mult
        Mult *mult_num_num_lhs = new Mult(new NumExpr(2), new NumExpr(3));

        // mult + num
        Add *add_mult_num = new Add(mult_num_num_lhs, new NumExpr(1));
        REQUIRE(add_mult_num->to_pretty_string() == "2 * 3 + 1");

        // mult + variable
        Add *add_mult_variable = new Add(mult_num_num_lhs, new Var("x"));
        REQUIRE(add_mult_variable->to_pretty_string() == "2 * 3 + x");

        // mult + add
        Add *add_num_num_3 = new Add(new NumExpr(1), new NumExpr(2));
        Add *add_mult_add = new Add(mult_num_num_lhs, add_num_num_3);
        REQUIRE(add_mult_add->to_pretty_string() == "2 * 3 + 1 + 2");

        // mult + mult
        Add *add_mult_mult = new Add(mult_num_num_lhs, mult_num_num_lhs);
        REQUIRE(add_mult_mult->to_pretty_string() == "2 * 3 + 2 * 3");
    }

    SECTION("Test Mult to_pretty_string") {
        // lhs: num
        // num * num
        Mult *mult_num_num = new Mult(new NumExpr(1), new NumExpr(2));
        REQUIRE(mult_num_num->to_pretty_string() == "1 * 2");

        // num * variable
        Mult *mult_num_variable = new Mult(new NumExpr(1), new Var("x"));
        REQUIRE(mult_num_variable->to_pretty_string() == "1 * x");

        // num * add
        Add *add_num_num = new Add(new NumExpr(1), new NumExpr(2));
        Mult *mult_num_add = new Mult(new NumExpr(3), add_num_num);
        REQUIRE(mult_num_add->to_pretty_string() == "3 * (1 + 2)");

        // num * mult
        Mult *mult_num_mult = new Mult(new NumExpr(3), mult_num_num);
        REQUIRE(mult_num_mult->to_pretty_string() == "3 * 1 * 2");

        // lhs: variable
        // variable * num
        Mult *mult_variable_num = new Mult(new Var("x"), new NumExpr(1));
        REQUIRE(mult_variable_num->to_pretty_string() == "x * 1");

        // variable * variable
        Mult *mult_variable_variable = new Mult(new Var("x"), new Var("y"));
        REQUIRE(mult_variable_variable->to_pretty_string() == "x * y");

        // variable * add
        Add *add_num_num_2 = new Add(new NumExpr(2), new NumExpr(3));
        Mult *mult_variable_add = new Mult(new Var("x"), add_num_num_2);
        REQUIRE(mult_variable_add->to_pretty_string() == "x * (2 + 3)");

        // variable * mult
        Mult *mult_variable_mult = new Mult(new Var("x"), mult_num_num);
        REQUIRE(mult_variable_mult->to_pretty_string() == "x * 1 * 2");

        // lhs: add
        Add *add_num_num_lhs = new Add(new NumExpr(1), new NumExpr(2));

        // add * num
        Mult *mult_add_num = new Mult(add_num_num_lhs, new NumExpr(3));
        REQUIRE(mult_add_num->to_pretty_string() == "(1 + 2) * 3");

        // add * variable
        Mult *mult_add_variable = new Mult(add_num_num_lhs, new Var("x"));
        REQUIRE(mult_add_variable->to_pretty_string() == "(1 + 2) * x");

        // add * add
        Mult *mult_add_add = new Mult(add_num_num_lhs, add_num_num_lhs);
        REQUIRE(mult_add_add->to_pretty_string() == "(1 + 2) * (1 + 2)");

        // add * mult
        Mult *mult_num_num_3 = new Mult(new NumExpr(2), new NumExpr(3));
        Mult *mult_add_mult = new Mult(add_num_num_lhs, mult_num_num_3);
        REQUIRE(mult_add_mult->to_pretty_string() == "(1 + 2) * 2 * 3");

        // lhs: mult
        Mult *mult_num_num_lhs = new Mult(new NumExpr(2), new NumExpr(3));

        // mult * num
        Mult *mult_mult_num = new Mult(mult_num_num_lhs, new NumExpr(4));
        REQUIRE(mult_mult_num->to_pretty_string() == "(2 * 3) * 4");

        // mult * variable
        Mult *mult_mult_variable = new Mult(mult_num_num_lhs, new Var("x"));
        REQUIRE(mult_mult_variable->to_pretty_string() == "(2 * 3) * x");

        // mult * add
        Add *add_num_num_4 = new Add(new NumExpr(1), new NumExpr(2));
        Mult *mult_mult_add = new Mult(mult_num_num_lhs, add_num_num_4);
        REQUIRE(mult_mult_add->to_pretty_string() == "(2 * 3) * (1 + 2)");

        // mult * mult
        Mult *mult_mult_mult = new Mult(mult_num_num_lhs, mult_num_num_lhs);
        REQUIRE(mult_mult_mult->to_pretty_string() == "(2 * 3) * 2 * 3");
    }

    SECTION("Test Variable to_pretty_string") {
        REQUIRE((new Var("x"))->to_pretty_string() == "x");
    }
    
    SECTION("Universal Examples")
    {
        CHECK((new Mult( new Mult(new NumExpr(10), new Mult(new Mult(new NumExpr(10), new NumExpr(10)), new NumExpr(10))), new Mult(new NumExpr(10), new NumExpr(10))))
              ->to_pretty_string()  == "(10 * (10 * 10) * 10) * 10 * 10");
        }
    
}


// ------------------------------------------------------------------------------------------------------------------------------------------------------------


TEST_CASE(" Tests for Let class written by me")
{
    SECTION("General Examples for Let to_pretty_string")
    {
        
        CHECK((new Let("x", new NumExpr(5), new Add(new Let("y", new NumExpr(3), new Add(new Var("y"), new NumExpr(2))), new Var("x"))))
              ->to_pretty_string() == "_let x = 5\n"
                                      "_in  (_let y = 3\n"
                                      "      _in  y + 2) + x") ;


        CHECK((new Mult(new Mult(new NumExpr (2), new Let("x", new NumExpr(5), new Add(new Var("x") , new NumExpr(1)))), new NumExpr(3)))
              ->to_pretty_string() == "(2 * _let x = 5\n"
                                      "     _in  x + 1) * 3") ;


        Let *expression1 = new Let("x", new NumExpr(5), new Add(new Let("y", new NumExpr(3), new Add(new Var("y"), new NumExpr(2))), new Var("x"))) ;

        Let *expression2 = new Let("z", new NumExpr(9), new Add(expression1, new Var("z"))) ;

        CHECK(expression2->to_pretty_string() == "_let z = 9\n"
                                                 "_in  (_let x = 5\n"
                                                 "      _in  (_let y = 3\n"
                                                 "            _in  y + 2) + x) + z") ;


        Add *expression3 = new Add(new Let("x", new NumExpr(5), new Var("x")), new NumExpr(1));
        REQUIRE(expression3->to_pretty_string() == "(_let x = 5\n"
                                                      " _in  x) + 1");

        Let *expression4 = new Let("x", new NumExpr(5), new Add(new Var("x"), new NumExpr(1)));
        Mult *expression5 = new Mult(new NumExpr(5), expression4);
        REQUIRE(expression5->to_pretty_string() == "5 * _let x = 5\n"
                                                   "    _in  x + 1");

        Add *expression6 = new Add(new NumExpr(5), expression4);
        REQUIRE(expression6->to_pretty_string() == "5 + _let x = 5\n"
                                                   "    _in  x + 1");

        Add *expression7 = new Add(expression5, new NumExpr(1));
        REQUIRE(expression7->to_pretty_string() == "5 * (_let x = 5\n"
                                                   "     _in  x + 1) + 1");


        Let *expression8 = new Let("x", new Let("y", new NumExpr(6), new Mult(new Var("y"), new NumExpr(2))), new Add(new Var("x"), new NumExpr(1)));
        REQUIRE(expression8->to_pretty_string() == "_let x = _let y = 6\n"
                                                   "         _in  y * 2\n"
                                                   "_in  x + 1");
        }
    
    
    SECTION("Test LetBinding equals")
    {
        Let *expression1 = new Let("x", new NumExpr(2), new Add(new Var("x"), new NumExpr(1)));

        REQUIRE(expression1->equals(new NumExpr(2)) == false);
        REQUIRE(expression1->equals(new Mult(new Var("x"), new NumExpr(1))) == false);

        Add *expression2 = new Add(new Var("x"), new NumExpr(3));
        Let *expression3 = new Let("x", new NumExpr(2), expression2) ;
        
        REQUIRE(expression2->equals(new Add(new Var("x"), new NumExpr(3))) == true);
        REQUIRE(expression3->equals(new Mult(new Var("x"), new NumExpr(1))) == false);
        
    }
    
    SECTION("Test LetBinding interp") {
            /*
             * 5 * (_let x = 5
                _in  x) + 1
             */
            Let *expression1 = new Let("x", new NumExpr(5), new Var("x"));
        
            Add *expression2 = new Add(new Mult(new NumExpr(5), expression1), new NumExpr(1));

            REQUIRE(expression1->interp() ->equals(new NumVal(5)));
            REQUIRE(expression2->interp() ->equals(new NumVal(26)));

        
            /*
             5 * _let x = 5
            _in  x + 1
             */

            Let *expression3 = new Let("x", new Add(new NumExpr(5), new NumExpr(2)), new Add(new Var("x"), new NumExpr(1)));
            Mult *expression4 = new Mult(new NumExpr(5), expression3);
            REQUIRE(expression4->interp()->equals(new NumVal(40)));


            // subst interp
            Add *add = new Add(new Var("x"), new Var("y"));
            Let *expression5 = new Let("x", new NumExpr(3), add);
            REQUIRE(expression5->subst("y", new NumExpr(4))->interp()->equals(new NumVal(7)));

        }

    SECTION("Test LetBinding has_variable") {
        std::string lhs = "x";

        NumExpr *num = new NumExpr(2);
        Add *exp = new Add(new NumExpr(1), new Var("y"));
        Var *variable = new Var("z");

        NumExpr *num_body = new NumExpr(2);
        Add *exp_body = new Add(new NumExpr(1), new Var("x"));
        Var *variable_body = new Var("x");

        // rhs: number
//        REQUIRE((new Let(lhs, num, num_body))->has_variable() == false);
//        REQUIRE((new Let(lhs, num, exp_body))->has_variable());
//        REQUIRE((new Let(lhs, num, variable_body))->has_variable());
//
//        // rhs: expression has variable
//        REQUIRE((new Let(lhs, exp, num_body))->has_variable());
//        REQUIRE((new Let(lhs, exp, exp_body))->has_variable());
//        REQUIRE((new Let(lhs, exp, variable_body))->has_variable());
//
//        // rhs: rhs is a variable
//        REQUIRE((new Let(lhs, variable, num_body))->has_variable());
//        REQUIRE((new Let(lhs, variable, exp_body))->has_variable());
//        REQUIRE((new Let(lhs, variable, variable_body))->has_variable());
    }

    SECTION("Test LetBinding subst") {

        /*   y = 4
          _let x = 3
         _in x + y
         */
        
        Let *expression1 = new Let("x", new NumExpr(3), new Add(new Var("x"), new Var("y")));
        Let *expression2 = new Let("x", new NumExpr(3), new Add(new Var("x"), new NumExpr(4)));

        CHECK(expression1->subst("y", new NumExpr(4))->equals(expression2) == true);
        
        CHECK(expression1->subst("y", new NumExpr(4))->to_pretty_string() == expression2->to_pretty_string()) ;

        /* subst x with 5
         _let x = 6
        _in x + 1)
        */
        Let *expression3 = new Let("x", new NumExpr(6),new Add(new Var("x"), new NumExpr(1)));
        
        REQUIRE(expression3->subst("x", new NumExpr(5))->equals(expression3));
    }

    SECTION("Test LetBinding to_string")
    {
        
        Let *expression1 = new Let("y", new NumExpr(3), new Add(new Var("y"), new NumExpr(2)));
        
        Let *expression2 = new Let("x", new NumExpr(5),new Add(expression1, new Var("x")));
        
        
        REQUIRE(expression1->to_string() == "(_let y=3 _in (y+2))");
        REQUIRE(expression2->to_string() == "(_let x=5 _in ((_let y=3 _in (y+2))+x))");
    }

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------


TEST_CASE("Pretty Print examples_Kevin") //Created from assignment examples

{
    std::stringstream out("");
    (new Let("x", new NumExpr(5), new Add(new Var("x"), new NumExpr(1))))->pretty_print(out);
    CHECK(out.str() == "_let x = 5\n"
                       "_in  x + 1");
    out.str(std::string());
    (new Add(new Let("x", new NumExpr(5), new Var("x")), new NumExpr(1)))->pretty_print(out);
    CHECK(out.str() == "(_let x = 5\n"
                       " _in  x) + 1");
    out.str(std::string());
    (new Mult(new NumExpr(5), new Let("x", new NumExpr(5), new Add(new Var("x"), new NumExpr(1)))))->pretty_print(out);
    CHECK(out.str() == "5 * _let x = 5\n"
                       "    _in  x + 1");
    out.str(std::string());
    (new Add(new Mult(new NumExpr(5), new Let("x", new NumExpr(5), new Var("x"))), new NumExpr(1)))->pretty_print(out);
    CHECK(out.str() == "5 * (_let x = 5\n"
                       "     _in  x) + 1");
    out.str(std::string());
    (new Let("x", new NumExpr(5), new Add(new Let("y", new NumExpr(3), new Add(new Var("y"), new NumExpr(2))), new Var("x"))))
            ->pretty_print(out);

    CHECK(out.str() == "_let x = 5\n"
                       "_in  (_let y = 3\n"
                       "      _in  y + 2) + x");
    out.str(std::string());
    (new Let("x", new Let("y", new NumExpr(6), new Mult(new Var("y"), new NumExpr(2))), new Add(new Var("x"), new NumExpr(1))))->pretty_print(out);
    CHECK(out.str() == "_let x = _let y = 6\n"
                       "         _in  y * 2\n"
                       "_in  x + 1");

}

TEST_CASE("pretty_print_let_mine_some_reuse_of_kevin_triple_nested_let") {
    Let* tripleNestedLet = new Let("x", new NumExpr(1),
                                   new Let("y", new NumExpr(1),
                                           new Mult(new Add(new Var("x"), new Var("y")), new Var("z"))));
    Let* tripleNestedLet2 = new Let("x", new NumExpr(1),
                                    new Let("y", new NumExpr(1),
                                            new Let("z", new Add(new Var("x"), new NumExpr(1)),
                                                    new Add(new Add(new Var("x"), new Var("y")), new Var("z")))));
    Let* tripleNestedLet3 = new Let("x", new NumExpr(1),
                                    new Let("y", new NumExpr(1),
                                            new Let("z", new Add(new Var("x"), new NumExpr(1)),
                                                    new Mult(new Add(new Var("x"), new Var("y")), new Var("z")))));


    CHECK(tripleNestedLet -> to_pretty_string() ==
          "_let x = 1\n"
          "_in  _let y = 1\n"
          "     _in  (x + y) * z"
    );
    CHECK(tripleNestedLet2 -> to_pretty_string() ==
          "_let x = 1\n"
          "_in  _let y = 1\n"
          "     _in  _let z = x + 1\n"
          "          _in  (x + y) + z"
    );
    CHECK(tripleNestedLet3 -> to_pretty_string() ==
                            "_let x = 1\n"
                            "_in  _let y = 1\n"
                            "     _in  _let z = x + 1\n"
                            "          _in  (x + y) * z"
    );
    Let* tripleNestedLet4 =new Let("x", new NumExpr(5),
                                   new Let("y", new NumExpr(3),
                                           new Add(new Var("y"), new NumExpr(2))));
    Let* tripleNestedLet5 =new Let("x", new NumExpr(5),
                                   new Add(new Let("y", new NumExpr(3),
                                                   new Add(new Var("y"), new NumExpr(2))), new Var("x")));
    std::stringstream out("");
    out.str("");
    tripleNestedLet4->pretty_print(out);
    CHECK(out.str() ==
                  "_let x = 5\n"
                  "_in  _let y = 3\n"
                  "     _in  y + 2"
    );

    CHECK(tripleNestedLet5 -> to_pretty_string() == "_let x = 5\n"
                                                          "_in  (_let y = 3\n"
                                                          "      _in  y + 2) + x");
    SECTION("assignment_examples") {
        CHECK( (new Add(new Mult(new NumExpr(5), new Let("x", new NumExpr(5), new Var("x"))), new NumExpr(1)))-> to_pretty_string()
               == "5 * (_let x = 5\n"
                  "     _in  x) + 1");
        CHECK( (new Mult(new Mult(new NumExpr (2), new Let("x", new NumExpr(5), new Add(new Var("x") ,new  NumExpr(1)) )), new NumExpr(3) )) -> to_pretty_string()
               == "(2 * _let x = 5\n"
                  "     _in  x + 1) * 3");
    }
    // A _let needs parentheses when it is nested immediately as the right argument of an unparenthesized *
    // where _let would have needed parentheses in the surrounding context
    // (that is, if the _let used in place of the whole * would need parentheses,
    // then it still needs parentheses within the right-hand size of *).
    SECTION("new_edge") {
        CHECK( (new Mult(new NumExpr (2), new Let("x", new NumExpr(5), new Add(new Var("x") ,new  NumExpr(1)) ) )) -> to_pretty_string()
               == "2 * _let x = 5\n"
                  "    _in  x + 1");
        CHECK( (new Add(new Mult(new NumExpr(5), new Let("x", new NumExpr(5), new Mult(new Var("x"), new NumExpr(2)))), new NumExpr(1)))-> to_pretty_string()
               == "5 * (_let x = 5\n"
                  "     _in  x * 2) + 1");
        CHECK( (new Mult((new Add(new Mult(new NumExpr(5), new Let("x", new NumExpr(5), new Mult(new Var("x"), new NumExpr(2)))), new NumExpr(1))), new NumExpr(7)))-> to_pretty_string()
               == "(5 * (_let x = 5\n"
                  "      _in  x * 2) + 1) * 7");
        CHECK( (new Let("x", new NumExpr(10), new Mult( new Mult(new Var("x"), new Mult(new Mult(new NumExpr(10), new NumExpr(10)), new NumExpr(10))), new Mult(new NumExpr(10), new NumExpr(10)))))
                       ->to_pretty_string()  == "_let x = 10\n"
                                                      "_in  (x * (10 * 10) * 10) * 10 * 10");
        CHECK( (new Let("x", new NumExpr(1), new Mult( new Mult(new Var("x"), new Mult(new Mult(new NumExpr(10), new NumExpr(10)), new Var("x"))), new Mult(new NumExpr(10), new NumExpr(10)))))
                        -> to_pretty_string() == "_let x = 1\n"
                                                       "_in  (x * (10 * 10) * x) * 10 * 10" );
        CHECK( (new Let("x", new NumExpr(1), new Mult( new Mult(new Var("x"), new Mult(new Mult(new NumExpr(10), new NumExpr(10)), new Var("x"))), new Mult(new Var("y"), new NumExpr(10)))))
                       -> to_pretty_string() == "_let x = 1\n"
                                                      "_in  (x * (10 * 10) * x) * y * 10" );
    }
}

TEST_CASE("Let_equals_mine") {
        SECTION("Values_same") {
            REQUIRE( (new Let("x", new NumExpr(4), new Add(new NumExpr(2), new Var("x")) ))->equals(new Let("x", new NumExpr(4), new Add(new NumExpr(2), new Var("x")) )));
        }
        SECTION("Values_same_different_rhs") {
            REQUIRE( !(new Let("x", new NumExpr(4), new Add(new NumExpr(2), new Var("x")) ))->equals(new Let("x", new NumExpr(5), new Add(new NumExpr(2), new Var("x")) )));
        }
        SECTION("Values_same_different_lhs") {
            REQUIRE( !(new Let("x", new NumExpr(4), new Add(new NumExpr(2), new Var("x")) ))->equals(new Let("y", new NumExpr(4), new Add(new NumExpr(2), new Var("x")) )));
        }
        SECTION("Values_same_different_body") {
            REQUIRE( !(new Let("x", new NumExpr(4), new Add(new NumExpr(2), new Var("x")) ))->equals(new Let("x", new NumExpr(4), new Mult(new NumExpr(3), new Var("y")) )));
        }
        SECTION("different_types") {
            REQUIRE( !(new Let("x", new NumExpr(4), new Add(new NumExpr(2), new Var("x")) ))->equals( new Mult(new NumExpr(3), new Var("y")) ));
        }
}
//TEST_CASE("Let_has_variable_mine") {
//    SECTION("has") {
//        REQUIRE( (new Let("x", new NumExpr(4), new Add(new NumExpr(2), new Var("x")) ))->has_variable());
//    }
//    SECTION("does_not_has") {
//        REQUIRE( !(new Let("x", new NumExpr(4), new Add(new NumExpr(2), new NumExpr(4)) ))->has_variable());
//    }
//}
TEST_CASE("Let_print_mine") {
    CHECK( (new Let("x", new NumExpr(5), new Add(new Let("y", new NumExpr(3), new Add(new Var("y"), new NumExpr(2))), new Var("x")))) -> to_string()
                                                                                                        == "(_let x=5 _in ((_let y=3 _in (y+2))+x))");
    CHECK( (new Let("x", new NumExpr(1),
                   new Let("y", new NumExpr(1),
                           new Let("z", new Add(new Var("x"), new NumExpr(1)),
                                   new Mult(new Add(new Var("x"), new Var("y")), new Var("z")))))) -> to_string()
                                                                                                        == "(_let x=1 _in (_let y=1 _in (_let z=(x+1) _in ((x+y)*z))))");
}
TEST_CASE ("Let_interp_mine") {
    SECTION("hw_examples") {
        CHECK((new Add(new Mult(new NumExpr(5), new Let("x", new NumExpr(5), new Var("x"))), new NumExpr(1))) -> interp()->equals(new NumVal(26)));
        CHECK((new Mult(new NumExpr(5), new Let("x", new NumExpr(5), new Add(new Var("x"), new NumExpr(1))))) -> interp()->equals(new NumVal(30)));
    }
    SECTION("from_pretty_print_edge") {
        CHECK( (new Let("x", new NumExpr(1),
                       new Let("y", new NumExpr(1),
                               new Let("z", new Add(new Var("x"), new NumExpr(1)),
                                       new Mult(new Add(new Var("x"), new Var("y")), new Var("z")))))) -> interp()->equals(new NumVal(4)));
        CHECK( (new Mult((new Add(new Mult(new NumExpr(5), new Let("x", new NumExpr(5), new Mult(new Var("x"), new NumExpr(2)))), new NumExpr(1))), new NumExpr(7))) -> interp() ->equals(new NumVal(357))); // 51 * 7
        CHECK( (new Let("x", new NumExpr(10), new Mult( new Mult(new Var("x"), new Mult(new Mult(new NumExpr(10), new NumExpr(10)), new NumExpr(10))), new Mult(new NumExpr(10), new NumExpr(10)))))
                      ->interp()->equals(new NumVal(1000000)));
        CHECK( (new Let("x", new NumExpr(1), new Mult( new Mult(new Var("x"), new Mult(new Mult(new NumExpr(10), new NumExpr(10)), new Var("x"))), new Mult(new NumExpr(10), new NumExpr(10)))))
                       ->interp()->equals(new NumVal(10000)));
        CHECK_THROWS_WITH( ((new Let("x", new NumExpr(1), new Mult( new Mult(new Var("x"), new Mult(new Mult(new NumExpr(10), new NumExpr(10)), new Var("x"))), new Mult(new Var("y"), new NumExpr(10)))))
                       -> interp()->equals(new NumVal(10000))), "interp does not work with variable expressions !!");
    }
    SECTION("bypass_middle_let") {
        CHECK ((new Let("x", new NumExpr(2), new Let("z", new NumExpr(4), new Add(new Var("x"), new NumExpr(10)))))
                -> interp()->equals(new NumVal(12)));
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------


TEST_CASE("Provided tests for parse ") {
  CHECK_THROWS_WITH( parse_expression_str("()"), "invalid input" );
  
  CHECK( parse_expression_str("(1)")->equals(new NumExpr(1)) );
  CHECK( parse_expression_str("(((1)))")->equals(new NumExpr(1)) );
  
  CHECK_THROWS_WITH( parse_expression_str("(1"), "missing close parenthesis" );
  
  CHECK( parse_expression_str("1")->equals(new NumExpr(1)) );
  CHECK( parse_expression_str("10")->equals(new NumExpr(10)) );
  CHECK( parse_expression_str("-3")->equals(new NumExpr(-3)) );
  CHECK( parse_expression_str("  \n 5  ")->equals(new NumExpr(5)) );
  CHECK_THROWS_WITH( parse_expression_str("-"), "number should come right after -" );
  // This was some temporary debugging code:
  //  std::istringstream in("-");
  //  parse_num(in)->print(std::cout); std::cout << "\n";
  
  CHECK_THROWS_WITH( parse_expression_str(" -   5  "), "number should come right after -" );
  
  CHECK( parse_expression_str("x")->equals(new Var("x")) );
  CHECK( parse_expression_str("xyz")->equals(new Var("xyz")) );
  CHECK( parse_expression_str("xYz")->equals(new Var("xYz")) );
  CHECK_THROWS_WITH( parse_expression_str("x_z"), "unexpected character in variable" );
  
  CHECK( parse_expression_str("x + y")->equals(new Add(new Var("x"), new Var("y"))) );
  CHECK( parse_expression_str("x * y")->equals(new Mult(new Var("x"), new Var("y"))) );
  CHECK( parse_expression_str("z * x + y")
        ->equals(new Add(new Mult(new Var("z"), new Var("x")),
                         new Var("y"))) );
  
  CHECK( parse_expression_str("z * (x + y)")
        ->equals(new Mult(new Var("z"),
                          new Add(new Var("x"), new Var("y"))) ));
}

TEST_CASE( "parse" ) {
CHECK((new NumExpr(1))->equals(new NumExpr(1)));
CHECK(!(new NumExpr(1))->equals(new NumExpr(2)));
CHECK(!(new NumExpr(1))->equals(new Mult (new NumExpr(2), new NumExpr(4))));
CHECK((new Var("hello"))->equals(new Var("hello")));
CHECK(!(new Var("hello"))->equals(new Var("ello")));
CHECK(parse_expression_str("1")->equals(new NumExpr(1)));
CHECK(parse_expression_str("  1")->equals(new NumExpr(1)));
CHECK(parse_expression_str("4+2")->equals(new Add(new NumExpr(4), new NumExpr(2))));
CHECK(parse_expression_str("   45 +    23")->equals(new Add(new NumExpr(45), new NumExpr(23))));
CHECK(parse_expression_str("   45   *  23   ")->equals(new Mult(new NumExpr(45), new NumExpr(23))));
CHECK(parse_expression_str("(90)")->equals(new NumExpr(90)));
CHECK(parse_expression_str("3*(2+43)")->equals(new Mult(new NumExpr(3),new Add(new NumExpr(2), new NumExpr(43)))));
CHECK(parse_expression_str("Hello")->equals(new Var("Hello")));
CHECK(parse_expression_str("3*(2+width)")->equals(new Mult(new NumExpr(3),new Add(new NumExpr(2), new Var("width")))));
}

TEST_CASE("Test cases for parse") {
    REQUIRE_THROWS_WITH(parse_expression_str("()"), "invalid input");

    REQUIRE(parse_expression_str("    1     ")->equals(new NumExpr(1)));
    REQUIRE(parse_expression_str("(   1   )")->equals(new NumExpr(1)));
    REQUIRE(parse_expression_str("(1)")->equals(new NumExpr(1)));
    REQUIRE(parse_expression_str("(((1)))")->equals(new NumExpr(1)));


    REQUIRE_THROWS_WITH(parse_expression_str("(1"), "missing close parenthesis");
    REQUIRE_THROWS_WITH(parse_expression_str("( 1    "), "missing close parenthesis");

    REQUIRE(parse_expression_str("1")->equals(new NumExpr(1)));
    REQUIRE(parse_expression_str("10")->equals(new NumExpr(10)));
    REQUIRE(parse_expression_str("-3")->equals(new NumExpr(-3)));

    REQUIRE(parse_expression_str("  \n 5  ")->equals(new NumExpr(5)));

    REQUIRE_THROWS_WITH(parse_expression_str("-"), "number should come right after -");
    REQUIRE_THROWS_WITH(parse_expression_str(" -   5  "), "number should come right after -");

    REQUIRE(parse_expression_str(" ( 2 + 3) * 5 + 1 ")->interp()->equals(new NumVal(26)));
    REQUIRE(parse_expression_str(" 2 + 3  * 5 + 1 ")->interp()->equals(new NumVal(18)));
    REQUIRE(parse_expression_str(" 2 + 3  * (5 + 1 )")->interp()->equals(new NumVal(20)));

    REQUIRE_THROWS_WITH(parse_expression_str(" 1 - 3")->interp(), "invalid input");
    REQUIRE_THROWS_WITH(parse_expression_str(" 1 - ")->interp(), "invalid input");

    SECTION("variable") {

        REQUIRE(parse_expression_str("x")->equals(new Var("x")));
        REQUIRE(parse_expression_str("xyz")->equals(new Var("xyz")));
        REQUIRE(parse_expression_str("xYz")->equals(new Var("xYz")));

        REQUIRE_THROWS_WITH(parse_expression_str("x_z"), "unexpected character in variable");

        REQUIRE(parse_expression_str("x + y")->equals(new Add(new Var("x"), new Var("y"))));

        REQUIRE(parse_expression_str("x * y")->equals(new Mult(new Var("x"), new Var("y"))));

        REQUIRE(parse_expression_str("z * x + y")
                        ->equals(new Add(new Mult(new Var("z"), new Var("x")),
                                         new Var("y"))));

        REQUIRE(parse_expression_str("z * (x + y)")
                        ->equals(new Mult(new Var("z"),
                                          new Add(new Var("x"), new Var("y")))));
    }

    SECTION("let binding") {
        Add *expression1 = new Add(new Var("x"), new NumExpr(1));
        Let *expression2 = new Let("x", new NumExpr(5), expression1);
        REQUIRE(parse_expression_str("_let x = 5 _in x + 1")->equals(expression2));

        Let *expression3 = new Let("x", new Add(new Var("x"), new NumExpr(1)), new Add(new Var("x"), new NumExpr(7)));
        Let *expression4 = new Let("x", new NumExpr(5), expression3);
        REQUIRE(parse_expression_str("_let x = 5 _in _let x = x +1    _in  x + 7")->equals(expression4));

        REQUIRE_THROWS_WITH(parse_expression_str("_lt x = 5 _in x + 1"), "consume mismatch");
        REQUIRE_THROWS_WITH(parse_expression_str("_let x  5 _in x + 1"), "invalid input");
        REQUIRE_THROWS_WITH(parse_expression_str("_let x = 5 _i x + 1"), "invalid input");
        REQUIRE_THROWS_WITH(parse_expression_str("_let x = 5 x + 1"), "invalid input");
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("NumVal") {
    SECTION("add_to") {
        auto *num1 = new NumVal(23);
        auto *num2 = new NumVal(27);
        auto *num3 = new NumVal(50);
        
        auto add1 = num1->add_to(num2);
        auto add2 = num2->add_to(num1);

        REQUIRE(add1->equals(num3));
        REQUIRE(add2->equals(num3));

        REQUIRE_THROWS_WITH(num3->add_to(nullptr), "add of non-number");
    }

    SECTION("mult_with") {
        auto *num1 = new NumVal(20);
        auto *num2 = new NumVal(5);
        auto *num3 = new NumVal(100);
        
        auto mult1 = num1->mult_with(num2);
        auto mult2 = num2->mult_with(num1);

        REQUIRE(mult1->equals(num3));
        REQUIRE(mult2->equals(num3));

        REQUIRE_THROWS_WITH(num3->mult_with(nullptr), "mult with non-number");
    }

    SECTION("equals") {
        auto *num1 = new NumVal(2);
        auto *num2 = new NumVal(3);
        auto *num3 = new NumVal(2);

        REQUIRE(num1->equals(num2) == false);
        REQUIRE(num1->equals(num3));
        REQUIRE(num1->equals(num1));
        REQUIRE(num1->equals(nullptr) == false);
    }

    SECTION("to_string") {
        auto *num1 = new NumVal(2);
        auto *num2 = new NumVal(-2);

        REQUIRE(num1->to_string() == std::to_string(2));
        REQUIRE(num2->to_string() == std::to_string(-2));
    }

    SECTION("to_expr") {
        auto *num1 = new NumVal(2);
        auto *num2 = new NumVal(-2);

        auto *num_expr1 = new NumExpr(2);
        auto *num_expr2 = new NumExpr(-2);

        REQUIRE(num1->to_expr()->equals(num_expr1));
        REQUIRE(num2->to_expr()->equals(num_expr2));
        REQUIRE(num1->to_expr()->equals(num_expr2) == false);
        REQUIRE(num2->to_expr()->equals(num_expr1) == false);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------


TEST_CASE("Bool Val") {

    auto *bool_false = new BoolVal(false);
    auto *bool_true = new BoolVal(true);
    auto *bool_true_1 = new BoolVal(true);
    auto *num = new NumVal(1);

    SECTION("add to") {
        REQUIRE_THROWS_WITH(bool_true->add_to(bool_true), "Addition to a boolean is not possible");
        REQUIRE_THROWS_WITH(bool_true->add_to(num), "Addition to a boolean is not possible");
    }

    SECTION("mult with") {
        REQUIRE_THROWS_WITH(bool_true->mult_with(bool_true), "Multiplication with a boolean is not possible");
        REQUIRE_THROWS_WITH(bool_true->mult_with(num), "Multiplication with a boolean is not possible");
    }

    SECTION("equals") {
        REQUIRE(bool_true->equals(nullptr) == false);
        REQUIRE(bool_true->equals(bool_false) == false);
        REQUIRE(bool_true->equals(num) == false);
        REQUIRE(bool_true->equals(bool_true_1));
    }

    SECTION("is_true") {
        REQUIRE(bool_true->is_true());
        REQUIRE(bool_false->is_true() == false);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------


TEST_CASE("Bool Expr") {

    auto *bool_true = new BoolExpr(true);
    auto *bool_true_1 = new BoolExpr(true);
    auto *bool_false = new BoolExpr(false);


    SECTION("equals") {
        REQUIRE(bool_true->equals(bool_true_1));
        REQUIRE(bool_false->equals(bool_true) == false);

        auto *num_2 = new NumExpr(2);
        REQUIRE(bool_true->equals(num_2) == false);

        auto *add_2_3 = new Add(new NumExpr(2), new NumExpr(3));
        REQUIRE(bool_true->equals(add_2_3) == false);

        auto *mult_2_3 = new Mult(new NumExpr(2), new NumExpr(3));
        REQUIRE(bool_true->equals(mult_2_3) == false);

        auto *var_x = new Var("x");
        REQUIRE(bool_true->equals(var_x) == false);

        auto *lb_x_5_x = new Let("x", new NumExpr(2), new Var("x"));
        REQUIRE(bool_true->equals(lb_x_5_x) == false);
    }

    SECTION("interp") {
        REQUIRE(bool_true->interp()->equals(new BoolVal(true)));
        REQUIRE(bool_false->interp()->equals(new BoolVal(false)));
    }

//    SECTION("has_variable") {
//        REQUIRE(bool_true->has_variable() == false);
//        REQUIRE(bool_false->has_variable() == false);
//    }

    SECTION("subst") {
        REQUIRE(bool_true->subst("x", new BoolExpr(true))->equals(bool_true_1));
    }

    SECTION("print") {
        REQUIRE(bool_true->to_string() == "_true");
        REQUIRE(bool_false->to_string() == "_false");
    }

    SECTION("pretty_print") {
        REQUIRE(bool_true->to_pretty_string() == "_true");
        REQUIRE(bool_false->to_pretty_string() == "_false");
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------


TEST_CASE("If Expr") {

    auto *add_expr = new Add(new NumExpr(3), new NumExpr(4));
    auto *mult_expr = new Mult(new NumExpr(1), new NumExpr(2));
    // x = 2, 1 + x => 3
    auto *let_add_expr = new Let("x", new NumExpr(2), new Add(new NumExpr(1), new Var("x")));
    // (_if _true _then 2 _else (3 + 4))
    auto *if_true_then_num_else_add_expr = new IfExpr(new BoolExpr(true), new NumExpr(2), add_expr);
    auto *if_true_then_num_else_add_expr1 = new IfExpr(new BoolExpr(true), new NumExpr(2), add_expr);
    auto *if_false_then_num_else_add_expr = new IfExpr(new BoolExpr(false), new NumExpr(2), add_expr);
    auto *if_true_then_mult_else_add_expr = new IfExpr(new BoolExpr(true), mult_expr, add_expr);
    auto *if_true_then_num_else_let_expr = new IfExpr(new BoolExpr(true), new NumExpr(2), let_add_expr);


    SECTION("equals") {
        REQUIRE(if_true_then_num_else_add_expr->equals(if_true_then_num_else_add_expr1));
        REQUIRE(if_true_then_num_else_add_expr->equals(if_false_then_num_else_add_expr) == false);
        REQUIRE(if_true_then_num_else_add_expr->equals(if_true_then_mult_else_add_expr) == false);
        REQUIRE(if_true_then_num_else_add_expr->equals(if_true_then_num_else_let_expr) == false);
    }

    auto *if_false_then_2_else_lb = new IfExpr(new BoolExpr(false), new NumExpr(2), let_add_expr);


    SECTION("interp") {
        REQUIRE(if_true_then_num_else_add_expr->interp()->equals(new NumVal(2)));
        REQUIRE(if_false_then_num_else_add_expr->interp()->equals(add_expr->interp()));
        REQUIRE(if_true_then_mult_else_add_expr->interp()->equals(mult_expr->interp()));
        REQUIRE(if_false_then_2_else_lb->interp()->equals(let_add_expr->interp()));
    }

//    SECTION("has_variable") {
//        REQUIRE(if_true_then_num_else_add_expr->has_variable() == false);
//        REQUIRE(if_false_then_num_else_add_expr->has_variable() == false);
//        REQUIRE(if_true_then_mult_else_add_expr->has_variable() == false);
//        REQUIRE(if_true_then_num_else_let_expr->has_variable());
//    }

    auto *add_x_1 = new Add(new Var("x"), new NumExpr(1));
    // x + 1
    auto *if_true_then_add_x_1_else_3 = new IfExpr(new BoolExpr(true), add_x_1, new NumExpr(3));
    auto *add_3_1 = new Add(new NumExpr(3), new NumExpr(1));
    auto *if_true_then_add_3_1_else_3 = new IfExpr(new BoolExpr(true), add_3_1, new NumExpr(3));

    SECTION("subst") {
        REQUIRE(if_true_then_num_else_add_expr->subst("x", new NumExpr(3))->equals(if_true_then_num_else_add_expr));
        REQUIRE(if_false_then_2_else_lb->subst("x", new NumExpr(3))->equals(if_false_then_2_else_lb));
        REQUIRE(if_true_then_add_x_1_else_3->subst("x", new NumExpr(3))->equals(if_true_then_add_3_1_else_3));
        REQUIRE(if_true_then_add_x_1_else_3->subst("y", new NumExpr(2))->equals(if_true_then_add_x_1_else_3));
    }

    auto *if_bool_expr_true_then_2_else_3 = new IfExpr(new BoolExpr(true), new NumExpr(2), new NumExpr(3));


    SECTION("print") {
        REQUIRE(if_true_then_num_else_add_expr->to_string() == "(_if _true _then 2 _else (3+4))");
        REQUIRE(if_false_then_2_else_lb->to_string() == "(_if _false _then 2 _else (_let x=2 _in (1+x)))");
        REQUIRE(if_true_then_add_x_1_else_3->to_string() == "(_if _true _then (x+1) _else 3)");

        REQUIRE(if_bool_expr_true_then_2_else_3->to_string() == "(_if _true _then 2 _else 3)");
    }
    
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("Eq Expr")
{
    auto *num2 = new NumExpr(2);
    auto *num3 = new NumExpr(3);
    auto *add_expr = new Add(num2, num3);
    auto *eq_expr = new EqExpr(num2, num3);
    // 2 == 2 + 3
    auto *eq_num_add = new EqExpr(num2, add_expr);
    auto *eq_num_num = new EqExpr(num2, new NumExpr(4));
    
    SECTION("equals") {
            REQUIRE(eq_expr->equals(eq_num_add) == false);
            REQUIRE(eq_expr->equals(eq_num_num) == false);
            REQUIRE(eq_expr->equals(eq_expr));
            REQUIRE(eq_expr->equals(num2) == false);
        }

        auto *eq_num_2_num_2 = new EqExpr(num2, num2);
        auto *eq_num_1_bool_false = new EqExpr(new NumExpr(1), new BoolExpr(false));

    /*    _let same = 1 == 2
        _in  _if 1 == 2
        _then _false + 5
        _else 88*/
        auto *eq_num_1_num_2 = new EqExpr(new NumExpr(1), num2);
        auto *add_bool_false_num_5 = new Add(new BoolExpr(false), new NumExpr(5));
        auto *if_eq_then_add_else_88 = new IfExpr(eq_num_1_num_2, add_bool_false_num_5, new NumExpr(88));
        auto *let_same_eq_in_if = new Let("same", eq_num_1_num_2, if_eq_then_add_else_88);

        /* _if 4 + 1
         _then 2
         _else 3*/
        auto *add_num_4_num_1 = new Add(new NumExpr(4), new NumExpr(1));
        auto *if_add_then_2_else_3 = new IfExpr(add_num_4_num_1, new NumExpr(2), new NumExpr(3));

        // (1 == 2) + 3
        auto *add_eq_1_2_num_3 = new Add(eq_num_1_num_2, new NumExpr(3));

        auto *eq_num_1_add_2_3 = new EqExpr(new NumExpr(1), new Add(new NumExpr(2), new NumExpr(3)));

        // 1+1 == 2+0
        auto *add_num_1_num_1 = new Add(new NumExpr(1), new NumExpr(1));
        auto *add_num_2_num_0 = new Add(new NumExpr(2), new NumExpr(0));
        auto *eq_add_1_add_2 = new EqExpr(add_num_1_num_1, add_num_2_num_0);

        SECTION("interp") {
            REQUIRE(eq_expr->interp()->is_true() == false);
            REQUIRE(eq_num_2_num_2->interp()->is_true());
            REQUIRE(eq_num_add->interp()->is_true() == false);
            REQUIRE(eq_num_1_bool_false->interp()->is_true() == false);
            REQUIRE(let_same_eq_in_if->interp()->equals(new NumVal(88)));
            REQUIRE_THROWS_WITH(if_add_then_2_else_3->interp(), "A num val can not be a bool val");
            REQUIRE_THROWS_WITH(add_eq_1_2_num_3->interp(), "Addition to a boolean is not possible");
            REQUIRE(eq_num_1_add_2_3->interp()->is_true() == false);
            REQUIRE(eq_add_1_add_2->interp()->is_true());
            REQUIRE(eq_add_1_add_2->interp()->to_string() == "_true");
            
            REQUIRE_THROWS_WITH(parse_expression_str("_let x = _true + 1\n"
                                       "_in  _if _true\n"
                                       "     _then 5\n"
                                       "     _else x")->interp(), "Addition to a boolean is not possible");
        }

        auto *eq_var_x_num_2 = new EqExpr(new Var("x"), num2);
        auto *eq_add_x_2_num_2 = new EqExpr(new Add(new Var("x"), num2), num2);
        // _let x=2 _in x+1
        auto *lb_x_2_add_x_1 = new Let("x", new NumExpr(2), new Add(new Var("x"), new NumExpr(1)));
        // _let x=2 _in x+1 == 2
        auto *eq_lb_num = new EqExpr(lb_x_2_add_x_1, num2);

//        SECTION("has_variable") {
//            REQUIRE(eq_expr->has_variable() == false);
//            REQUIRE(eq_var_x_num_2->has_variable());
//            REQUIRE(eq_add_x_2_num_2->has_variable());
//            REQUIRE(eq_lb_num->has_variable());
//        }

        auto *eq_add_1_2_num_2 = new EqExpr(new Add(new NumExpr(1), new NumExpr(2)), new NumExpr(2));

        SECTION("subst") {
            REQUIRE(eq_expr->subst("x", new NumExpr(1))->equals(eq_expr));
            REQUIRE(eq_var_x_num_2->subst("x", new NumExpr(1))->equals(eq_num_1_num_2));
            REQUIRE(eq_add_x_2_num_2->subst("x", new NumExpr(1))->equals(eq_add_1_2_num_2));
            REQUIRE(eq_lb_num->subst("x", new NumExpr(1))->equals(eq_lb_num));
        }

        SECTION("print") {
            REQUIRE(eq_num_1_num_2->to_string() == "(1==2)");
            REQUIRE(eq_num_add->to_string() == "(2==(2+3))");
            REQUIRE(eq_lb_num->to_string() == "((_let x=2 _in (x+1))==2)");
        }

        SECTION("pretty_print") {
            // left: num / var / bool
            REQUIRE(eq_num_1_num_2->to_pretty_string() == "1 == 2");
            REQUIRE(eq_num_add->to_pretty_string() == "2 == 2 + 3");
            REQUIRE(eq_lb_num->to_pretty_string() == "(_let x = 2\n"
                                                     " _in  x + 1) == 2");
            REQUIRE(if_add_then_2_else_3->to_pretty_string() == "_if 4 + 1\n"
                                                                "_then 2\n"
                                                                "_else 3");
        }

        SECTION("to_string") {
            REQUIRE(parse_expression_str("1==2+3")->to_string() == "(1==(2+3))");
            REQUIRE(parse_expression_str("(1==2)+3")->to_string() == "((1==2)+3)");
            REQUIRE(parse_expression_str("1+1 == 2+0")->to_string() == "((1+1)==(2+0))");
        }

        SECTION("to_pretty_string")
        {
            REQUIRE(parse_expression_str("1==2+3")->to_pretty_string() == "1 == 2 + 3");
            REQUIRE(parse_expression_str("(1==2)+3")->to_pretty_string() == "(1 == 2) + 3");
            REQUIRE(parse_expression_str("1+1 == 2+0")->to_pretty_string() == "1 + 1 == 2 + 0");
            REQUIRE(let_same_eq_in_if->to_pretty_string() == "_let same = 1 == 2\n"
                                                             "_in  _if 1 == 2\n"
                                                             "     _then _false + 5\n"
                                                             "     _else 88");
        }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("Fun Expr") {
    auto *add_x_1 = new Add(new Var("x"), new NumExpr(1));
    auto *add_x_2 = new Add(new Var("x"), new NumExpr(2));
    auto *fun_expr_x_add_x_1 = new FunExpr("x", add_x_1);
    auto *fun_expr_x_add_x_1_copy = new FunExpr("x", add_x_1);
    auto *fun_expr_y_add_x_1 = new FunExpr("y", add_x_1);
    auto *fun_expr_x_add_x_2 = new FunExpr("x", add_x_2);
    auto *num_expr_5 = new NumExpr(5);

    // (_fun (y) (_fun (x) y + x))
    auto add_y_x = new Add(new Var("y"), new Var("x"));
    auto *fun_expr_x_add_y_x = new FunExpr("x", add_y_x);
    auto *fun_expr_y_fun_var_x_add_y_x = new FunExpr("y", fun_expr_x_add_y_x);
    auto *fun_var_y_fun_var_x_add_y_x = new FunVal("y", fun_expr_x_add_y_x);


    SECTION("equals") {
        REQUIRE(fun_expr_x_add_x_1->equals(fun_expr_x_add_x_1_copy));
        REQUIRE(fun_expr_y_add_x_1->equals(fun_expr_x_add_x_1) == false);
        REQUIRE(fun_expr_x_add_x_2->equals(fun_expr_x_add_x_1) == false);
        REQUIRE(fun_expr_x_add_x_1->equals(num_expr_5) == false);
    }

    auto *fun_val_x_add_x_1 = new FunVal("x", add_x_1);

    SECTION("interp") {
        REQUIRE(fun_expr_x_add_x_1->interp()->equals(fun_val_x_add_x_1));
        REQUIRE(fun_expr_y_fun_var_x_add_y_x->interp()->equals(fun_var_y_fun_var_x_add_y_x));
    }

    auto *fun_expr_x_add_x_1_subst_x_2 = fun_expr_x_add_x_1->subst("x", new NumExpr(2));
    auto *add_2_1 = new Add(new NumExpr(2), new NumExpr(1));
    auto *fun_expr_x_add_2_1_subst_x_2 = new FunExpr("x", add_2_1);
    auto *fun_expr_x_add_x_1_subst_y_2 = fun_expr_x_add_x_1->subst("y", new NumExpr(2));
    auto *fun_val_x_add_2_1 = new FunVal("x", add_2_1);

    SECTION("subst") {
        REQUIRE(fun_expr_x_add_x_1_subst_x_2->equals(fun_expr_x_add_x_1));
        REQUIRE(fun_expr_x_add_x_1_subst_y_2->equals(fun_expr_x_add_x_1));
    }

    SECTION("print") {
        REQUIRE(fun_expr_x_add_x_1->to_string() == "(_fun(x)(x+1))");
        REQUIRE(fun_expr_y_fun_var_x_add_y_x->to_string() == "(_fun(y)(_fun(x)(y+x)))");
    }

    SECTION("pretty_print") {
        REQUIRE(fun_expr_x_add_x_1->to_pretty_string() == "_fun (x)\n"
                                                          "  x + 1");
        REQUIRE(fun_expr_y_fun_var_x_add_y_x->to_pretty_string() == "_fun (y)\n"
                                                                    "  _fun (x)\n"
                                                                    "    y + x");
    }
}

TEST_CASE("Call Expr") {
    auto *add_x_1 = new Add(new Var("x"), new NumExpr(1));
    auto *add_x_2 = new Add(new Var("x"), new NumExpr(2));
    auto *fun_expr_x_add_x_1 = new FunExpr("x", add_x_1);
    auto *call_fun_expr_x_add_x_1_on_2 = new CallExpr(fun_expr_x_add_x_1, new NumExpr(2));
    auto *call_fun_expr_x_add_x_1_on_2_copy = new CallExpr(fun_expr_x_add_x_1, new NumExpr(2));
    auto *call_fun_expr_x_add_x_1_on_3 = new CallExpr(fun_expr_x_add_x_1, new NumExpr(3));
    auto *fun_expr_x_add_x_2 = new FunExpr("x", add_x_2);
    auto *call_fun_expr_x_add_x_2_on_2 = new CallExpr(fun_expr_x_add_x_2, new NumExpr(2));

    // (_fun (y) (_fun (x) y + x))
    auto add_y_x = new Add(new Var("y"), new Var("x"));
    auto *fun_expr_x_add_y_x = new FunExpr("x", add_y_x);
    auto *fun_expr_y_fun_var_x_add_y_x = new FunExpr("y", fun_expr_x_add_y_x);
    auto *fun_var_y_fun_var_x_add_y_x = new FunVal("y", fun_expr_x_add_y_x);
    auto *call_fun_expr_y_fun_var_x_add_y_x_on_y_2 = new CallExpr(fun_expr_y_fun_var_x_add_y_x, new NumExpr(2));
    auto *call_call_fun_expr_y_fun_var_x_add_y_x_on_y_2_on_x_1 = new CallExpr(call_fun_expr_y_fun_var_x_add_y_x_on_y_2, new NumExpr(1));

    SECTION("equals") {
        REQUIRE(call_fun_expr_x_add_x_1_on_2->equals(call_fun_expr_x_add_x_1_on_2_copy));
        REQUIRE(call_fun_expr_x_add_x_1_on_2->equals(call_fun_expr_x_add_x_1_on_3) == false);
        REQUIRE(call_fun_expr_x_add_x_1_on_2->equals(add_x_1) == false);
        REQUIRE(call_fun_expr_x_add_x_1_on_2->equals(call_fun_expr_x_add_x_2_on_2) == false);
    }

    auto *call_fun_expr_x_add_x_1_on_2_subst_x_for_2 = call_fun_expr_x_add_x_1_on_2->subst("x", new NumExpr(2));
    auto *add_2_1 = new Add(new NumExpr(2), new NumExpr(1));
    auto *fun_expr_x_add_2_1 = new FunExpr("x", add_2_1);
    auto *call_fun_expr_x_add_2_1_on_2 = new CallExpr(fun_expr_x_add_2_1, new NumExpr(2));
    auto *call_fun_expr_x_add_x_1_on_2_subst_y_for_2 = call_fun_expr_x_add_x_1_on_2->subst("y", new NumExpr(2));

    auto *add_2_x = new Add(new NumExpr(2), new Var("x"));
    auto *fun_val_x_add_2_x = new FunVal("x", add_2_x);

    SECTION("interp") {
        REQUIRE(call_fun_expr_x_add_x_1_on_2->interp()->equals(new NumVal(3)));
        REQUIRE(call_fun_expr_y_fun_var_x_add_y_x_on_y_2->interp()->equals(fun_val_x_add_2_x));
        REQUIRE(call_call_fun_expr_y_fun_var_x_add_y_x_on_y_2_on_x_1->interp()->equals(new NumVal(3)));
    }

    SECTION("subst") {
        REQUIRE(call_fun_expr_x_add_x_1_on_2_subst_x_for_2->equals(call_fun_expr_x_add_x_1_on_2));
        REQUIRE(call_fun_expr_x_add_x_1_on_2_subst_y_for_2->equals(call_fun_expr_x_add_x_1_on_2));
    }

    SECTION("print") {
        REQUIRE(call_fun_expr_x_add_x_1_on_2->to_string() == "((_fun(x)(x+1))) (2)");
        REQUIRE(call_call_fun_expr_y_fun_var_x_add_y_x_on_y_2_on_x_1->to_string() == "(((_fun(y)(_fun(x)(y+x)))) (2)) (1)");
    }

    SECTION("pretty_print") {
        REQUIRE(call_fun_expr_x_add_x_1_on_2->to_pretty_string() == "(_fun (x)\n"
                                                                    "   x + 1)(2)");
        REQUIRE(call_call_fun_expr_y_fun_var_x_add_y_x_on_y_2_on_x_1->to_pretty_string() == "(_fun (y)\n"
                                                                                            "   _fun (x)\n"
                                                                                            "     y + x)(2)(1)");
    }

    SECTION("factorial function recusion test") {
        Expr *factrl_expr = new Let(
                "factrl",
                new FunExpr(
                        "factrl",
                        new FunExpr(
                                "x",
                                new IfExpr(
                                        new EqExpr(
                                                new Var("x"),
                                                new NumExpr(1)
                                        ),
                                        new NumExpr(1),
                                        new Mult(
                                                new Var("x"),
                                                new CallExpr(
                                                        new CallExpr(
                                                                new Var("factrl"),
                                                                new Var("factrl")
                                                        ),
                                                        new Add(
                                                                new Var("x"),
                                                                new NumExpr(-1)
                                                        )
                                                )
                                        )
                                )
                        )
                ),
                new CallExpr(
                        new CallExpr(
                                new Var("factrl"),
                                new Var("factrl")
                        ),
                        new NumExpr(10)
                )
        );

        REQUIRE(factrl_expr->to_pretty_string() == "_let factrl = _fun (factrl)\n"
                                                   "                _fun (x)\n"
                                                   "                  _if x == 1\n"
                                                   "                  _then 1\n"
                                                   "                  _else x * factrl(factrl)(x + -1)\n"
                                                   "_in  factrl(factrl)(10)");

        REQUIRE(factrl_expr->interp()->equals(new NumVal(3628800)));
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------


TEST_CASE("parse Fun Expr string") {
    auto *add_x_1 = new Add(new Var("x"), new NumExpr(1));
    auto *fun_expr_x_add_x_1 = new FunExpr("x", add_x_1);

    REQUIRE(parse_expression_str("_fun (x)\n"
                                 "  x + 1")->equals(fun_expr_x_add_x_1));


    std::string test_str = "_let factrl = _fun (factrl)\n"
                           "                _fun (x)\n"
                           "                  _if x == 1\n"
                           "                  _then 1\n"
                           "                  _else x * factrl(factrl)(x + -1)\n"
                           "_in  factrl(factrl)(10)";

    auto *parsed_expr = parse_expression_str(test_str);
    auto parsed_expr_pretty_str = parsed_expr->to_pretty_string();
    auto parsed_expr_str = parsed_expr->to_string();
    REQUIRE(parsed_expr_pretty_str == test_str);
    REQUIRE(parsed_expr->equals(parse_expression_str(parsed_expr_pretty_str)));
    REQUIRE(parsed_expr->equals(parse_expression_str(parsed_expr_str)));
    REQUIRE(parsed_expr->interp()->equals(new NumVal(3628800)));
}


TEST_CASE("parse Call Expr string") {
    auto *add_x_1 = new Add(new Var("x"), new NumExpr(1));
    auto *fun_expr_x_add_x_1 = new FunExpr("x", add_x_1);
    auto *call_2_on_fun_expr_x_add_x_1 = new CallExpr(fun_expr_x_add_x_1, new NumExpr(2));
    REQUIRE(parse_expression_str("(_fun (x) x + 1)(2)")->equals(call_2_on_fun_expr_x_add_x_1));
    REQUIRE(parse_expression_str("(_fun (x)\n"
                                 "   x + 1)(2)")->equals(call_2_on_fun_expr_x_add_x_1));

    REQUIRE(parse_expression_str("_let f = _fun (x)\n"
                                 "\t       _fun (y) \n"
                                 "\t         x*x + y*y \n"
                                 "_in f(2)(3)")->interp()->equals(new NumVal(13)));
}

TEST_CASE("quiz function vals") {
    // 1
    REQUIRE(parse_expression_str("_let f = _fun (x) x+ 1 \n"
                                 "_in f(5) ")->interp()->equals(new NumVal(6)));

    // 2
    REQUIRE(parse_expression_str("_let f = _fun (x)\n"
                                 "           7\n"
                                 "_in f(5)")->interp()->equals(new NumVal(7)));

    // 3
    REQUIRE(parse_expression_str("_let f = _fun (x)\n"
                                 "           _true\n"
                                 "_in f(5) ")->interp()->equals(new BoolVal(true)));

    // 4
    REQUIRE_THROWS_WITH(parse_expression_str("_let f = _fun (x)\n"
                                             "           x + _true\n"
                                             "_in f(5) ")->interp(), "add of non-number");

    // 5
    REQUIRE(parse_expression_str("_let f = _fun (x)\n"
                                 "           x + _true\n"
                                 "_in 5 + 1 ")->interp()->equals(new NumVal(6)));

    // 6
    REQUIRE_THROWS_WITH(parse_expression_str("_let f = _fun (x)\n"
                                             "           7\n"
                                             "_in  f(5 + _true)")->interp(), "add of non-number");

    // 7
    REQUIRE_THROWS_WITH(parse_expression_str("_let f = _fun (x) x+ 1\n"
                                             "_in f + 5")->interp(), "Addition to a FunVal is not possible");

    // 8
    REQUIRE(parse_expression_str("_let f = _fun (x) x+ 1 \n"
                                 "_in _if _false\n"
                                 "    _then f(5)\n"
                                 "    _else f(6)")->interp()->equals(new NumVal(7)));

    // 9
    REQUIRE(parse_expression_str("_let f = _fun (x) x+ 1 \n"
                                 "_in _let g = _fun (y) y+ 2 \n"
                                 "_in _if _true\n"
                                 "    _then f(5)\n"
                                 "    _else g(5)")->interp()->equals(new NumVal(6)));

    // 10
    REQUIRE(parse_expression_str("_let f = _fun (x) x+ 1 \n"
                                 "_in _let g = _fun (y) y+ 2 \n"
                                 "_in f(g(5)) ")->interp()->equals(new NumVal(8)));

    // 11
    REQUIRE(parse_expression_str("_let f = _fun (x) x+ 1 \n"
                                 "_in _let g = _fun (y)\n"
                                 "              f(y + 2)\n"
                                 "_in g(5) ")->interp()->equals(new NumVal(8)));

    // 12
    REQUIRE(parse_expression_str("_let f = _fun (x) x+ 1 \n"
                                 "_in _let g = _fun (x)\n"
                                 "              f(2) + x\n"
                                 "_in g(5) ")->interp()->equals(new NumVal(8)));

    // 13
    REQUIRE_THROWS_WITH(parse_expression_str("_let f = _fun (x) x+ 1 \n"
                                             "_in f 5 ")->interp(), "invalid input");

    // 14
    REQUIRE(parse_expression_str("_let f = _fun (x) x+ 1 \n"
                                 "_in (f)(5) ")->interp()->equals(new NumVal(6)));



    // 15
    auto *add_x_1 = new Add(new Var("x"), new NumExpr(1));
    auto *fun_val_x_add_x_1 = new FunVal("x", add_x_1);
    REQUIRE(parse_expression_str("_fun (x) x+ 1 ")->interp()->equals(fun_val_x_add_x_1));

    //16
    REQUIRE(parse_expression_str("_let f = _fun (x) x+ 1 \n"
                                 "_in f ")->interp()->equals(fun_val_x_add_x_1));

    // 17
    REQUIRE(parse_expression_str("(_fun (x)\n"
                                 "   x + 1)(5)")->interp()->equals(new NumVal(6)));

    // 18
    REQUIRE(parse_expression_str("_let f = _if _false\n"
                                 "            _then _fun (x)  \n"
                                 "                        x+ 1 \n"
                                 "           _else _fun (x)\n"
                                 "                       x+ 2\n"
                                 "_in f(5)")->interp()->equals(new NumVal(7)));

    // 19
    REQUIRE(parse_expression_str("(_if _false \n"
                                 "  _then _fun (x)\n"
                                 "            x+ 1\n"
                                 "   _else _fun (x)\n"
                                 "                x + 2)(5)")->interp()->equals(new NumVal(7)));


    // 20
    REQUIRE(parse_expression_str("_let f = _fun (g)\n"
                                 "           g(5)\n"
                                 "_in _let g = _fun (y)  \n"
                                 "             y + 2\n"
                                 "_in f(g) ")->interp()->equals(new NumVal(7)));

    // 21
    REQUIRE(parse_expression_str("_let f = _fun (g)\n"
                                 "           g(5)\n"
                                 "_in f(_fun (y)\n"
                                 "        y + 2)")->interp()->equals(new NumVal(7)));

    // 22
    REQUIRE(parse_expression_str("_let f = _fun (x)\n"
                                 "           _fun (y)\n"
                                 "x+ y _in (f(5))(1) ")->interp()->equals(new NumVal(6)));

    // 23
    REQUIRE(parse_expression_str("_let f = _fun (x)\n"
                                 "           _fun (y)\n"
                                 "x+ y _in f(5)(1) ")->interp()->equals(new NumVal(6)));

    // 24
    REQUIRE(parse_expression_str("_let f = _fun (x)\n"
                                 "           _fun (g)\n"
                                 "             g(x + 1)\n"
                                 "_in _let g = _fun (y)\n"
                                 "              y+ 2 \n"
                                 "_in (f(5))(g) ")->interp()->equals(new NumVal(8)));

    // 25
    REQUIRE(parse_expression_str("_let f = _fun (x)\n"
                                 "           _fun (g)\n"
                                 "             g(x + 1)\n"
                                 "_in _let g = _fun (y)\n"
                                 "y+ 2 _in f(5)(g)")->interp()->equals(new NumVal(8)));

    // 26
    REQUIRE(parse_expression_str("_let f = _fun (f)\n"
                                 "           _fun (x)\n"
                                 "             _if x == 0\n"
                                 "             _then 0\n"
                                 "             _else x + f(f)(x + -1)\n"
                                 "_in f(f)(3)")->interp()->equals(new NumVal(6)));




    // Q 26 example
    Var *x = new Var("x");
    Var *f = new Var("f");
    FunExpr *q26_expr_rhs = new FunExpr("f", new FunExpr("x", new IfExpr(new EqExpr(x, new NumExpr(0)),
                                                                         new NumExpr(0),
                                                                         new Add(x, new CallExpr(new CallExpr(f, f),
                                                                                                  new Add(x,
                                                                                                              new NumExpr(
                                                                                                                      -1))))
                                                      )
    ));
    Let *q26_expr = new Let("f", q26_expr_rhs, new CallExpr(new CallExpr(f, f), new NumExpr(3)));

    REQUIRE(q26_expr->to_pretty_string() == "_let f = _fun (f)\n"
                                            "           _fun (x)\n"
                                            "             _if x == 0\n"
                                            "             _then 0\n"
                                            "             _else x + f(f)(x + -1)\n"
                                            "_in  f(f)(3)");

    REQUIRE(q26_expr->interp()->equals(new NumVal(6)));
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("Fun Val") {
    auto *add_x_1 = new Add(new Var("x"), new NumExpr(1));
    auto *fun_var_x_add_x_1 = new FunVal("x", add_x_1);
    auto *fun_var_x_add_x_1_copy = new FunVal("x", add_x_1);

    auto *num_val_2 = new NumVal(2);

    // (_fun (y) (_fun (x) y + x))
    auto add_y_x = new Add(new Var("y"), new Var("x"));
    auto *fun_expr_x_add_y_x = new FunExpr("x", add_y_x);
    auto *fun_expr_y_fun_var_x_add_y_x = new FunExpr("y", fun_expr_x_add_y_x);
    auto *fun_var_y_fun_var_x_add_y_x = new FunVal("y", fun_expr_x_add_y_x);

    SECTION("add_to") {
        REQUIRE_THROWS_WITH(fun_var_x_add_x_1->add_to(num_val_2), "Addition to a FunVal is not possible");
    }

    SECTION("mult_with") {
        REQUIRE_THROWS_WITH(fun_var_x_add_x_1->mult_with(num_val_2), "Multiplication with a FunVal is not possible");
    }

    SECTION("equals") {
        REQUIRE(fun_var_x_add_x_1->equals(fun_var_x_add_x_1_copy));
        REQUIRE(fun_var_x_add_x_1->equals(num_val_2) == false);
    }

    SECTION("to_string") {
        REQUIRE(fun_var_x_add_x_1->to_string() == "(_fun(x)(x+1))");
        REQUIRE(fun_var_y_fun_var_x_add_y_x->to_string() == "(_fun(y)(_fun(x)(y+x)))");
    }

    auto *fun_expr_x_add_x_1 = new FunExpr("x", add_x_1);

    SECTION("to_expr") {
        REQUIRE(fun_var_x_add_x_1->to_expr()->equals(fun_expr_x_add_x_1));
        REQUIRE(fun_var_y_fun_var_x_add_y_x->to_expr()->equals(fun_expr_y_fun_var_x_add_y_x));
    }

    SECTION("is_true") {
        REQUIRE_THROWS_WITH(fun_var_x_add_x_1->is_true(), "a fun val cannot be interpreted as a bool val");
        REQUIRE_THROWS_WITH(fun_var_y_fun_var_x_add_y_x->is_true(), "a fun val cannot be interpreted as a bool val");
    }

    auto *add_2_x = new Add(new NumExpr(2), new Var("x"));
    auto *fun_val_x_add_2_x = new FunVal("x", add_2_x);
    auto *fun_var_y_fun_var_x_add_y_x_call_2 = fun_var_y_fun_var_x_add_y_x->call(new NumVal(2));

    SECTION("call") {
        REQUIRE(fun_var_x_add_x_1->call(new NumVal(3))->equals(new NumVal(4)));
        REQUIRE_THROWS_WITH(fun_var_x_add_x_1->call(new BoolVal(3)), "Addition to a boolean is not possible");
        REQUIRE_THROWS_WITH(fun_var_x_add_x_1->call(fun_var_x_add_x_1), "Addition to a FunVal is not possible");
        REQUIRE(fun_var_y_fun_var_x_add_y_x_call_2->equals(fun_val_x_add_2_x));
        REQUIRE(fun_var_y_fun_var_x_add_y_x_call_2->call(new NumVal(1))->equals(new NumVal(3)));
    }
}
