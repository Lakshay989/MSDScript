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
    CHECK_THROWS_WITH( (new Var("x"))->interp(), "interp does not work with variable expressions !!" );
    CHECK_THROWS_WITH( (new Var("x+y"))->interp(), "interp does not work with variable expressions !!" );
    CHECK_THROWS_WITH( (new Var("1"))->interp(), "interp does not work with variable expressions !!" );

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

// ------------------------------------------------------------------------------------------------------------

    
TEST_CASE("to_pretty_string") {
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
    
    SECTION("Universal Examples")
    {
        CHECK((new Mult( new Mult(new Num(10), new Mult(new Mult(new Num(10), new Num(10)), new Num(10))), new Mult(new Num(10), new Num(10))))
              ->to_pretty_string()  == "(10 * (10 * 10) * 10) * 10 * 10");
        }
    
}


// ------------------------------------------------------------------------------------------------------------------------------------------------------------


TEST_CASE(" Tests for Let class written by me")
{
    SECTION("General Examples for Let to_pretty_string")
    {
        
        CHECK((new Let("x", new Num(5), new Add(new Let("y", new Num(3), new Add(new Var("y"), new Num(2))), new Var("x"))))
              ->to_pretty_string() == "_let x = 5\n"
                                      "_in  (_let y = 3\n"
                                      "      _in  y + 2) + x") ;


        CHECK((new Mult(new Mult(new Num (2), new Let("x", new Num(5), new Add(new Var("x") , new Num(1)))), new Num(3)))
              ->to_pretty_string() == "(2 * _let x = 5\n"
                                      "     _in  x + 1) * 3") ;


        Let *expression1 = new Let("x", new Num(5), new Add(new Let("y", new Num(3), new Add(new Var("y"), new Num(2))), new Var("x"))) ;

        Let *expression2 = new Let("z", new Num(9), new Add(expression1, new Var("z"))) ;

        CHECK(expression2->to_pretty_string() == "_let z = 9\n"
                                                 "_in  (_let x = 5\n"
                                                 "      _in  (_let y = 3\n"
                                                 "            _in  y + 2) + x) + z") ;


        Add *expression3 = new Add(new Let("x", new Num(5), new Var("x")), new Num(1));
        REQUIRE(expression3->to_pretty_string() == "(_let x = 5\n"
                                                      " _in  x) + 1");

        Let *expression4 = new Let("x", new Num(5), new Add(new Var("x"), new Num(1)));
        Mult *expression5 = new Mult(new Num(5), expression4);
        REQUIRE(expression5->to_pretty_string() == "5 * _let x = 5\n"
                                                   "    _in  x + 1");

        Add *expression6 = new Add(new Num(5), expression4);
        REQUIRE(expression6->to_pretty_string() == "5 + _let x = 5\n"
                                                   "    _in  x + 1");

        Add *expression7 = new Add(expression5, new Num(1));
        REQUIRE(expression7->to_pretty_string() == "5 * (_let x = 5\n"
                                                   "     _in  x + 1) + 1");


        Let *expression8 = new Let("x", new Let("y", new Num(6), new Mult(new Var("y"), new Num(2))), new Add(new Var("x"), new Num(1)));
        REQUIRE(expression8->to_pretty_string() == "_let x = _let y = 6\n"
                                                   "         _in  y * 2\n"
                                                   "_in  x + 1");
        }
    
    
    SECTION("Test LetBinding equals")
    {
        Let *expression1 = new Let("x", new Num(2), new Add(new Var("x"), new Num(1)));

        REQUIRE(expression1->equals(new Num(2)) == false);
        REQUIRE(expression1->equals(new Mult(new Var("x"), new Num(1))) == false);

        Add *expression2 = new Add(new Var("x"), new Num(3));
        Let *expression3 = new Let("x", new Num(2), expression2) ;
        
        REQUIRE(expression2->equals(new Add(new Var("x"), new Num(3))) == true);
        REQUIRE(expression3->equals(new Mult(new Var("x"), new Num(1))) == false);
        
    }
    
    SECTION("Test LetBinding interp") {
            /*
             * 5 * (_let x = 5
                _in  x) + 1
             */
            Let *expression1 = new Let("x", new Num(5), new Var("x"));
        
            Add *expression2 = new Add(new Mult(new Num(5), expression1), new Num(1));

            REQUIRE(expression1->interp() == 5);
            REQUIRE(expression2->interp() == 26);

        
            /*
             5 * _let x = 5
            _in  x + 1
             */

            Let *expression3 = new Let("x", new Add(new Num(5), new Num(2)), new Add(new Var("x"), new Num(1)));
            Mult *expression4 = new Mult(new Num(5), expression3);
            REQUIRE(expression4->interp() == 40);


            // subst interp
            Add *add = new Add(new Var("x"), new Var("y"));
            Let *expression5 = new Let("x", new Num(3), add);
            REQUIRE(expression5->subst("y", new Num(4))->interp() == 7);

        }

    SECTION("Test LetBinding has_variable") {
        std::string lhs = "x";

        Num *num = new Num(2);
        Add *exp = new Add(new Num(1), new Var("y"));
        Var *variable = new Var("z");

        Num *num_body = new Num(2);
        Add *exp_body = new Add(new Num(1), new Var("x"));
        Var *variable_body = new Var("x");

        // rhs: number
        REQUIRE((new Let(lhs, num, num_body))->has_variable() == false);
        REQUIRE((new Let(lhs, num, exp_body))->has_variable());
        REQUIRE((new Let(lhs, num, variable_body))->has_variable());

        // rhs: expression has variable
        REQUIRE((new Let(lhs, exp, num_body))->has_variable());
        REQUIRE((new Let(lhs, exp, exp_body))->has_variable());
        REQUIRE((new Let(lhs, exp, variable_body))->has_variable());

        // rhs: rhs is a variable
        REQUIRE((new Let(lhs, variable, num_body))->has_variable());
        REQUIRE((new Let(lhs, variable, exp_body))->has_variable());
        REQUIRE((new Let(lhs, variable, variable_body))->has_variable());
    }

    SECTION("Test LetBinding subst") {

        /*   y = 4
          _let x = 3
         _in x + y
         */
        
        Let *expression1 = new Let("x", new Num(3), new Add(new Var("x"), new Var("y")));
        Let *expression2 = new Let("x", new Num(3), new Add(new Var("x"), new Num(4)));

        CHECK(expression1->subst("y", new Num(4))->equals(expression2) == true);
        
        CHECK(expression1->subst("y", new Num(4))->to_pretty_string() == expression2->to_pretty_string()) ;

        /* subst x with 5
         _let x = 6
        _in x + 1)
        */
        Let *expression3 = new Let("x", new Num(6),new Add(new Var("x"), new Num(1)));
        
        REQUIRE(expression3->subst("x", new Num(5))->equals(expression3));
    }

    SECTION("Test LetBinding to_string")
    {
        
        Let *expression1 = new Let("y", new Num(3), new Add(new Var("y"), new Num(2)));
        
        Let *expression2 = new Let("x", new Num(5),new Add(expression1, new Var("x")));
        
        
        REQUIRE(expression1->to_string() == "(_let y=3 _in (y+2))");
        REQUIRE(expression2->to_string() == "(_let x=5 _in ((_let y=3 _in (y+2))+x))");
    }

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------


TEST_CASE("Pretty Print examples_Kevin") //Created from assignment examples

{
    std::stringstream out("");
    (new Let("x", new Num(5), new Add(new Var("x"), new Num(1))))->pretty_print(out);
    CHECK(out.str() == "_let x = 5\n"
                       "_in  x + 1");
    out.str(std::string());
    (new Add(new Let("x", new Num(5), new Var("x")), new Num(1)))->pretty_print(out);
    CHECK(out.str() == "(_let x = 5\n"
                       " _in  x) + 1");
    out.str(std::string());
    (new Mult(new Num(5), new Let("x", new Num(5), new Add(new Var("x"), new Num(1)))))->pretty_print(out);
    CHECK(out.str() == "5 * _let x = 5\n"
                       "    _in  x + 1");
    out.str(std::string());
    (new Add(new Mult(new Num(5), new Let("x", new Num(5), new Var("x"))), new Num(1)))->pretty_print(out);
    CHECK(out.str() == "5 * (_let x = 5\n"
                       "     _in  x) + 1");
    out.str(std::string());
    (new Let("x", new Num(5), new Add(new Let("y", new Num(3), new Add(new Var("y"), new Num(2))), new Var("x"))))
            ->pretty_print(out);

    CHECK(out.str() == "_let x = 5\n"
                       "_in  (_let y = 3\n"
                       "      _in  y + 2) + x");
    out.str(std::string());
    (new Let("x", new Let("y", new Num(6), new Mult(new Var("y"), new Num(2))), new Add(new Var("x"), new Num(1))))->pretty_print(out);
    CHECK(out.str() == "_let x = _let y = 6\n"
                       "         _in  y * 2\n"
                       "_in  x + 1");

}

TEST_CASE("pretty_print_let_mine_some_reuse_of_kevin_triple_nested_let") {
    Let* tripleNestedLet = new Let("x", new Num(1),
                                   new Let("y", new Num(1),
                                           new Mult(new Add(new Var("x"), new Var("y")), new Var("z"))));
    Let* tripleNestedLet2 = new Let("x", new Num(1),
                                    new Let("y", new Num(1),
                                            new Let("z", new Add(new Var("x"), new Num(1)),
                                                    new Add(new Add(new Var("x"), new Var("y")), new Var("z")))));
    Let* tripleNestedLet3 = new Let("x", new Num(1),
                                    new Let("y", new Num(1),
                                            new Let("z", new Add(new Var("x"), new Num(1)),
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
    Let* tripleNestedLet4 =new Let("x", new Num(5),
                                   new Let("y", new Num(3),
                                           new Add(new Var("y"), new Num(2))));
    Let* tripleNestedLet5 =new Let("x", new Num(5),
                                   new Add(new Let("y", new Num(3),
                                                   new Add(new Var("y"), new Num(2))), new Var("x")));
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
        CHECK( (new Add(new Mult(new Num(5), new Let("x", new Num(5), new Var("x"))), new Num(1)))-> to_pretty_string()
               == "5 * (_let x = 5\n"
                  "     _in  x) + 1");
        CHECK( (new Mult(new Mult(new Num (2), new Let("x", new Num(5), new Add(new Var("x") ,new  Num(1)) )), new Num(3) )) -> to_pretty_string()
               == "(2 * _let x = 5\n"
                  "     _in  x + 1) * 3");
    }
    // A _let needs parentheses when it is nested immediately as the right argument of an unparenthesized *
    // where _let would have needed parentheses in the surrounding context
    // (that is, if the _let used in place of the whole * would need parentheses,
    // then it still needs parentheses within the right-hand size of *).
    SECTION("new_edge") {
        CHECK( (new Mult(new Num (2), new Let("x", new Num(5), new Add(new Var("x") ,new  Num(1)) ) )) -> to_pretty_string()
               == "2 * _let x = 5\n"
                  "    _in  x + 1");
        CHECK( (new Add(new Mult(new Num(5), new Let("x", new Num(5), new Mult(new Var("x"), new Num(2)))), new Num(1)))-> to_pretty_string()
               == "5 * (_let x = 5\n"
                  "     _in  x * 2) + 1");
        CHECK( (new Mult((new Add(new Mult(new Num(5), new Let("x", new Num(5), new Mult(new Var("x"), new Num(2)))), new Num(1))), new Num(7)))-> to_pretty_string()
               == "(5 * (_let x = 5\n"
                  "      _in  x * 2) + 1) * 7");
        CHECK( (new Let("x", new Num(10), new Mult( new Mult(new Var("x"), new Mult(new Mult(new Num(10), new Num(10)), new Num(10))), new Mult(new Num(10), new Num(10)))))
                       ->to_pretty_string()  == "_let x = 10\n"
                                                      "_in  (x * (10 * 10) * 10) * 10 * 10");
        CHECK( (new Let("x", new Num(1), new Mult( new Mult(new Var("x"), new Mult(new Mult(new Num(10), new Num(10)), new Var("x"))), new Mult(new Num(10), new Num(10)))))
                        -> to_pretty_string() == "_let x = 1\n"
                                                       "_in  (x * (10 * 10) * x) * 10 * 10" );
        CHECK( (new Let("x", new Num(1), new Mult( new Mult(new Var("x"), new Mult(new Mult(new Num(10), new Num(10)), new Var("x"))), new Mult(new Var("y"), new Num(10)))))
                       -> to_pretty_string() == "_let x = 1\n"
                                                      "_in  (x * (10 * 10) * x) * y * 10" );
    }
}

TEST_CASE("Let_equals_mine") {
        SECTION("Values_same") {
            REQUIRE( (new Let("x", new Num(4), new Add(new Num(2), new Var("x")) ))->equals(new Let("x", new Num(4), new Add(new Num(2), new Var("x")) )));
        }
        SECTION("Values_same_different_rhs") {
            REQUIRE( !(new Let("x", new Num(4), new Add(new Num(2), new Var("x")) ))->equals(new Let("x", new Num(5), new Add(new Num(2), new Var("x")) )));
        }
        SECTION("Values_same_different_lhs") {
            REQUIRE( !(new Let("x", new Num(4), new Add(new Num(2), new Var("x")) ))->equals(new Let("y", new Num(4), new Add(new Num(2), new Var("x")) )));
        }
        SECTION("Values_same_different_body") {
            REQUIRE( !(new Let("x", new Num(4), new Add(new Num(2), new Var("x")) ))->equals(new Let("x", new Num(4), new Mult(new Num(3), new Var("y")) )));
        }
        SECTION("different_types") {
            REQUIRE( !(new Let("x", new Num(4), new Add(new Num(2), new Var("x")) ))->equals( new Mult(new Num(3), new Var("y")) ));
        }
}
TEST_CASE("Let_has_variable_mine") {
    SECTION("has") {
        REQUIRE( (new Let("x", new Num(4), new Add(new Num(2), new Var("x")) ))->has_variable());
    }
    SECTION("does_not_has") {
        REQUIRE( !(new Let("x", new Num(4), new Add(new Num(2), new Num(4)) ))->has_variable());
    }
}
TEST_CASE("Let_print_mine") {
    CHECK( (new Let("x", new Num(5), new Add(new Let("y", new Num(3), new Add(new Var("y"), new Num(2))), new Var("x")))) -> to_string()
                                                                                                        == "(_let x=5 _in ((_let y=3 _in (y+2))+x))");
    CHECK( (new Let("x", new Num(1),
                   new Let("y", new Num(1),
                           new Let("z", new Add(new Var("x"), new Num(1)),
                                   new Mult(new Add(new Var("x"), new Var("y")), new Var("z")))))) -> to_string()
                                                                                                        == "(_let x=1 _in (_let y=1 _in (_let z=(x+1) _in ((x+y)*z))))");
}
TEST_CASE ("Let_interp_mine") {
    SECTION("hw_examples") {
        CHECK((new Add(new Mult(new Num(5), new Let("x", new Num(5), new Var("x"))), new Num(1))) -> interp() == 26);
        CHECK((new Mult(new Num(5), new Let("x", new Num(5), new Add(new Var("x"), new Num(1))))) -> interp() == 30);
    }
    SECTION("from_pretty_print_edge") {
        CHECK( (new Let("x", new Num(1),
                       new Let("y", new Num(1),
                               new Let("z", new Add(new Var("x"), new Num(1)),
                                       new Mult(new Add(new Var("x"), new Var("y")), new Var("z")))))) -> interp() == 4);
        CHECK( (new Mult((new Add(new Mult(new Num(5), new Let("x", new Num(5), new Mult(new Var("x"), new Num(2)))), new Num(1))), new Num(7))) -> interp() == 357); // 51 * 7
        CHECK( (new Let("x", new Num(10), new Mult( new Mult(new Var("x"), new Mult(new Mult(new Num(10), new Num(10)), new Num(10))), new Mult(new Num(10), new Num(10)))))
                      ->interp()  == 1000000);
        CHECK( (new Let("x", new Num(1), new Mult( new Mult(new Var("x"), new Mult(new Mult(new Num(10), new Num(10)), new Var("x"))), new Mult(new Num(10), new Num(10)))))
                       ->interp()  == 10000);
        CHECK_THROWS_WITH( ((new Let("x", new Num(1), new Mult( new Mult(new Var("x"), new Mult(new Mult(new Num(10), new Num(10)), new Var("x"))), new Mult(new Var("y"), new Num(10)))))
                       -> interp() == 10000), "interp does not work with variable expressions !!");
    }
    SECTION("bypass_middle_let") {
        CHECK ((new Let("x", new Num(2), new Let("z", new Num(4), new Add(new Var("x"), new Num(10)))))
                -> interp() == 12);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------


TEST_CASE("Provided tests for parse ") {
  CHECK_THROWS_WITH( parse_expression_str("()"), "invalid input" );
  
  CHECK( parse_expression_str("(1)")->equals(new Num(1)) );
  CHECK( parse_expression_str("(((1)))")->equals(new Num(1)) );
  
  CHECK_THROWS_WITH( parse_expression_str("(1"), "missing close parenthesis" );
  
  CHECK( parse_expression_str("1")->equals(new Num(1)) );
  CHECK( parse_expression_str("10")->equals(new Num(10)) );
  CHECK( parse_expression_str("-3")->equals(new Num(-3)) );
  CHECK( parse_expression_str("  \n 5  ")->equals(new Num(5)) );
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
CHECK((new Num(1))->equals(new Num(1)));
CHECK(!(new Num(1))->equals(new Num(2)));
CHECK(!(new Num(1))->equals(new Mult (new Num(2), new Num(4))));
CHECK((new Var("hello"))->equals(new Var("hello")));
CHECK(!(new Var("hello"))->equals(new Var("ello")));
CHECK(parse_expression_str("1")->equals(new Num(1)));
CHECK(parse_expression_str("  1")->equals(new Num(1)));
CHECK(parse_expression_str("4+2")->equals(new Add(new Num(4), new Num(2))));
CHECK(parse_expression_str("   45 +    23")->equals(new Add(new Num(45), new Num(23))));
CHECK(parse_expression_str("   45   *  23   ")->equals(new Mult(new Num(45), new Num(23))));
CHECK(parse_expression_str("(90)")->equals(new Num(90)));
CHECK(parse_expression_str("3*(2+43)")->equals(new Mult(new Num(3),new Add(new Num(2), new Num(43)))));
CHECK(parse_expression_str("Hello")->equals(new Var("Hello")));
CHECK(parse_expression_str("3*(2+width)")->equals(new Mult(new Num(3),new Add(new Num(2), new Var("width")))));
}

TEST_CASE("Test cases for parse") {
    REQUIRE_THROWS_WITH(parse_expression_str("()"), "invalid input");

    REQUIRE(parse_expression_str("    1     ")->equals(new Num(1)));
    REQUIRE(parse_expression_str("(   1   )")->equals(new Num(1)));
    REQUIRE(parse_expression_str("(1)")->equals(new Num(1)));
    REQUIRE(parse_expression_str("(((1)))")->equals(new Num(1)));


    REQUIRE_THROWS_WITH(parse_expression_str("(1"), "missing close parenthesis");
    REQUIRE_THROWS_WITH(parse_expression_str("( 1    "), "missing close parenthesis");

    REQUIRE(parse_expression_str("1")->equals(new Num(1)));
    REQUIRE(parse_expression_str("10")->equals(new Num(10)));
    REQUIRE(parse_expression_str("-3")->equals(new Num(-3)));

    REQUIRE(parse_expression_str("  \n 5  ")->equals(new Num(5)));

    REQUIRE_THROWS_WITH(parse_expression_str("-"), "number should come right after -");
    REQUIRE_THROWS_WITH(parse_expression_str(" -   5  "), "number should come right after -");

    REQUIRE(parse_expression_str(" ( 2 + 3) * 5 + 1 ")->interp() == 26);
    REQUIRE(parse_expression_str(" 2 + 3  * 5 + 1 ")->interp() == 18);
    REQUIRE(parse_expression_str(" 2 + 3  * (5 + 1 )")->interp() == 20);

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
        Add *expression1 = new Add(new Var("x"), new Num(1));
        Let *expression2 = new Let("x", new Num(5), expression1);
        REQUIRE(parse_expression_str("_let x = 5 _in x + 1")->equals(expression2));

        Let *expression3 = new Let("x", new Add(new Var("x"), new Num(1)), new Add(new Var("x"), new Num(7)));
        Let *expression4 = new Let("x", new Num(5), expression3);
        REQUIRE(parse_expression_str("_let x = 5 _in _let x = x +1    _in  x + 7")->equals(expression4));

        REQUIRE_THROWS_WITH(parse_expression_str("_lt x = 5 _in x + 1"), "invalid input");
        REQUIRE_THROWS_WITH(parse_expression_str("_let x  5 _in x + 1"), "invalid input");
        REQUIRE_THROWS_WITH(parse_expression_str("_let x = 5 _i x + 1"), "invalid input");
        REQUIRE_THROWS_WITH(parse_expression_str("_let x = 5 x + 1"), "invalid input");
    }
}
