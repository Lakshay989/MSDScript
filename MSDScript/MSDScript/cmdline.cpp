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


#include "cmdline.hpp"


void if_interp()
{
    //cout << " Handling interp" << endl ;
    Expr* expr = parse_input();
    //cout << "Expression entered: " << expr->to_pretty_string() << endl ;
    //cout << "interp value: " ;
    cout << expr->interp()->to_string() << endl; // ->to_string()
}

void if_print()
{
    //cout << "Handling print" << endl ;
    Expr* expr = parse_input();
    //cout << "Printed expression: ";
    cout<< expr->to_string() << endl ;
}

void if_pretty_print()
{
    //cout << "Handling pretty print" << endl ;
    Expr* expr = parse_input();
    //cout << "Printed expression: " ;
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

Expr* parse_input()
{
    std::string input ;
    std::getline(std::cin, input);
    std::stringstream ss(input);
    return parse_expression(ss);
}
//    Expr *expr;
//        try {
//            expr = parse_expression(std::cin);
//        } catch (std::runtime_error &err) {
//            std::cerr << "Runtime error: " << err.what();
//            std::exit(1);
//        }
//        return expr;



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
