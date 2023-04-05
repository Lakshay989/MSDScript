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
    REQUIRE((NEW(NumExpr)(6))->equals(NEW(NumExpr)(6)));
    CHECK( (NEW (NumExpr)(1))->equals(NEW (NumExpr)(1)));
    CHECK( (NEW (NumExpr)(0))->equals(NEW (NumExpr)(-0)));
    CHECK( (NEW (NumExpr)(5))->equals(NEW (NumExpr)(-5)) == false);
    CHECK( ! (NEW (NumExpr) (1))->equals(NEW (NumExpr)(2)) );

    CHECK( ! (NEW (NumExpr) (1))->equals(NEW (Mult)(NEW (NumExpr)(2), NEW (NumExpr)(4))) );
    CHECK(!(NEW (NumExpr) (8))->equals(NEW (Mult)(NEW (NumExpr)(2), NEW (NumExpr)(4))) );
    CHECK(!(NEW (NumExpr) (1))->equals(NEW (Mult)(NEW (NumExpr)(1), NEW (NumExpr)(1))) );

    CHECK( (NEW (Var)("x"))->equals(NEW (Var)("x")) );
    CHECK( ! (NEW (Var)("x"))->equals(NEW (NumExpr)(5)) );
    CHECK( !(NEW (Var)("x"))->equals(NEW (Var)("y")) );

    CHECK( (NEW (Add)(NEW (NumExpr)(8), NEW (NumExpr)(9)))
          ->equals(NEW (Add)(NEW (NumExpr)(8), NEW (NumExpr)(9))) );
    CHECK( ! (NEW (Add) (NEW (NumExpr)(8), NEW (NumExpr)(9)))
          ->equals(NEW (Add)(NEW (NumExpr)(8), NEW (NumExpr)(10))) );
    CHECK( ! (NEW (Add) (NEW (NumExpr)(8), NEW (NumExpr)(9)))
          ->equals(NEW (Add)(NEW (NumExpr)(10), NEW (NumExpr)(9))) );
    CHECK( ! (NEW (Add)(NEW (NumExpr)(8), NEW (NumExpr)(9)))
          ->equals(NEW (NumExpr)(8)) );

    CHECK( (NEW (Mult)(NEW (NumExpr)(1),NEW (NumExpr)(2)))->equals(NEW (Add)(NEW (NumExpr)(1),NEW (NumExpr)(2)))==false );

    CHECK( (NEW (NumExpr)(1))->equals(NEW (NumExpr)(1)) == true );
    CHECK( (NEW (Var)("x"))->equals(NEW (Var)("y")) == false );
    CHECK( (NEW (Add)(NEW (NumExpr)(2),NEW (NumExpr)(3)))->equals(NEW (Add)(NEW (NumExpr)(2),NEW (NumExpr)(3)))==true );
    CHECK( (NEW (Add)(NEW (NumExpr)(2),NEW (NumExpr)(3)))->equals(NEW (Add)(NEW (NumExpr)(3),NEW (NumExpr)(2)))==false );
    CHECK( (NEW (Mult)(NEW (NumExpr)(2),NEW (NumExpr)(2)))->equals(NEW (Add)(NEW (NumExpr)(1),NEW (NumExpr)(2)))==false );

}

// ------------------------------------------------------------------------------------------------------------

TEST_CASE("Tests for checking interp")
{
    CHECK_THROWS_WITH( (NEW (Var)("x"))->interp(), "free variable: x" );

    CHECK( (NEW (Add)(NEW (Add)(NEW (NumExpr)(10), NEW (NumExpr)(15)),NEW (Add)(NEW (NumExpr)(20),NEW (NumExpr)(20))))->interp()->equals(NEW (NumVal)(65))) ;
    CHECK( (NEW (Add)(NEW (Add)(NEW (NumExpr)(5), NEW (NumExpr)(5)),NEW (Add)(NEW (NumExpr)(20),NEW (NumExpr)(20))))->interp()->equals(NEW (NumVal)(50)));
    CHECK( (NEW (Add)(NEW (Add)(NEW (NumExpr)(-50), NEW (NumExpr)(15)),NEW (Add)(NEW (NumExpr)(-15),NEW (NumExpr)(50))))->interp()->equals(NEW (NumVal)(0)));
    CHECK( (NEW (Add)(NEW (Add)(NEW (NumExpr)(1000), NEW (NumExpr)(500)),NEW (Add)(NEW (NumExpr)(500),NEW (NumExpr)(-1000))))->interp()->equals(NEW (NumVal)(1000)));
    CHECK( (NEW (Add)(NEW (Mult)(NEW (NumExpr)(5), NEW (NumExpr)(10)),NEW (Mult)(NEW (NumExpr)(-5),NEW (NumExpr)(-10))))->interp()->equals(NEW (NumVal)(100)));
    CHECK( (NEW (Add)(NEW (Mult)(NEW (NumExpr)(50), NEW (NumExpr)(1)),NEW (Add)(NEW (NumExpr)(30),NEW (NumExpr)(20))))->interp()->equals(NEW (NumVal)(100)));

    CHECK( (NEW (Mult)(NEW (NumExpr)(3), NEW (NumExpr)(2)))->interp()->equals(NEW (NumVal)(6)));
    CHECK( (NEW (Mult)(NEW (NumExpr)(3), NEW (NumExpr)(0)))->interp()->equals(NEW (NumVal)(0)));
    CHECK( (NEW (Mult)(NEW (NumExpr)(6), NEW (NumExpr)(9)))->interp()->equals(NEW (NumVal)(54)));
    CHECK( (NEW (Mult)(NEW (NumExpr)(3), NEW (NumExpr)(-30)))->interp()->equals(NEW (NumVal)(-90)));
    CHECK( (NEW (Mult)(NEW (Add)(NEW (NumExpr)(5), NEW (NumExpr)(5)),NEW (Add)(NEW (NumExpr)(27),NEW (NumExpr)(23))))->interp()->equals(NEW (NumVal)(500)));
    CHECK( (NEW (Mult)(NEW (Add)(NEW (NumExpr)(-50), NEW (NumExpr)(-50)),NEW (Mult)(NEW (NumExpr)(0),NEW (NumExpr)(50))))->interp()->equals(NEW (NumVal)(0)));
    CHECK( (NEW (Mult)(NEW (Mult)(NEW (NumExpr)(5), NEW (NumExpr)(5)),NEW (Mult)(NEW (NumExpr)(-5),NEW (NumExpr)(-5))))->interp()->equals(NEW (NumVal)(625)));
}

// ------------------------------------------------------------------------------------------------------------

TEST_CASE("to_string") {
    SECTION("Test Num to_string") {
        REQUIRE((NEW (NumExpr)(10))->to_string() == "10");
        REQUIRE((NEW (NumExpr)(-1))->to_string() == "-1");
    }
    
    SECTION("Test (Add) to_string") {
        auto add_test1 = NEW (Add)(NEW (NumExpr)(1), NEW (NumExpr)(-1)); // (Add)(Num, Num)
        auto add_test2 = NEW (Add)(add_test1, NEW (NumExpr)(3)); // (Add)((Add), Num)
        auto add_test3 = NEW (Add)(NEW (NumExpr)(1), NEW (Mult)(NEW (NumExpr)(1), NEW (NumExpr)(2))); // (Add)(Num , (Mult))
        auto add_test4 = NEW (Add)(NEW (NumExpr)(1), NEW (Var)("x")); //(Add) (Num, (Var))
        
        REQUIRE(add_test1->to_string() == "(1+-1)");
        REQUIRE(add_test2->to_string() == "((1+-1)+3)");
        REQUIRE(add_test3->to_string() == "(1+(1*2))");
        REQUIRE(add_test4->to_string() == "(1+x)");
    }
    
    SECTION("Test (Mult) to_string") {
         auto mult_test1 = NEW (Mult)(NEW (NumExpr)(1), NEW (NumExpr)(-2)); // (Mult) (Num , Num)
        auto mult_test2 = NEW (Mult)(mult_test1, NEW (NumExpr)(3)); // (Mult)((Mult) , Num)
        auto mult_test3 = NEW (Mult)(NEW (NumExpr)(1), NEW (Add)(NEW (NumExpr)(2), NEW (NumExpr)(3))); // (Mult)(Num , (Add))
        auto mult_test4 = NEW (Mult)(NEW (NumExpr)(1), NEW (Var)("x")); // (Mult)(Num , (Var))
        
        
        REQUIRE(mult_test1->to_string() == "(1*-2)");
        REQUIRE(mult_test2->to_string() == "((1*-2)*3)");
        REQUIRE(mult_test3->to_string() == "(1*(2+3))");
        REQUIRE(mult_test4->to_string() == "(1*x)");
        
    }
}

// ------------------------------------------------------------------------------------------------------------

    
TEST_CASE("to_pretty_string") {
    SECTION("Test Num to_pretty_string") {
        REQUIRE((NEW (NumExpr)(10))->to_pretty_string() == "10");
    }

    SECTION("Test (Add) to_pretty_string") {
        // lhs: num

        // num + num
        auto add_num_num = NEW (Add)(NEW (NumExpr)(1), NEW (NumExpr)(2));
        REQUIRE(add_num_num->to_pretty_string() == "1 + 2");

        // num + variable
        auto add_num_variable = NEW (Add)(NEW (NumExpr)(1), NEW (Var)("x"));
        REQUIRE(add_num_variable->to_pretty_string() == "1 + x");

        // num + add
        auto add_num_add = NEW (Add)(NEW (NumExpr)(1), add_num_num);
        REQUIRE(add_num_add->to_pretty_string() == "1 + 1 + 2");

        // num + mult
        auto mult_num_num = NEW (Mult)(NEW (NumExpr)(1), NEW (NumExpr)(2));
        auto add_num_mult = NEW (Add)(NEW (NumExpr)(1), mult_num_num);
        REQUIRE(add_num_mult->to_pretty_string() == "1 + 1 * 2");

        // lhs: variable

        // variable + num
        auto add_variable_num = NEW (Add)(NEW (Var)("x"), NEW (NumExpr)(1));
        REQUIRE(add_variable_num->to_pretty_string() == "x + 1");

        // variable + variable
        auto add_variable_variable = NEW (Add)(NEW (Var)("x"), NEW (Var)("y"));
        REQUIRE(add_variable_variable->to_pretty_string() == "x + y");

        // variable + add
        auto add_num_num_2 = NEW (Add)(NEW (NumExpr)(1), NEW (NumExpr)(2));
        auto add_variable_add = NEW (Add)(NEW (Var)("x"), add_num_num_2);
        REQUIRE(add_variable_add->to_pretty_string() == "x + 1 + 2");

        // variable + mult
        auto mult_num_num_2 = NEW (Mult)(NEW (NumExpr)(1), NEW (NumExpr)(2));
        auto add_variable_mult = NEW (Add)(NEW (Var)("x"), mult_num_num_2);
        REQUIRE(add_variable_mult->to_pretty_string() == "x + 1 * 2");

        // lhs: add
        auto add_num_num_lhs = NEW (Add)(NEW (NumExpr)(1), NEW (NumExpr)(2));

        // add + num
        auto add_add_num = NEW (Add)(add_num_num_lhs, NEW (NumExpr)(3));
        REQUIRE(add_add_num->to_pretty_string() == "(1 + 2) + 3");

        // add + variable
        auto add_add_variable = NEW (Add)(add_num_num_lhs, NEW (Var)("x"));
        REQUIRE(add_add_variable->to_pretty_string() == "(1 + 2) + x");

        // add + add
        auto add_add_add = NEW (Add)(add_num_num_lhs, add_num_num_lhs);
        REQUIRE(add_add_add->to_pretty_string() == "(1 + 2) + 1 + 2");

        // add + mult
        auto mult_num_num_3 = NEW (Mult)(NEW (NumExpr)(2), NEW (NumExpr)(3));
        auto add_add_mult = NEW (Add)(add_num_num_lhs, mult_num_num_3);
        REQUIRE(add_add_mult->to_pretty_string() == "(1 + 2) + 2 * 3");

        // lhs: mult
        auto mult_num_num_lhs = NEW (Mult)(NEW (NumExpr)(2), NEW (NumExpr)(3));

        // mult + num
        auto add_mult_num = NEW (Add)(mult_num_num_lhs, NEW (NumExpr)(1));
        REQUIRE(add_mult_num->to_pretty_string() == "2 * 3 + 1");

        // mult + variable
        auto add_mult_variable = NEW (Add)(mult_num_num_lhs, NEW (Var)("x"));
        REQUIRE(add_mult_variable->to_pretty_string() == "2 * 3 + x");

        // mult + add
        auto add_num_num_3 = NEW (Add)(NEW (NumExpr)(1), NEW (NumExpr)(2));
        auto add_mult_add = NEW (Add)(mult_num_num_lhs, add_num_num_3);
        REQUIRE(add_mult_add->to_pretty_string() == "2 * 3 + 1 + 2");

        // mult + mult
        auto add_mult_mult = NEW (Add)(mult_num_num_lhs, mult_num_num_lhs);
        REQUIRE(add_mult_mult->to_pretty_string() == "2 * 3 + 2 * 3");
    }

    SECTION("Test (Mult) to_pretty_string") {
        // lhs: num
        // num * num
        auto mult_num_num = NEW (Mult)(NEW (NumExpr)(1), NEW (NumExpr)(2));
        REQUIRE(mult_num_num->to_pretty_string() == "1 * 2");

        // num * variable
        auto mult_num_variable = NEW (Mult)(NEW (NumExpr)(1), NEW (Var)("x"));
        REQUIRE(mult_num_variable->to_pretty_string() == "1 * x");

        // num * add
        auto add_num_num = NEW (Add)(NEW (NumExpr)(1), NEW (NumExpr)(2));
        auto mult_num_add = NEW (Mult)(NEW (NumExpr)(3), add_num_num);
        REQUIRE(mult_num_add->to_pretty_string() == "3 * (1 + 2)");

        // num * mult
        auto mult_num_mult = NEW (Mult)(NEW (NumExpr)(3), mult_num_num);
        REQUIRE(mult_num_mult->to_pretty_string() == "3 * 1 * 2");

        // lhs: variable
        // variable * num
        auto mult_variable_num = NEW (Mult)(NEW (Var)("x"), NEW (NumExpr)(1));
        REQUIRE(mult_variable_num->to_pretty_string() == "x * 1");

        // variable * variable
        auto mult_variable_variable = NEW (Mult)(NEW (Var)("x"), NEW (Var)("y"));
        REQUIRE(mult_variable_variable->to_pretty_string() == "x * y");

        // variable * add
        auto add_num_num_2 = NEW (Add)(NEW (NumExpr)(2), NEW (NumExpr)(3));
        auto mult_variable_add = NEW (Mult)(NEW (Var)("x"), add_num_num_2);
        REQUIRE(mult_variable_add->to_pretty_string() == "x * (2 + 3)");

        // variable * mult
        auto mult_variable_mult = NEW (Mult)(NEW (Var)("x"), mult_num_num);
        REQUIRE(mult_variable_mult->to_pretty_string() == "x * 1 * 2");

        // lhs: add
        auto add_num_num_lhs = NEW (Add)(NEW (NumExpr)(1), NEW (NumExpr)(2));

        // add * num
        auto mult_add_num = NEW (Mult)(add_num_num_lhs, NEW (NumExpr)(3));
        REQUIRE(mult_add_num->to_pretty_string() == "(1 + 2) * 3");

        // add * variable
        auto mult_add_variable = NEW (Mult)(add_num_num_lhs, NEW (Var)("x"));
        REQUIRE(mult_add_variable->to_pretty_string() == "(1 + 2) * x");

        // add * add
        auto mult_add_add = NEW (Mult)(add_num_num_lhs, add_num_num_lhs);
        REQUIRE(mult_add_add->to_pretty_string() == "(1 + 2) * (1 + 2)");

        // add * mult
        auto mult_num_num_3 = NEW (Mult)(NEW (NumExpr)(2), NEW (NumExpr)(3));
        auto mult_add_mult = NEW (Mult)(add_num_num_lhs, mult_num_num_3);
        REQUIRE(mult_add_mult->to_pretty_string() == "(1 + 2) * 2 * 3");

        // lhs: mult
        auto mult_num_num_lhs = NEW (Mult)(NEW (NumExpr)(2), NEW (NumExpr)(3));

        // mult * num
        auto mult_mult_num = NEW (Mult)(mult_num_num_lhs, NEW (NumExpr)(4));
        REQUIRE(mult_mult_num->to_pretty_string() == "(2 * 3) * 4");

        // mult * variable
        auto mult_mult_variable = NEW (Mult)(mult_num_num_lhs, NEW (Var)("x"));
        REQUIRE(mult_mult_variable->to_pretty_string() == "(2 * 3) * x");

        // mult * add
        auto add_num_num_4 = NEW (Add)(NEW (NumExpr)(1), NEW (NumExpr)(2));
        auto mult_mult_add = NEW (Mult)(mult_num_num_lhs, add_num_num_4);
        REQUIRE(mult_mult_add->to_pretty_string() == "(2 * 3) * (1 + 2)");

        // mult * mult
        auto mult_mult_mult = NEW (Mult)(mult_num_num_lhs, mult_num_num_lhs);
        REQUIRE(mult_mult_mult->to_pretty_string() == "(2 * 3) * 2 * 3");
    }

    SECTION("Test Variable to_pretty_string") {
        REQUIRE((NEW (Var)("x"))->to_pretty_string() == "x");
    }
    
    SECTION("Universal Examples")
    {
        CHECK((NEW (Mult)( NEW (Mult)(NEW (NumExpr)(10), NEW (Mult)(NEW (Mult)(NEW (NumExpr)(10), NEW (NumExpr)(10)), NEW (NumExpr)(10))), NEW (Mult)(NEW (NumExpr)(10), NEW (NumExpr)(10))))
              ->to_pretty_string()  == "(10 * (10 * 10) * 10) * 10 * 10");
        }
    
}


// ------------------------------------------------------------------------------------------------------------------------------------------------------------


TEST_CASE(" Tests for (Let) class written by me")
{
    SECTION("General Examples for (Let) to_pretty_string")
    {
        
        CHECK((NEW (Let)("x", NEW (NumExpr)(5), NEW (Add)(NEW (Let)("y", NEW (NumExpr)(3), NEW (Add)(NEW (Var)("y"), NEW (NumExpr)(2))), NEW (Var)("x"))))
              ->to_pretty_string() == "_let x = 5\n"
                                      "_in  (_let y = 3\n"
                                      "      _in  y + 2) + x") ;


        CHECK((NEW (Mult)(NEW (Mult)(NEW (NumExpr) (2), NEW (Let)("x", NEW (NumExpr)(5), NEW (Add)(NEW (Var)("x") , NEW (NumExpr)(1)))), NEW (NumExpr)(3)))
              ->to_pretty_string() == "(2 * _let x = 5\n"
                                      "     _in  x + 1) * 3") ;


        auto expression1 = NEW (Let)("x", NEW (NumExpr)(5), NEW (Add)(NEW (Let)("y", NEW (NumExpr)(3), NEW (Add)(NEW (Var)("y"), NEW (NumExpr)(2))), NEW (Var)("x"))) ;

        auto expression2 = NEW (Let)("z", NEW (NumExpr)(9), NEW (Add)(expression1, NEW (Var)("z"))) ;

        CHECK(expression2->to_pretty_string() == "_let z = 9\n"
                                                 "_in  (_let x = 5\n"
                                                 "      _in  (_let y = 3\n"
                                                 "            _in  y + 2) + x) + z") ;


        auto expression3 = NEW (Add)(NEW (Let)("x", NEW (NumExpr)(5), NEW (Var)("x")), NEW (NumExpr)(1));
        REQUIRE(expression3->to_pretty_string() == "(_let x = 5\n"
                                                      " _in  x) + 1");

        auto expression4 = NEW (Let)("x", NEW (NumExpr)(5), NEW (Add)(NEW (Var)("x"), NEW (NumExpr)(1)));
        auto expression5 = NEW (Mult)(NEW (NumExpr)(5), expression4);
        REQUIRE(expression5->to_pretty_string() == "5 * _let x = 5\n"
                                                   "    _in  x + 1");

        auto expression6 = NEW (Add)(NEW (NumExpr)(5), expression4);
        REQUIRE(expression6->to_pretty_string() == "5 + _let x = 5\n"
                                                   "    _in  x + 1");

        auto expression7 = NEW (Add)(expression5, NEW (NumExpr)(1));
        REQUIRE(expression7->to_pretty_string() == "5 * (_let x = 5\n"
                                                   "     _in  x + 1) + 1");


        auto expression8 = NEW (Let)("x", NEW (Let)("y", NEW (NumExpr)(6), NEW (Mult)(NEW (Var)("y"), NEW (NumExpr)(2))), NEW (Add)(NEW (Var)("x"), NEW (NumExpr)(1)));
        REQUIRE(expression8->to_pretty_string() == "_let x = _let y = 6\n"
                                                   "         _in  y * 2\n"
                                                   "_in  x + 1");
        }
    
    
    SECTION("Test LetBinding equals")
    {
        auto expression1 = NEW (Let)("x", NEW (NumExpr)(2), NEW (Add)(NEW (Var)("x"), NEW (NumExpr)(1)));

        REQUIRE(expression1->equals(NEW (NumExpr)(2)) == false);
        REQUIRE(expression1->equals(NEW (Mult)(NEW (Var)("x"), NEW (NumExpr)(1))) == false);

        auto expression2 = NEW (Add)(NEW (Var)("x"), NEW (NumExpr)(3));
        auto expression3 = NEW (Let)("x", NEW (NumExpr)(2), expression2) ;
        
        REQUIRE(expression2->equals(NEW (Add)(NEW (Var)("x"), NEW (NumExpr)(3))) == true);
        REQUIRE(expression3->equals(NEW (Mult)(NEW (Var)("x"), NEW (NumExpr)(1))) == false);
        
    }
    
    SECTION("Test LetBinding interp") {
            /*
             * 5 * (_let x = 5
                _in  x) + 1
             */
        auto expression1 = NEW (Let)("x", NEW (NumExpr)(5), NEW (Var)("x"));
        
        auto expression2 = NEW (Add)(NEW (Mult)(NEW (NumExpr)(5), expression1), NEW (NumExpr)(1));

            REQUIRE(expression1->interp() ->equals(NEW (NumVal)(5)));
            REQUIRE(expression2->interp() ->equals(NEW (NumVal)(26)));

        
            /*
             5 * _let x = 5
            _in  x + 1
             */

        auto expression3 = NEW (Let)("x", NEW (Add)(NEW (NumExpr)(5), NEW (NumExpr)(2)), NEW (Add)(NEW (Var)("x"), NEW (NumExpr)(1)));
        
        auto expression4 = NEW (Mult)(NEW (NumExpr)(5), expression3);
        
        REQUIRE(expression4->interp()->equals(NEW (NumVal)(40)));


            // subst interp
        auto add = NEW (Add)(NEW (Var)("x"), NEW (Var)("y"));
        auto expression5 = NEW (Let)("x", NEW (NumExpr)(3), add);
        
        }

    SECTION("Test LetBinding has_variable") {
        std::string lhs = "x";

        auto num = NEW (NumExpr)(2);
        auto exp = NEW (Add)(NEW (NumExpr)(1), NEW (Var)("y"));
        auto variable = NEW (Var)("z");

        auto num_body = NEW (NumExpr)(2);
        auto exp_body = NEW (Add)(NEW (NumExpr)(1), NEW (Var)("x"));
        auto variable_body = NEW (Var)("x");
        
    }

    SECTION("Test LetBinding subst") {

        /*   y = 4
          _let x = 3
         _in x + y
         */
        
        auto expression1 = NEW (Let)("x", NEW (NumExpr)(3), NEW (Add)(NEW (Var)("x"), NEW (Var)("y")));
        auto expression2 = NEW (Let)("x", NEW (NumExpr)(3), NEW (Add)(NEW (Var)("x"), NEW (NumExpr)(4)));

        /* subst x with 5
         _let x = 6
        _in x + 1)
        */
        auto expression3 = NEW (Let)("x", NEW (NumExpr)(6),NEW (Add)(NEW (Var)("x"), NEW (NumExpr)(1)));
        
    }

    SECTION("Test LetBinding to_string")
    {
        
        auto expression1 = NEW (Let)("y", NEW (NumExpr)(3), NEW (Add)(NEW (Var)("y"), NEW (NumExpr)(2)));
        
        auto expression2 = NEW (Let)("x", NEW (NumExpr)(5),NEW (Add)(expression1, NEW (Var)("x")));
        
        
        REQUIRE(expression1->to_string() == "(_let y=3 _in (y+2))");
        REQUIRE(expression2->to_string() == "(_let x=5 _in ((_let y=3 _in (y+2))+x))");
    }

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------------


TEST_CASE("Pretty Print examples_Kevin") //Created from assignment examples

{
    std::stringstream out("");
    (NEW (Let)("x", NEW (NumExpr)(5), NEW (Add)(NEW (Var)("x"), NEW (NumExpr)(1))))->pretty_print(out);
    CHECK(out.str() == "_let x = 5\n"
                       "_in  x + 1");
    out.str(std::string());
    (NEW (Add)(NEW (Let)("x", NEW (NumExpr)(5), NEW (Var)("x")), NEW (NumExpr)(1)))->pretty_print(out);
    CHECK(out.str() == "(_let x = 5\n"
                       " _in  x) + 1");
    out.str(std::string());
    (NEW (Mult)(NEW (NumExpr)(5), NEW (Let)("x", NEW (NumExpr)(5), NEW (Add)(NEW (Var)("x"), NEW (NumExpr)(1)))))->pretty_print(out);
    CHECK(out.str() == "5 * _let x = 5\n"
                       "    _in  x + 1");
    out.str(std::string());
    (NEW (Add)(NEW (Mult)(NEW (NumExpr)(5), NEW (Let)("x", NEW (NumExpr)(5), NEW (Var)("x"))), NEW (NumExpr)(1)))->pretty_print(out);
    CHECK(out.str() == "5 * (_let x = 5\n"
                       "     _in  x) + 1");
    out.str(std::string());
    (NEW (Let)("x", NEW (NumExpr)(5), NEW (Add)(NEW (Let)("y", NEW (NumExpr)(3), NEW (Add)(NEW (Var)("y"), NEW (NumExpr)(2))), NEW (Var)("x"))))
            ->pretty_print(out);

    CHECK(out.str() == "_let x = 5\n"
                       "_in  (_let y = 3\n"
                       "      _in  y + 2) + x");
    out.str(std::string());
    (NEW (Let)("x", NEW (Let)("y", NEW (NumExpr)(6), NEW (Mult)(NEW (Var)("y"), NEW (NumExpr)(2))), NEW (Add)(NEW (Var)("x"), NEW (NumExpr)(1))))->pretty_print(out);
    CHECK(out.str() == "_let x = _let y = 6\n"
                       "         _in  y * 2\n"
                       "_in  x + 1");

}

TEST_CASE("pretty_print_let_mine_some_reuse_of_kevin_triple_nested_let") {
    auto tripleNestedLet = NEW (Let)("x", NEW (NumExpr)(1),
                                   NEW (Let)("y", NEW (NumExpr)(1),
                                           NEW (Mult)(NEW (Add)(NEW (Var)("x"), NEW (Var)("y")), NEW (Var)("z"))));
    auto tripleNestedLet2 = NEW (Let)("x", NEW (NumExpr)(1),
                                    NEW (Let)("y", NEW (NumExpr)(1),
                                            NEW (Let)("z", NEW (Add)(NEW (Var)("x"), NEW (NumExpr)(1)),
                                                    NEW (Add)(NEW (Add)(NEW (Var)("x"), NEW (Var)("y")), NEW (Var)("z")))));
    auto tripleNestedLet3 = NEW (Let)("x", NEW (NumExpr)(1),
                                    NEW (Let)("y", NEW (NumExpr)(1),
                                            NEW (Let)("z", NEW (Add)(NEW (Var)("x"), NEW (NumExpr)(1)),
                                                    NEW (Mult)(NEW (Add)(NEW (Var)("x"), NEW (Var)("y")), NEW (Var)("z")))));


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
    auto tripleNestedLet4 =NEW (Let)("x", NEW (NumExpr)(5),
                                   NEW (Let)("y", NEW (NumExpr)(3),
                                           NEW (Add)(NEW (Var)("y"), NEW (NumExpr)(2))));
    auto tripleNestedLet5 =NEW (Let)("x", NEW (NumExpr)(5),
                                   NEW (Add)(NEW (Let)("y", NEW (NumExpr)(3),
                                                   NEW (Add)(NEW (Var)("y"), NEW (NumExpr)(2))), NEW (Var)("x")));
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
        CHECK( (NEW (Add)(NEW (Mult)(NEW (NumExpr)(5), NEW (Let)("x", NEW (NumExpr)(5), NEW (Var)("x"))), NEW (NumExpr)(1)))-> to_pretty_string()
               == "5 * (_let x = 5\n"
                  "     _in  x) + 1");
        CHECK( (NEW (Mult)(NEW (Mult)(NEW (NumExpr) (2), NEW (Let)("x", NEW (NumExpr)(5), NEW (Add)(NEW (Var)("x") ,NEW  (NumExpr)(1)) )), NEW (NumExpr)(3) )) -> to_pretty_string()
               == "(2 * _let x = 5\n"
                  "     _in  x + 1) * 3");
    }
    // A _let needs parentheses when it is nested immediately as the right argument of an unparenthesized *
    // where _let would have needed parentheses in the surrounding context
    // (that is, if the _let used in place of the whole * would need parentheses,
    // then it still needs parentheses within the right-hand size of *).
    SECTION("NEW_edge") {
        CHECK( (NEW (Mult)(NEW (NumExpr) (2), NEW (Let)("x", NEW (NumExpr)(5), NEW (Add)(NEW (Var)("x") ,NEW  (NumExpr)(1)) ) )) -> to_pretty_string()
               == "2 * _let x = 5\n"
                  "    _in  x + 1");
        CHECK( (NEW (Add)(NEW (Mult)(NEW (NumExpr)(5), NEW (Let)("x", NEW (NumExpr)(5), NEW (Mult)(NEW (Var)("x"), NEW (NumExpr)(2)))), NEW (NumExpr)(1)))-> to_pretty_string()
               == "5 * (_let x = 5\n"
                  "     _in  x * 2) + 1");
        CHECK( (NEW (Mult)((NEW (Add)(NEW (Mult)(NEW (NumExpr)(5), NEW (Let)("x", NEW (NumExpr)(5), NEW (Mult)(NEW (Var)("x"), NEW (NumExpr)(2)))), NEW (NumExpr)(1))), NEW (NumExpr)(7)))-> to_pretty_string()
               == "(5 * (_let x = 5\n"
                  "      _in  x * 2) + 1) * 7");
        CHECK( (NEW (Let)("x", NEW (NumExpr)(10), NEW (Mult)( NEW (Mult)(NEW (Var)("x"), NEW (Mult)(NEW (Mult)(NEW (NumExpr)(10), NEW (NumExpr)(10)), NEW (NumExpr)(10))), NEW (Mult)(NEW (NumExpr)(10), NEW (NumExpr)(10)))))
                       ->to_pretty_string()  == "_let x = 10\n"
                                                      "_in  (x * (10 * 10) * 10) * 10 * 10");
        CHECK( (NEW (Let)("x", NEW (NumExpr)(1), NEW (Mult)( NEW (Mult)(NEW (Var)("x"), NEW (Mult)(NEW (Mult)(NEW (NumExpr)(10), NEW (NumExpr)(10)), NEW (Var)("x"))), NEW (Mult)(NEW (NumExpr)(10), NEW (NumExpr)(10)))))
                        -> to_pretty_string() == "_let x = 1\n"
                                                       "_in  (x * (10 * 10) * x) * 10 * 10" );
        CHECK( (NEW (Let)("x", NEW (NumExpr)(1), NEW (Mult)( NEW (Mult)(NEW (Var)("x"), NEW (Mult)(NEW (Mult)(NEW (NumExpr)(10), NEW (NumExpr)(10)), NEW (Var)("x"))), NEW (Mult)(NEW (Var)("y"), NEW (NumExpr)(10)))))
                       -> to_pretty_string() == "_let x = 1\n"
                                                      "_in  (x * (10 * 10) * x) * y * 10" );
    }
}

TEST_CASE("Let_equals_mine") {
        SECTION("Values_same") {
            REQUIRE( (NEW (Let)("x", NEW (NumExpr)(4), NEW (Add)(NEW (NumExpr)(2), NEW (Var)("x")) ))->equals(NEW (Let)("x", NEW (NumExpr)(4), NEW (Add)(NEW (NumExpr)(2), NEW (Var)("x")) )));
        }
        SECTION("Values_same_different_rhs") {
            REQUIRE( !(NEW (Let)("x", NEW (NumExpr)(4), NEW (Add)(NEW (NumExpr)(2), NEW (Var)("x")) ))->equals(NEW (Let)("x", NEW (NumExpr)(5), NEW (Add)(NEW (NumExpr)(2), NEW (Var)("x")) )));
        }
        SECTION("Values_same_different_lhs") {
            REQUIRE( !(NEW (Let)("x", NEW (NumExpr)(4), NEW (Add)(NEW (NumExpr)(2), NEW (Var)("x")) ))->equals(NEW (Let)("y", NEW (NumExpr)(4), NEW (Add)(NEW (NumExpr)(2), NEW (Var)("x")) )));
        }
        SECTION("Values_same_different_body") {
            REQUIRE( !(NEW (Let)("x", NEW (NumExpr)(4), NEW (Add)(NEW (NumExpr)(2), NEW (Var)("x")) ))->equals(NEW (Let)("x", NEW (NumExpr)(4), NEW (Mult)(NEW (NumExpr)(3), NEW (Var)("y")) )));
        }
        SECTION("different_types") {
            REQUIRE( !(NEW (Let)("x", NEW (NumExpr)(4), NEW (Add)(NEW (NumExpr)(2), NEW (Var)("x")) ))->equals( NEW (Mult)(NEW (NumExpr)(3), NEW (Var)("y")) ));
        }
}

TEST_CASE("Let_print_mine") {
    CHECK( (NEW (Let)("x", NEW (NumExpr)(5), NEW (Add)(NEW (Let)("y", NEW (NumExpr)(3), NEW (Add)(NEW (Var)("y"), NEW (NumExpr)(2))), NEW (Var)("x")))) -> to_string()
                                                                                                        == "(_let x=5 _in ((_let y=3 _in (y+2))+x))");
    CHECK( (NEW (Let)("x", NEW (NumExpr)(1),
                   NEW (Let)("y", NEW (NumExpr)(1),
                           NEW (Let)("z", NEW (Add)(NEW (Var)("x"), NEW (NumExpr)(1)),
                                   NEW (Mult)(NEW (Add)(NEW (Var)("x"), NEW (Var)("y")), NEW (Var)("z")))))) -> to_string()
                                                                                                        == "(_let x=1 _in (_let y=1 _in (_let z=(x+1) _in ((x+y)*z))))");
}
TEST_CASE ("Let_interp_mine") {
    SECTION("hw_examples") {
        CHECK((NEW (Add)(NEW (Mult)(NEW (NumExpr)(5), NEW (Let)("x", NEW (NumExpr)(5), NEW (Var)("x"))), NEW (NumExpr)(1))) -> interp()->equals(NEW (NumVal)(26)));
        CHECK((NEW (Mult)(NEW (NumExpr)(5), NEW (Let)("x", NEW (NumExpr)(5), NEW (Add)(NEW (Var)("x"), NEW (NumExpr)(1))))) -> interp()->equals(NEW (NumVal)(30)));
    }
    SECTION("from_pretty_print_edge") {
        CHECK( (NEW (Let)("x", NEW (NumExpr)(1),
                       NEW (Let)("y", NEW (NumExpr)(1),
                               NEW (Let)("z", NEW (Add)(NEW (Var)("x"), NEW (NumExpr)(1)),
                                       NEW (Mult)(NEW (Add)(NEW (Var)("x"), NEW (Var)("y")), NEW (Var)("z")))))) -> interp()->equals(NEW (NumVal)(4)));
        CHECK( (NEW (Mult)((NEW (Add)(NEW (Mult)(NEW (NumExpr)(5), NEW (Let)("x", NEW (NumExpr)(5), NEW (Mult)(NEW (Var)("x"), NEW (NumExpr)(2)))), NEW (NumExpr)(1))), NEW (NumExpr)(7))) -> interp() ->equals(NEW (NumVal)(357))); // 51 * 7
        CHECK( (NEW (Let)("x", NEW (NumExpr)(10), NEW (Mult)( NEW (Mult)(NEW (Var)("x"), NEW (Mult)(NEW (Mult)(NEW (NumExpr)(10), NEW (NumExpr)(10)), NEW (NumExpr)(10))), NEW (Mult)(NEW (NumExpr)(10), NEW (NumExpr)(10)))))
                      ->interp()->equals(NEW (NumVal)(1000000)));
        CHECK( (NEW (Let)("x", NEW (NumExpr)(1), NEW (Mult)( NEW (Mult)(NEW (Var)("x"), NEW (Mult)(NEW (Mult)(NEW (NumExpr)(10), NEW (NumExpr)(10)), NEW (Var)("x"))), NEW (Mult)(NEW (NumExpr)(10), NEW (NumExpr)(10)))))
                       ->interp()->equals(NEW (NumVal)(10000)));
    }
    SECTION("bypass_middle_let") {
        CHECK ((NEW (Let)("x", NEW (NumExpr)(2), NEW (Let)("z", NEW (NumExpr)(4), NEW (Add)(NEW (Var)("x"), NEW (NumExpr)(10)))))
                -> interp()->equals(NEW (NumVal)(12)));
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------


TEST_CASE("Provided tests for parse ") {
  CHECK_THROWS_WITH( parse_expression_str("()"), "invalid input" );
  
  CHECK( parse_expression_str("(1)")->equals(NEW (NumExpr)(1)) );
  CHECK( parse_expression_str("(((1)))")->equals(NEW (NumExpr)(1)) );
  
  CHECK_THROWS_WITH( parse_expression_str("(1"), "missing close parenthesis" );
  
  CHECK( parse_expression_str("1")->equals(NEW (NumExpr)(1)) );
  CHECK( parse_expression_str("10")->equals(NEW (NumExpr)(10)) );
  CHECK( parse_expression_str("-3")->equals(NEW (NumExpr)(-3)) );
  CHECK( parse_expression_str("  \n 5  ")->equals(NEW (NumExpr)(5)) );
  CHECK_THROWS_WITH( parse_expression_str("-"), "number should come right after -" );
  CHECK_THROWS_WITH( parse_expression_str(" -   5  "), "number should come right after -" );
  
  CHECK( parse_expression_str("x")->equals(NEW (Var)("x")) );
  CHECK( parse_expression_str("xyz")->equals(NEW (Var)("xyz")) );
  CHECK( parse_expression_str("xYz")->equals(NEW (Var)("xYz")) );
  CHECK_THROWS_WITH( parse_expression_str("x_z"), "unexpected character in variable" );
  
  CHECK( parse_expression_str("x + y")->equals(NEW (Add)(NEW (Var)("x"), NEW (Var)("y"))) );
  CHECK( parse_expression_str("x * y")->equals(NEW (Mult)(NEW (Var)("x"), NEW (Var)("y"))) );
  CHECK( parse_expression_str("z * x + y")
        ->equals(NEW (Add)(NEW (Mult)(NEW (Var)("z"), NEW (Var)("x")),
                         NEW (Var)("y"))) );
  
  CHECK( parse_expression_str("z * (x + y)")
        ->equals(NEW (Mult)(NEW (Var)("z"),
                          NEW (Add)(NEW (Var)("x"), NEW (Var)("y"))) ));
}

TEST_CASE( "parse" ) {
CHECK((NEW (NumExpr)(1))->equals(NEW (NumExpr)(1)));
CHECK(!(NEW (NumExpr)(1))->equals(NEW (NumExpr)(2)));
CHECK(!(NEW (NumExpr)(1))->equals(NEW (Mult) (NEW (NumExpr)(2), NEW (NumExpr)(4))));
CHECK((NEW (Var)("hello"))->equals(NEW (Var)("hello")));
CHECK(!(NEW (Var)("hello"))->equals(NEW (Var)("ello")));
CHECK(parse_expression_str("1")->equals(NEW (NumExpr)(1)));
CHECK(parse_expression_str("  1")->equals(NEW (NumExpr)(1)));
CHECK(parse_expression_str("4+2")->equals(NEW (Add)(NEW (NumExpr)(4), NEW (NumExpr)(2))));
CHECK(parse_expression_str("   45 +    23")->equals(NEW (Add)(NEW (NumExpr)(45), NEW (NumExpr)(23))));
CHECK(parse_expression_str("   45   *  23   ")->equals(NEW (Mult)(NEW (NumExpr)(45), NEW (NumExpr)(23))));
CHECK(parse_expression_str("(90)")->equals(NEW (NumExpr)(90)));
CHECK(parse_expression_str("3*(2+43)")->equals(NEW (Mult)(NEW (NumExpr)(3),NEW (Add)(NEW (NumExpr)(2), NEW (NumExpr)(43)))));
CHECK(parse_expression_str("Hello")->equals(NEW (Var)("Hello")));
CHECK(parse_expression_str("3*(2+width)")->equals(NEW (Mult)(NEW (NumExpr)(3),NEW (Add)(NEW (NumExpr)(2), NEW (Var)("width")))));
}

TEST_CASE("Test cases for parse") {
    REQUIRE_THROWS_WITH(parse_expression_str("()"), "invalid input");

    REQUIRE(parse_expression_str("    1     ")->equals(NEW (NumExpr)(1)));
    REQUIRE(parse_expression_str("(   1   )")->equals(NEW (NumExpr)(1)));
    REQUIRE(parse_expression_str("(1)")->equals(NEW (NumExpr)(1)));
    REQUIRE(parse_expression_str("(((1)))")->equals(NEW (NumExpr)(1)));


    REQUIRE_THROWS_WITH(parse_expression_str("(1"), "missing close parenthesis");
    REQUIRE_THROWS_WITH(parse_expression_str("( 1    "), "missing close parenthesis");

    REQUIRE(parse_expression_str("1")->equals(NEW (NumExpr)(1)));
    REQUIRE(parse_expression_str("10")->equals(NEW (NumExpr)(10)));
    REQUIRE(parse_expression_str("-3")->equals(NEW (NumExpr)(-3)));

    REQUIRE(parse_expression_str("  \n 5  ")->equals(NEW (NumExpr)(5)));

    REQUIRE_THROWS_WITH(parse_expression_str("-"), "number should come right after -");
    REQUIRE_THROWS_WITH(parse_expression_str(" -   5  "), "number should come right after -");

    REQUIRE(parse_expression_str(" ( 2 + 3) * 5 + 1 ")->interp()->equals(NEW (NumVal)(26)));
    REQUIRE(parse_expression_str(" 2 + 3  * 5 + 1 ")->interp()->equals(NEW (NumVal)(18)));
    REQUIRE(parse_expression_str(" 2 + 3  * (5 + 1 )")->interp()->equals(NEW (NumVal)(20)));

    REQUIRE_THROWS_WITH(parse_expression_str(" 1 - 3")->interp(), "invalid input");
    REQUIRE_THROWS_WITH(parse_expression_str(" 1 - ")->interp(), "invalid input");

    SECTION("variable") {

        REQUIRE(parse_expression_str("x")->equals(NEW (Var)("x")));
        REQUIRE(parse_expression_str("xyz")->equals(NEW (Var)("xyz")));
        REQUIRE(parse_expression_str("xYz")->equals(NEW (Var)("xYz")));

        REQUIRE_THROWS_WITH(parse_expression_str("x_z"), "unexpected character in variable");

        REQUIRE(parse_expression_str("x + y")->equals(NEW (Add)(NEW (Var)("x"), NEW (Var)("y"))));

        REQUIRE(parse_expression_str("x * y")->equals(NEW (Mult)(NEW (Var)("x"), NEW (Var)("y"))));

        REQUIRE(parse_expression_str("z * x + y")
                        ->equals(NEW (Add)(NEW (Mult)(NEW (Var)("z"), NEW (Var)("x")),
                                         NEW (Var)("y"))));

        REQUIRE(parse_expression_str("z * (x + y)")
                        ->equals(NEW (Mult)(NEW (Var)("z"),
                                          NEW (Add)(NEW (Var)("x"), NEW (Var)("y")))));
    }

    SECTION("let binding") {
        auto expression1 = NEW (Add)(NEW (Var)("x"), NEW (NumExpr)(1));
        auto expression2 = NEW (Let)("x", NEW (NumExpr)(5), expression1);
        REQUIRE(parse_expression_str("_let x = 5 _in x + 1")->equals(expression2));

        auto expression3 = NEW (Let)("x", NEW (Add)(NEW (Var)("x"), NEW (NumExpr)(1)), NEW (Add)(NEW (Var)("x"), NEW (NumExpr)(7)));
        auto expression4 = NEW (Let)("x", NEW (NumExpr)(5), expression3);
        REQUIRE(parse_expression_str("_let x = 5 _in _let x = x +1    _in  x + 7")->equals(expression4));

        REQUIRE_THROWS_WITH(parse_expression_str("_lt x = 5 _in x + 1"), "consume mismatch");
        REQUIRE_THROWS_WITH(parse_expression_str("_let x  5 _in x + 1"), "invalid input");
        REQUIRE_THROWS_WITH(parse_expression_str("_let x = 5 _i x + 1"), "invalid input");
        REQUIRE_THROWS_WITH(parse_expression_str("_let x = 5 x + 1"), "invalid input");
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("(NumVal)") {
    SECTION("add_to") {
        auto num1 = NEW (NumVal)(23);
        auto num2 = NEW (NumVal)(27);
        auto num3 = NEW (NumVal)(50);
        
        auto add1 = num1->add_to(num2);
        auto add2 = num2->add_to(num1);

        REQUIRE(add1->equals(num3));
        REQUIRE(add2->equals(num3));

        REQUIRE_THROWS_WITH(num3->add_to(nullptr), "add of non-number");
    }

    SECTION("mult_with") {
        auto num1 = NEW (NumVal)(20);
        auto num2 = NEW (NumVal)(5);
        auto num3 = NEW (NumVal)(100);
        
        auto mult1 = num1->mult_with(num2);
        auto mult2 = num2->mult_with(num1);

        REQUIRE(mult1->equals(num3));
        REQUIRE(mult2->equals(num3));

        REQUIRE_THROWS_WITH(num3->mult_with(nullptr), "mult with non-number");
    }

    SECTION("equals") {
        auto num1 = NEW (NumVal)(2);
        auto num2 = NEW (NumVal)(3);
        auto num3 = NEW (NumVal)(2);

        REQUIRE(num1->equals(num2) == false);
        REQUIRE(num1->equals(num3));
        REQUIRE(num1->equals(num1));
        REQUIRE(num1->equals(nullptr) == false);
    }

    SECTION("to_string") {
        auto num1 = NEW (NumVal)(2);
        auto num2 = NEW (NumVal)(-2);

        REQUIRE(num1->to_string() == std::to_string(2));
        REQUIRE(num2->to_string() == std::to_string(-2));
    }

    SECTION("to_expr") {
        auto num1 = NEW (NumVal)(2);
        auto num2 = NEW (NumVal)(-2);

        auto num_expr1 = NEW (NumExpr)(2);
        auto num_expr2 = NEW (NumExpr)(-2);

        REQUIRE(num1->to_expr()->equals(num_expr1));
        REQUIRE(num2->to_expr()->equals(num_expr2));
        REQUIRE(num1->to_expr()->equals(num_expr2) == false);
        REQUIRE(num2->to_expr()->equals(num_expr1) == false);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------


TEST_CASE("Bool Val") {

    auto bool_false = NEW (BoolVal)(false);
    auto bool_true = NEW (BoolVal)(true);
    auto bool_true_1 = NEW (BoolVal)(true);
    auto num = NEW (NumVal)(1);

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

    auto bool_true = NEW (BoolExpr)(true);
    auto bool_true_1 = NEW (BoolExpr)(true);
    auto bool_false = NEW (BoolExpr)(false);


    SECTION("equals") {
        REQUIRE(bool_true->equals(bool_true_1));
        REQUIRE(bool_false->equals(bool_true) == false);

        auto num_2 = NEW (NumExpr)(2);
        REQUIRE(bool_true->equals(num_2) == false);

        auto add_2_3 = NEW (Add)(NEW (NumExpr)(2), NEW (NumExpr)(3));
        REQUIRE(bool_true->equals(add_2_3) == false);

        auto mult_2_3 = NEW (Mult)(NEW (NumExpr)(2), NEW (NumExpr)(3));
        REQUIRE(bool_true->equals(mult_2_3) == false);

        auto var_x = NEW (Var)("x");
        REQUIRE(bool_true->equals(var_x) == false);

        auto lb_x_5_x = NEW (Let)("x", NEW (NumExpr)(2), NEW (Var)("x"));
        REQUIRE(bool_true->equals(lb_x_5_x) == false);
    }

    SECTION("interp") {
        REQUIRE(bool_true->interp()->equals(NEW (BoolVal)(true)));
        REQUIRE(bool_false->interp()->equals(NEW (BoolVal)(false)));
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

    auto add_expr = NEW (Add)(NEW (NumExpr)(3), NEW (NumExpr)(4));
    auto mult_expr = NEW (Mult)(NEW (NumExpr)(1), NEW (NumExpr)(2));
    // x = 2, 1 + x => 3
    auto let_add_expr = NEW (Let)("x", NEW (NumExpr)(2), NEW (Add)(NEW (NumExpr)(1), NEW (Var)("x")));
    // (_if _true _then 2 _else (3 + 4))
    auto if_true_then_num_else_add_expr = NEW (IfExpr)(NEW (BoolExpr)(true), NEW (NumExpr)(2), add_expr);
    auto if_true_then_num_else_add_expr1 = NEW (IfExpr)(NEW (BoolExpr)(true), NEW (NumExpr)(2), add_expr);
    auto if_false_then_num_else_add_expr = NEW (IfExpr)(NEW (BoolExpr)(false), NEW (NumExpr)(2), add_expr);
    auto if_true_then_mult_else_add_expr = NEW (IfExpr)(NEW (BoolExpr)(true), mult_expr, add_expr);
    auto if_true_then_num_else_let_expr = NEW (IfExpr)(NEW (BoolExpr)(true), NEW (NumExpr)(2), let_add_expr);


    SECTION("equals") {
        REQUIRE(if_true_then_num_else_add_expr->equals(if_true_then_num_else_add_expr1));
        REQUIRE(if_true_then_num_else_add_expr->equals(if_false_then_num_else_add_expr) == false);
        REQUIRE(if_true_then_num_else_add_expr->equals(if_true_then_mult_else_add_expr) == false);
        REQUIRE(if_true_then_num_else_add_expr->equals(if_true_then_num_else_let_expr) == false);
    }

    auto if_false_then_2_else_lb = NEW (IfExpr)(NEW (BoolExpr)(false), NEW (NumExpr)(2), let_add_expr);


    SECTION("interp") {
        REQUIRE(if_true_then_num_else_add_expr->interp()->equals(NEW (NumVal)(2)));
        REQUIRE(if_false_then_num_else_add_expr->interp()->equals(add_expr->interp()));
        REQUIRE(if_true_then_mult_else_add_expr->interp()->equals(mult_expr->interp()));
        REQUIRE(if_false_then_2_else_lb->interp()->equals(let_add_expr->interp()));
    }

    auto add_x_1 = NEW (Add)(NEW (Var)("x"), NEW (NumExpr)(1));
    // x + 1
    auto if_true_then_add_x_1_else_3 = NEW (IfExpr)(NEW (BoolExpr)(true), add_x_1, NEW (NumExpr)(3));
    auto add_3_1 = NEW (Add)(NEW (NumExpr)(3), NEW (NumExpr)(1));
    auto if_true_then_add_3_1_else_3 = NEW (IfExpr)(NEW (BoolExpr)(true), add_3_1, NEW (NumExpr)(3));

    auto if_bool_expr_true_then_2_else_3 = NEW (IfExpr)(NEW (BoolExpr)(true), NEW (NumExpr)(2), NEW (NumExpr)(3));


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
    auto num2 = NEW (NumExpr)(2);
    auto num3 = NEW (NumExpr)(3);
    auto add_expr = NEW (Add)(num2, num3);
    auto eq_expr = NEW (EqExpr)(num2, num3);
    // 2 == 2 + 3
    auto eq_num_add = NEW (EqExpr)(num2, add_expr);
    auto eq_num_num = NEW (EqExpr)(num2, NEW (NumExpr)(4));
    
    SECTION("equals") {
            REQUIRE(eq_expr->equals(eq_num_add) == false);
            REQUIRE(eq_expr->equals(eq_num_num) == false);
            REQUIRE(eq_expr->equals(eq_expr));
            REQUIRE(eq_expr->equals(num2) == false);
        }

        auto eq_num_2_num_2 = NEW (EqExpr)(num2, num2);
        auto eq_num_1_bool_false = NEW (EqExpr)(NEW (NumExpr)(1), NEW (BoolExpr)(false));

    /*    _let same = 1 == 2
        _in  _if 1 == 2
        _then _false + 5
        _else 88*/
        auto eq_num_1_num_2 = NEW (EqExpr)(NEW (NumExpr)(1), num2);
        auto add_bool_false_num_5 = NEW (Add)(NEW (BoolExpr)(false), NEW (NumExpr)(5));
        auto if_eq_then_add_else_88 = NEW (IfExpr)(eq_num_1_num_2, add_bool_false_num_5, NEW (NumExpr)(88));
        auto let_same_eq_in_if = NEW (Let)("same", eq_num_1_num_2, if_eq_then_add_else_88);

        /* _if 4 + 1
         _then 2
         _else 3*/
        auto add_num_4_num_1 = NEW (Add)(NEW (NumExpr)(4), NEW (NumExpr)(1));
        auto if_add_then_2_else_3 = NEW (IfExpr)(add_num_4_num_1, NEW (NumExpr)(2), NEW (NumExpr)(3));

        // (1 == 2) + 3
        auto add_eq_1_2_num_3 = NEW (Add)(eq_num_1_num_2, NEW (NumExpr)(3));

        auto eq_num_1_add_2_3 = NEW (EqExpr)(NEW (NumExpr)(1), NEW (Add)(NEW (NumExpr)(2), NEW (NumExpr)(3)));

        // 1+1 == 2+0
        auto add_num_1_num_1 = NEW (Add)(NEW (NumExpr)(1), NEW (NumExpr)(1));
        auto add_num_2_num_0 = NEW (Add)(NEW (NumExpr)(2), NEW (NumExpr)(0));
        auto eq_add_1_add_2 = NEW (EqExpr)(add_num_1_num_1, add_num_2_num_0);

        SECTION("interp") {
            REQUIRE(eq_expr->interp()->is_true() == false);
            REQUIRE(eq_num_2_num_2->interp()->is_true());
            REQUIRE(eq_num_add->interp()->is_true() == false);
            REQUIRE(eq_num_1_bool_false->interp()->is_true() == false);
            REQUIRE(let_same_eq_in_if->interp()->equals(NEW (NumVal)(88)));
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

        auto eq_var_x_num_2 = NEW (EqExpr)(NEW (Var)("x"), num2);
        auto eq_add_x_2_num_2 = NEW (EqExpr)(NEW (Add)(NEW (Var)("x"), num2), num2);
        // _let x=2 _in x+1
        auto lb_x_2_add_x_1 = NEW (Let)("x", NEW (NumExpr)(2), NEW (Add)(NEW (Var)("x"), NEW (NumExpr)(1)));
        // _let x=2 _in x+1 == 2
        auto eq_lb_num = NEW (EqExpr)(lb_x_2_add_x_1, num2);

        auto eq_add_1_2_num_2 = NEW (EqExpr)(NEW (Add)(NEW (NumExpr)(1), NEW (NumExpr)(2)), NEW (NumExpr)(2));

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
    auto add_x_1 = NEW (Add)(NEW (Var)("x"), NEW (NumExpr)(1));
    auto add_x_2 = NEW (Add)(NEW (Var)("x"), NEW (NumExpr)(2));
    auto fun_expr_x_add_x_1 = NEW (FunExpr)("x", add_x_1);
    auto fun_expr_x_add_x_1_copy = NEW (FunExpr)("x", add_x_1);
    auto fun_expr_y_add_x_1 = NEW (FunExpr)("y", add_x_1);
    auto fun_expr_x_add_x_2 = NEW (FunExpr)("x", add_x_2);
    auto num_expr_5 = NEW (NumExpr)(5);

    // (_fun (y) (_fun (x) y + x))
    auto add_y_x = NEW (Add)(NEW (Var)("y"), NEW (Var)("x"));
    auto fun_expr_x_add_y_x = NEW (FunExpr)("x", add_y_x);
    auto fun_expr_y_fun_var_x_add_y_x = NEW (FunExpr)("y", fun_expr_x_add_y_x);
    auto fun_var_y_fun_var_x_add_y_x = NEW (FunVal)("y", fun_expr_x_add_y_x);


    SECTION("equals") {
        REQUIRE(fun_expr_x_add_x_1->equals(fun_expr_x_add_x_1_copy));
        REQUIRE(fun_expr_y_add_x_1->equals(fun_expr_x_add_x_1) == false);
        REQUIRE(fun_expr_x_add_x_2->equals(fun_expr_x_add_x_1) == false);
        REQUIRE(fun_expr_x_add_x_1->equals(num_expr_5) == false);
    }

    auto fun_val_x_add_x_1 = NEW (FunVal)("x", add_x_1);

    SECTION("interp") {
        REQUIRE(fun_expr_x_add_x_1->interp()->equals(fun_val_x_add_x_1));
        REQUIRE(fun_expr_y_fun_var_x_add_y_x->interp()->equals(fun_var_y_fun_var_x_add_y_x));
    }
    
    auto add_2_1 = NEW (Add)(NEW (NumExpr)(2), NEW (NumExpr)(1));
    auto fun_expr_x_add_2_1_subst_x_2 = NEW (FunExpr)("x", add_2_1);
    auto fun_val_x_add_2_1 = NEW (FunVal)("x", add_2_1);

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
    auto add_x_1 = NEW (Add)(NEW (Var)("x"), NEW (NumExpr)(1));
    auto add_x_2 = NEW (Add)(NEW (Var)("x"), NEW (NumExpr)(2));
    auto fun_expr_x_add_x_1 = NEW (FunExpr)("x", add_x_1);
    auto call_fun_expr_x_add_x_1_on_2 = NEW (CallExpr)(fun_expr_x_add_x_1, NEW (NumExpr)(2));
    auto call_fun_expr_x_add_x_1_on_2_copy = NEW (CallExpr)(fun_expr_x_add_x_1, NEW (NumExpr)(2));
    auto call_fun_expr_x_add_x_1_on_3 = NEW (CallExpr)(fun_expr_x_add_x_1, NEW (NumExpr)(3));
    auto fun_expr_x_add_x_2 = NEW (FunExpr)("x", add_x_2);
    auto call_fun_expr_x_add_x_2_on_2 = NEW (CallExpr)(fun_expr_x_add_x_2, NEW (NumExpr)(2));

    // (_fun (y) (_fun (x) y + x))
    auto add_y_x = NEW (Add)(NEW (Var)("y"), NEW (Var)("x"));
    auto fun_expr_x_add_y_x = NEW (FunExpr)("x", add_y_x);
    auto fun_expr_y_fun_var_x_add_y_x = NEW (FunExpr)("y", fun_expr_x_add_y_x);
    auto fun_var_y_fun_var_x_add_y_x = NEW (FunVal)("y", fun_expr_x_add_y_x);
    auto call_fun_expr_y_fun_var_x_add_y_x_on_y_2 = NEW (CallExpr)(fun_expr_y_fun_var_x_add_y_x, NEW (NumExpr)(2));
    auto call_call_fun_expr_y_fun_var_x_add_y_x_on_y_2_on_x_1 = NEW (CallExpr)(call_fun_expr_y_fun_var_x_add_y_x_on_y_2, NEW (NumExpr)(1));

    SECTION("equals") {
        REQUIRE(call_fun_expr_x_add_x_1_on_2->equals(call_fun_expr_x_add_x_1_on_2_copy));
        REQUIRE(call_fun_expr_x_add_x_1_on_2->equals(call_fun_expr_x_add_x_1_on_3) == false);
        REQUIRE(call_fun_expr_x_add_x_1_on_2->equals(add_x_1) == false);
        REQUIRE(call_fun_expr_x_add_x_1_on_2->equals(call_fun_expr_x_add_x_2_on_2) == false);
    }

    //auto call_fun_expr_x_add_x_1_on_2_subst_x_for_2 = call_fun_expr_x_add_x_1_on_2->subst("x", NEW (NumExpr)(2));
    auto add_2_1 = NEW (Add)(NEW (NumExpr)(2), NEW (NumExpr)(1));
    auto fun_expr_x_add_2_1 = NEW (FunExpr)("x", add_2_1);
    auto call_fun_expr_x_add_2_1_on_2 = NEW (CallExpr)(fun_expr_x_add_2_1, NEW (NumExpr)(2));
    //auto call_fun_expr_x_add_x_1_on_2_subst_y_for_2 = call_fun_expr_x_add_x_1_on_2->subst("y", NEW (NumExpr)(2));

    auto add_2_x = NEW (Add)(NEW (NumExpr)(2), NEW (Var)("x"));
    auto fun_val_x_add_2_x = NEW (FunVal)("x", add_2_x);

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
        auto factrl_expr = NEW (Let)(
                "factrl",
                NEW (FunExpr)(
                        "factrl",
                        NEW (FunExpr)(
                                "x",
                                NEW (IfExpr)(
                                        NEW (EqExpr)(
                                                NEW (Var)("x"),
                                                NEW (NumExpr)(1)
                                        ),
                                        NEW (NumExpr)(1),
                                        NEW (Mult)(
                                                NEW (Var)("x"),
                                                NEW (CallExpr)(
                                                        NEW (CallExpr)(
                                                                NEW (Var)("factrl"),
                                                                NEW (Var)("factrl")
                                                        ),
                                                        NEW (Add)(
                                                                NEW (Var)("x"),
                                                                NEW (NumExpr)(-1)
                                                        )
                                                )
                                        )
                                )
                        )
                ),
                NEW (CallExpr)(
                        NEW (CallExpr)(
                                NEW (Var)("factrl"),
                                NEW (Var)("factrl")
                        ),
                        NEW (NumExpr)(10)
                )
        );

        REQUIRE(factrl_expr->to_pretty_string() == "_let factrl = _fun (factrl)\n"
                                                   "                _fun (x)\n"
                                                   "                  _if x == 1\n"
                                                   "                  _then 1\n"
                                                   "                  _else x * factrl(factrl)(x + -1)\n"
                                                   "_in  factrl(factrl)(10)");

        REQUIRE(factrl_expr->interp()->equals(NEW (NumVal)(3628800)));
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------


TEST_CASE("parse Fun Expr string") {
    auto add_x_1 = NEW (Add)(NEW (Var)("x"), NEW (NumExpr)(1));
    auto fun_expr_x_add_x_1 = NEW (FunExpr)("x", add_x_1);

    REQUIRE(parse_expression_str("_fun (x)\n"
                                 "  x + 1")->equals(fun_expr_x_add_x_1));


    std::string test_str = "_let factrl = _fun (factrl)\n"
                           "                _fun (x)\n"
                           "                  _if x == 1\n"
                           "                  _then 1\n"
                           "                  _else x * factrl(factrl)(x + -1)\n"
                           "_in  factrl(factrl)(10)";

    auto parsed_expr = parse_expression_str(test_str);
    auto parsed_expr_pretty_str = parsed_expr->to_pretty_string();
    auto parsed_expr_str = parsed_expr->to_string();
    REQUIRE(parsed_expr_pretty_str == test_str);
    REQUIRE(parsed_expr->equals(parse_expression_str(parsed_expr_pretty_str)));
    REQUIRE(parsed_expr->equals(parse_expression_str(parsed_expr_str)));
    REQUIRE(parsed_expr->interp()->equals(NEW (NumVal)(3628800)));
}


TEST_CASE("parse Call Expr string") {
    auto add_x_1 = NEW (Add)(NEW (Var)("x"), NEW (NumExpr)(1));
    auto fun_expr_x_add_x_1 = NEW (FunExpr)("x", add_x_1);
    auto call_2_on_fun_expr_x_add_x_1 = NEW (CallExpr)(fun_expr_x_add_x_1, NEW (NumExpr)(2));
    REQUIRE(parse_expression_str("(_fun (x) x + 1)(2)")->equals(call_2_on_fun_expr_x_add_x_1));
    REQUIRE(parse_expression_str("(_fun (x)\n"
                                 "   x + 1)(2)")->equals(call_2_on_fun_expr_x_add_x_1));

    REQUIRE(parse_expression_str("_let f = _fun (x)\n"
                                 "\t       _fun (y) \n"
                                 "\t         x*x + y*y \n"
                                 "_in f(2)(3)")->interp()->equals(NEW (NumVal)(13)));
}

TEST_CASE("quiz function vals") {
    // 1
    REQUIRE(parse_expression_str("_let f = _fun (x) x+ 1 \n"
                                 "_in f(5) ")->interp()->equals(NEW (NumVal)(6)));

    // 2
    REQUIRE(parse_expression_str("_let f = _fun (x)\n"
                                 "           7\n"
                                 "_in f(5)")->interp()->equals(NEW (NumVal)(7)));

    // 3
    REQUIRE(parse_expression_str("_let f = _fun (x)\n"
                                 "           _true\n"
                                 "_in f(5) ")->interp()->equals(NEW (BoolVal)(true)));

    // 4
    REQUIRE_THROWS_WITH(parse_expression_str("_let f = _fun (x)\n"
                                             "           x + _true\n"
                                             "_in f(5) ")->interp(), "add of non-number");

    // 5
    REQUIRE(parse_expression_str("_let f = _fun (x)\n"
                                 "           x + _true\n"
                                 "_in 5 + 1 ")->interp()->equals(NEW (NumVal)(6)));

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
                                 "    _else f(6)")->interp()->equals(NEW (NumVal)(7)));

    // 9
    REQUIRE(parse_expression_str("_let f = _fun (x) x+ 1 \n"
                                 "_in _let g = _fun (y) y+ 2 \n"
                                 "_in _if _true\n"
                                 "    _then f(5)\n"
                                 "    _else g(5)")->interp()->equals(NEW (NumVal)(6)));

    // 10
    REQUIRE(parse_expression_str("_let f = _fun (x) x+ 1 \n"
                                 "_in _let g = _fun (y) y+ 2 \n"
                                 "_in f(g(5)) ")->interp()->equals(NEW (NumVal)(8)));

    // 11
    REQUIRE(parse_expression_str("_let f = _fun (x) x+ 1 \n"
                                 "_in _let g = _fun (y)\n"
                                 "              f(y + 2)\n"
                                 "_in g(5) ")->interp()->equals(NEW (NumVal)(8)));

    // 12
    REQUIRE(parse_expression_str("_let f = _fun (x) x+ 1 \n"
                                 "_in _let g = _fun (x)\n"
                                 "              f(2) + x\n"
                                 "_in g(5) ")->interp()->equals(NEW (NumVal)(8)));

    // 13
    REQUIRE_THROWS_WITH(parse_expression_str("_let f = _fun (x) x+ 1 \n"
                                             "_in f 5 ")->interp(), "invalid input");

    // 14
    REQUIRE(parse_expression_str("_let f = _fun (x) x+ 1 \n"
                                 "_in (f)(5) ")->interp()->equals(NEW (NumVal)(6)));



    // 15
    auto add_x_1 = NEW (Add)(NEW (Var)("x"), NEW (NumExpr)(1));
    auto fun_val_x_add_x_1 = NEW (FunVal)("x", add_x_1);
    REQUIRE(parse_expression_str("_fun (x) x+ 1 ")->interp()->equals(fun_val_x_add_x_1));

    //16
    REQUIRE(parse_expression_str("_let f = _fun (x) x+ 1 \n"
                                 "_in f ")->interp()->equals(fun_val_x_add_x_1));

    // 17
    REQUIRE(parse_expression_str("(_fun (x)\n"
                                 "   x + 1)(5)")->interp()->equals(NEW (NumVal)(6)));

    // 18
    REQUIRE(parse_expression_str("_let f = _if _false\n"
                                 "            _then _fun (x)  \n"
                                 "                        x+ 1 \n"
                                 "           _else _fun (x)\n"
                                 "                       x+ 2\n"
                                 "_in f(5)")->interp()->equals(NEW (NumVal)(7)));

    // 19
    REQUIRE(parse_expression_str("(_if _false \n"
                                 "  _then _fun (x)\n"
                                 "            x+ 1\n"
                                 "   _else _fun (x)\n"
                                 "                x + 2)(5)")->interp()->equals(NEW (NumVal)(7)));


    // 20
    REQUIRE(parse_expression_str("_let f = _fun (g)\n"
                                 "           g(5)\n"
                                 "_in _let g = _fun (y)  \n"
                                 "             y + 2\n"
                                 "_in f(g) ")->interp()->equals(NEW (NumVal)(7)));

    // 21
    REQUIRE(parse_expression_str("_let f = _fun (g)\n"
                                 "           g(5)\n"
                                 "_in f(_fun (y)\n"
                                 "        y + 2)")->interp()->equals(NEW (NumVal)(7)));

    // 22
    REQUIRE(parse_expression_str("_let f = _fun (x)\n"
                                 "           _fun (y)\n"
                                 "x+ y _in (f(5))(1) ")->interp()->equals(NEW (NumVal)(6)));

    // 23
    REQUIRE(parse_expression_str("_let f = _fun (x)\n"
                                 "           _fun (y)\n"
                                 "x+ y _in f(5)(1) ")->interp()->equals(NEW (NumVal)(6)));

    // 24
    REQUIRE(parse_expression_str("_let f = _fun (x)\n"
                                 "           _fun (g)\n"
                                 "             g(x + 1)\n"
                                 "_in _let g = _fun (y)\n"
                                 "              y+ 2 \n"
                                 "_in (f(5))(g) ")->interp()->equals(NEW (NumVal)(8)));

    // 25
    REQUIRE(parse_expression_str("_let f = _fun (x)\n"
                                 "           _fun (g)\n"
                                 "             g(x + 1)\n"
                                 "_in _let g = _fun (y)\n"
                                 "y+ 2 _in f(5)(g)")->interp()->equals(NEW (NumVal)(8)));

    // 26
    REQUIRE(parse_expression_str("_let f = _fun (f)\n"
                                 "           _fun (x)\n"
                                 "             _if x == 0\n"
                                 "             _then 0\n"
                                 "             _else x + f(f)(x + -1)\n"
                                 "_in f(f)(3)")->interp()->equals(NEW (NumVal)(6)));




    // Q 26 example
    auto x = NEW (Var)("x");
    auto f = NEW (Var)("f");
    auto q26_expr_rhs = NEW (FunExpr)("f", NEW (FunExpr)("x", NEW (IfExpr)(NEW (EqExpr)(x, NEW (NumExpr)(0)),
                                                                         NEW (NumExpr)(0),
                                                                         NEW (Add)(x, NEW (CallExpr)(NEW (CallExpr)(f, f),
                                                                                                  NEW (Add)(x,
                                                                                                              NEW (NumExpr)(
                                                                                                                      -1))))
                                                      )
    ));
    auto q26_expr = NEW (Let)("f", q26_expr_rhs, NEW (CallExpr)(NEW (CallExpr)(f, f), NEW (NumExpr)(3)));

    REQUIRE(q26_expr->to_pretty_string() == "_let f = _fun (f)\n"
                                            "           _fun (x)\n"
                                            "             _if x == 0\n"
                                            "             _then 0\n"
                                            "             _else x + f(f)(x + -1)\n"
                                            "_in  f(f)(3)");

    REQUIRE(q26_expr->interp()->equals(NEW (NumVal)(6)));
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("Fun Val") {
    auto add_x_1 = NEW (Add)(NEW (Var)("x"), NEW (NumExpr)(1));
    auto fun_var_x_add_x_1 = NEW (FunVal)("x", add_x_1);
    auto fun_var_x_add_x_1_copy = NEW (FunVal)("x", add_x_1);

    auto num_val_2 = NEW (NumVal)(2);

    // (_fun (y) (_fun (x) y + x))
    auto add_y_x = NEW (Add)(NEW (Var)("y"), NEW (Var)("x"));
    auto fun_expr_x_add_y_x = NEW (FunExpr)("x", add_y_x);
    auto fun_expr_y_fun_var_x_add_y_x = NEW (FunExpr)("y", fun_expr_x_add_y_x);
    auto fun_var_y_fun_var_x_add_y_x = NEW (FunVal)("y", fun_expr_x_add_y_x);

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

    auto fun_expr_x_add_x_1 = NEW (FunExpr)("x", add_x_1);

    SECTION("is_true") {
        REQUIRE_THROWS_WITH(fun_var_x_add_x_1->is_true(), "a fun val cannot be interpreted as a bool val");
        REQUIRE_THROWS_WITH(fun_var_y_fun_var_x_add_y_x->is_true(), "a fun val cannot be interpreted as a bool val");
    }

    auto add_2_x = NEW (Add)(NEW (NumExpr)(2), NEW (Var)("x"));
    auto fun_val_x_add_2_x = NEW (FunVal)("x", add_2_x);
    auto fun_var_y_fun_var_x_add_y_x_call_2 = fun_var_y_fun_var_x_add_y_x->call(NEW (NumVal)(2));

    SECTION("call") {
        REQUIRE(fun_var_x_add_x_1->call(NEW (NumVal)(3))->equals(NEW (NumVal)(4)));
        REQUIRE_THROWS_WITH(fun_var_x_add_x_1->call(NEW (BoolVal)(3)), "Addition to a boolean is not possible");
        REQUIRE_THROWS_WITH(fun_var_x_add_x_1->call(fun_var_x_add_x_1), "Addition to a FunVal is not possible");
        REQUIRE(fun_var_y_fun_var_x_add_y_x_call_2->call(NEW (NumVal)(1))->equals(NEW (NumVal)(3)));
    }
}
