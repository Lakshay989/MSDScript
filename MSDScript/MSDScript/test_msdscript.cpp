#include <iostream>
#include <random>
#include "exec.h"

#define SINGLE_TEST_COUNT 200

using SINGLE_TEST_ARGVS = const char *const;

int generate_random_int(int min = 0, int max = 9) {
    // If no explicit seed, C++ takes the current CPU time as the default seed
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
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
    int randNum = rand() % 10;
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

//std::string random_invalid_expression() {
//    int possibility = generate_random_int();
//    if (possibility < 2) {
//        // missing opening parenthesis
//        return random_valid_expr_string() + " )";
//    } else if (possibility < 4) {
//        // missing closing parenthesis
//        return "( " + random_valid_expr_string();
//    } else if (possibility < 6) {
//        // missing operator between two expressions
//        return random_valid_expr_string() + " " + random_valid_expr_string();
//    } else if (possibility < 8) {
//        // single minus operator
//        return "-";
//    } else {
//        // single minus operator should only be the prefix of a number
//        return "- " + random_valid_expr_string();
//    }
//}

//std::string random_expression_string() {
//    int possibility = generate_random_int();
//    if (possibility < 5) {
//        // valid expression
//        return random_valid_expr_string();
//    } else {
//        // invalid expression
//        return random_invalid_expression();
//    }
//}

//bool are_both_test_failed(ExecResult r1, ExecResult r2) {
//    return r1.exit_code != 0 && r2.exit_code;
//}
//
//void run_single_test(SINGLE_TEST_ARGVS argv_1[], SINGLE_TEST_ARGVS argv_2[], std::string argv_1_name,
//                     std::string argv_2_name) {
//    for (int i = 0; i < SINGLE_TEST_COUNT; i++) {
//        std::string in = random_expr_string();
//
//        ExecResult result_1 = exec_program(2, argv_1, in);
//        ExecResult result_2 = exec_program(2, argv_2, in);
//
//        bool both_test_failed = are_both_test_failed(result_1, result_2);
//
//        if (!both_test_failed && (result_1.out != result_2.out)) {
//            std::cerr << "\n\nTrying " << in << "\n";
//            std::cerr << argv_1_name << ".exit_code: " << result_1.exit_code << '\n';
//            std::cerr << argv_1_name << ".out: \n" << result_1.out << '\n';
//            if (result_1.exit_code != 0) {
//                std::cerr << argv_1_name << ".error: " << result_1.err << "\n";
//            }
//            std::cerr << argv_2_name << ".exit_code: " << result_2.exit_code << '\n';
//            std::cerr << argv_2_name << ".out: \n" << result_2.out << '\n';
//            if (result_2.exit_code != 0) {
//                std::cerr << argv_2_name << ".error: " << result_2.err << "\n";
//            }
//            std::cerr << "different results \n\n";
//        }
//    }
//}

int main(int argc, char** argv) {
    srand(time(NULL));
    if (argc == 2 && std::strcmp(argv[0], "./test_msdscript") != 0) {
        char* scriptName = argv[1];
        const char* const interp_argv[] = { scriptName, "--interp" };
        const char* const print_argv[] = { scriptName, "--print" };
        const char* const prettyPrint_argv[] = { scriptName, "--pretty-print" };
        
        for (int i = 0; i < 100; i++) {
            std::string in = random_expr_string();
            std::cout << "Testing: " << in << "\n";
            
            ExecResult interp_result = exec_program(2, interp_argv, in);
            ExecResult print_result = exec_program(2, print_argv, in);
            ExecResult prettyPrint_result = exec_program(2, prettyPrint_argv, in);
            
            std::cout << "Interp: " << interp_result.out << "\n";
            std::cout << "Print: " << print_result.out << "\n";
            std::cout << "Pretty Print: " << prettyPrint_result.out << "\n";
            
            ExecResult interp_result_verify = exec_program(2, interp_argv, print_result.out);
            
            if (interp_result.out != interp_result_verify.out) {
                throw std::runtime_error("results does not match!");
            }
        }
        std::cout << "Result: Test successful!" << std::endl;
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

