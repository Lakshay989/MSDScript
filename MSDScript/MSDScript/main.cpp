//
//  main.cpp
//  MSDScript
//
//  Created by Lakshay Santosh Kucheriya on 1/16/23.
//

/**
* \file main.cpp
* \brief Entry point for the project
*
* \author Lakshay Santosh Kucheriya
*/

/**
* \mainpage MSDScript
* \author Lakshay Santosh Kucheriya
* \date Feburary 7, 2023
*/


#include <iostream>
#include "cmdline.hpp"

int main(int argc, char  **argv) {
    
    run_mode_t run_mode = use_arguments(argc, argv);
    
    switch (run_mode) {
        case do_nothing:
            break;
        case do_interp:
            if_interp();
            break;
        case do_print:
            if_print();
            break;
        case do_pretty_print:
            if_pretty_print();
            break;
        case do_test:
            if_test(argv);
            break;
        case do_help:
            if_help();
            break;
        case do_error_handling:
            if_throw_error(argv);
            break;
    }
    
    return 0;
}
