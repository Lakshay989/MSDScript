#include <iostream>
#include <random>
#include "exec.h"

#define SINGLE_TEST_COUNT 200

using SINGLE_TEST_ARGVS = const char *const;

int generate_random_int()
{
    //int randNum = rand() % 10;
    return rand() % 10;
}

std::string random_valid_let_binding() {
    std::string let_binding = "_let ";
    std::string variable = "x";
    let_binding += variable + " = " + std::to_string(generate_random_int());
    let_binding += " _in " + variable;
    if (generate_random_int() < 5) {
        let_binding += " + ";
    } else {
        let_binding += " * ";
    }
    return "( " + let_binding + std::to_string(generate_random_int()) + " )";
}

std::string random_expr_string(bool var = false) {
    int randNum = generate_random_int();
    if (randNum < 6) {
        return std::to_string(rand());
    }
    else if (randNum < 7) {
        return random_expr_string() + "+" + random_expr_string();
    }
    else if (randNum < 8) {
        return random_expr_string() + "*" + random_expr_string();
    }
    else if (randNum < 9 && var) {
        return "x";
    }
    else {
        return random_valid_let_binding() ;
    }
}

int main(int argc, char** argv) {
    srand(time(NULL));
    if (argc == 2) {
        char* scriptName = argv[1];
        const char* const interp_argv[] = { scriptName, "--interp" };
        const char* const print_argv[] = { scriptName, "--print" };
        const char* const prettyPrint_argv[] = { scriptName, "--pretty-print" };
        
        for (int i = 0; i < 100; i++) {
            std::string in = random_expr_string();
            std::cout << "Testing: " << in << std::endl;
            
            ExecResult interp_result = exec_program(2, interp_argv, in);
            ExecResult print_result = exec_program(2, print_argv, in);
            ExecResult prettyPrint_result = exec_program(2, prettyPrint_argv, in);
            
            std::cout << "Interp: " << interp_result.out;
            std::cout << "Print: " << print_result.out ;
            std::cout << "Pretty Print: " << prettyPrint_result.out << std::endl << std::endl;
            
            ExecResult interp_result_verify = exec_program(2, interp_argv, print_result.out);
            
            if (interp_result.out != interp_result_verify.out) {
                throw std::runtime_error("results does not match!");
            }
        }
        std::cout << "Result: Tests passed ! Verified interp() == print()->interp()" << std::endl;

    }
    if (argc > 2) {
        char* scriptName1 = argv[1];
        char* scriptName2 = argv[2];
        char* mode = argv[3] ;
        
//        const char* const interp_argv1[] = { scriptName1, "--interp" };
//        const char* const interp_argv2[] = { scriptName2, "--interp" };
        
        const char* const solution_argv1[] = { scriptName1, mode };
        const char* const solution_argv2[] = { scriptName2, mode };
        

        for (int i = 0; i < 100; i++) {
            std::string in = random_expr_string();
            
            ExecResult result1 = exec_program(2, solution_argv1, in);
            ExecResult result2 = exec_program(2, solution_argv2, in);

            std::cout << "Testing: " << in<< std::endl ;
            std::cout << mode << "1 :" << result1.out;
            std::cout << mode << "2 :" << result2.out<< std::endl << std::endl ;
        }
    }
    return 0;
}

