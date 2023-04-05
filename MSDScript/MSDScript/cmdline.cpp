//
//  cmdline.cpp
//  MSDScript
//
//  Created by Lakshay Santosh Kucheriya on 1/16/23.
//

/**
* \file cmdline.cpp
* \brief Contains the code for  identifiying the argument given ahead of running the executable on the command line is valid and if it is valid executes the command.
*
* \author Lakshay Santosh Kucheriya
*/

#define CATCH_CONFIG_RUNNER

#include "expr.hpp"
#include "val.hpp"
#include "catch.h"
#include "parse.hpp"
#include "cmdline.hpp"


void if_interp()
{
    auto expr = parse_input();
    cout << expr->interp()->to_string() << endl; // ->to_string()
}

void if_print()
{
    auto expr = parse_input();
    cout<< expr->to_string() << endl ;
}

void if_pretty_print()
{
    auto expr = parse_input();
    cout<< expr->to_pretty_string() << endl ;
}

void if_test(char* argv[])
{
    if(Catch::Session().run(1, argv) != 0)
        exit(1);
    cout<<"Tests passed \n";
}

void if_throw_error(char* argv[])
{
    cerr<<"No such argument exists. Kindly use --help for a list of accepted arguments \n";
    exit(0) ;
}

void if_help()
{
    cout<<"The command line arguments allowed are : \n --help \n --test \n --interp \n --print \n --pretty-print \n ";
    exit(1);
}

PTR(Expr) parse_input()
{
    std::string input ;
    std::getline(std::cin, input);
    std::stringstream ss(input);
    return parse_expression(ss);
}

/**
* \brief Identifies the argument given ahead of running the executable
* \param argc first parameter, stores the number of command line arguments passed by the user
* \param argv second parameter, is array of character pointers listing all the arguments.
*/
run_mode_t use_arguments(int argc , char* argv[])
{
    if(argc == 1)
    {
        return do_nothing;
    }
    
    std::string argument = argv[1] ;
    //bool test_occured = false ;

    if(argument == "--help")
    {
        return do_help;
    }
    else if(argument == "--test")
    {
        return do_test;
    }
    else if(argument == "--interp")
    {
        return do_interp;
    }
    else if(argument == "--print")
    {
        return do_print;
    }
    else if(argument == "--pretty-print")
    {
        return do_pretty_print;
    }
    else
    {
        return do_error_handling;
    }
}
