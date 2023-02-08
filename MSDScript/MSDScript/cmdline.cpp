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

using namespace std ;


/* use_arguments( int argc , char* argv[]) identifies the argument given ahead of running the executable
    on the command line is valid and if it is valid executes the command.
*/

/**
* \brief Identifies the argument given ahead of running the executable
* \param argc first parameter, stores the number of command line arguments passed by the user
* \param argv second parameter, is array of character pointers listing all the arguments.
*/
void use_arguments(int argc , char* argv[])
{
    
    if(argc == 1)
    {
        return;
    }
    
    bool test_occured = false ;
    
    
    for(int i = 1 ; i< argc ; i++)
    {
        if(strcmp(argv[i], "--help") == 0)
        {
            cout<<"The command line arguments allowed are : \n --help \n --test \n";
            exit(0);
        }
        
        else if(test_occured == true)
        {
            cerr<<"Tests already performed \n";
            exit(1) ;
        }
        else if(strcmp(argv[i],"--test") == 0)
        {
            if(Catch::Session().run(1, argv) != 0)
                exit(1);
            cout<<"Tests passed \n";
            test_occured = true;
        }
        else
        {
            cerr<<"No such argument exists. Kindly use --help for a list of accepted arguments \n";
            exit(1) ;
        }
    }
}

