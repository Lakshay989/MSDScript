//
//  main.cpp
//  MSDScript
//
//  Created by Lakshay Santosh Kucheriya on 1/16/23.
//
#define CATCH_CONFIG_RUNNER

#include "catch.h"

#include <iostream>
#include "cmdline.hpp"

int main(int argc, char  **argv) {
    // insert code here...
    use_arguments(argc, argv);
    Catch::Session().run(1, argv);
    return 0;
}
