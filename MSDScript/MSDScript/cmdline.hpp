//
//  cmdline.hpp
//  MSDScript
//
//  Created by Lakshay Santosh Kucheriya on 1/16/23.
//

/**
* \file cmdline.hpp
* \brief Header file for cmdline.cpp file
*
* \author Lakshay Santosh Kucheriya
*/

#ifndef cmdline_hpp
#define cmdline_hpp
//
#include "catch.h"
#include "expr.hpp"
#include "parse.hpp"
#include "cmdline.hpp"

#include <stdio.h>
#include <iostream>
#include <string>

using namespace std ;


typedef enum {

    do_nothing,
    do_interp,
    do_print,
    do_pretty_print,
    do_test,
    do_error_handling,
    do_help,

} run_mode_t;

void if_interp();
void if_print();
void if_pretty_print();
void if_test(char* argv[]);
void if_throw_error(char* argv[]);
void if_help();

Expr* parse_input();

run_mode_t use_arguments(int argc, char* argv[]) ;

#endif /* cmdline_hpp */
