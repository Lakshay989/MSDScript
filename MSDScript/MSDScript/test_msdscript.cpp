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

std::string random_valid_expr_string() {
    int possibility = generate_random_int();
    if (possibility < 2) {
        // negative number
        return "-" + std::to_string(generate_random_int());
    } else if (possibility < 4) {
        // positive number
        return std::to_string(generate_random_int());
    } else if (possibility < 6) {
        // let binding
        return random_valid_let_binding();
    } else if (possibility < 8) {
        // mult
        return random_valid_expr_string() + "*" + random_valid_expr_string();
    } else {
        // add
        return random_valid_expr_string() + "+" + random_valid_expr_string();
    }
}

std::string random_invalid_expression() {
    int possibility = generate_random_int();
    if (possibility < 2) {
        // missing opening parenthesis
        return random_valid_expr_string() + " )";
    } else if (possibility < 4) {
        // missing closing parenthesis
        return "( " + random_valid_expr_string();
    } else if (possibility < 6) {
        // missing operator between two expressions
        return random_valid_expr_string() + " " + random_valid_expr_string();
    } else if (possibility < 8) {
        // single minus operator
        return "-";
    } else {
        // single minus operator should only be the prefix of a number
        return "- " + random_valid_expr_string();
    }
}

std::string random_expression_string() {
    int possibility = generate_random_int();
    if (possibility < 5) {
        // valid expression
        return random_valid_expr_string();
    } else {
        // invalid expression
        return random_invalid_expression();
    }
}

bool are_both_test_failed(ExecResult r1, ExecResult r2) {
    return r1.exit_code != 0 && r2.exit_code;
}

void run_single_test(SINGLE_TEST_ARGVS argv_1[], SINGLE_TEST_ARGVS argv_2[], std::string argv_1_name,
                     std::string argv_2_name) {
    for (int i = 0; i < SINGLE_TEST_COUNT; i++) {
        std::string in = random_expression_string();

        ExecResult result_1 = exec_program(2, argv_1, in);
        ExecResult result_2 = exec_program(2, argv_2, in);

        bool both_test_failed = are_both_test_failed(result_1, result_2);

        if (!both_test_failed && (result_1.out != result_2.out)) {
            std::cerr << "\n\nTrying " << in << "\n";
            std::cerr << argv_1_name << ".exit_code: " << result_1.exit_code << '\n';
            std::cerr << argv_1_name << ".out: \n" << result_1.out << '\n';
            if (result_1.exit_code != 0) {
                std::cerr << argv_1_name << ".error: " << result_1.err << "\n";
            }
            std::cerr << argv_2_name << ".exit_code: " << result_2.exit_code << '\n';
            std::cerr << argv_2_name << ".out: \n" << result_2.out << '\n';
            if (result_2.exit_code != 0) {
                std::cerr << argv_2_name << ".error: " << result_2.err << "\n";
            }
            std::cerr << "different results \n\n";
        }
    }
}


int main(int argc, char **argv) {


    if (argc != 4) {
        std::cerr << "You should pass three arguments: script 1, script 2, argument for msdscripts\n"
                     "e.g.: msdscript msdscript3 --interp\n";
        std::exit(EXIT_FAILURE);
    }

    char *script_1 = argv[1];
    char *script_2 = argv[2];
    char *msdscript_argument = argv[3];

    SINGLE_TEST_ARGVS argv_1[] = {script_1, msdscript_argument};
    SINGLE_TEST_ARGVS argv_2[] = {script_2, msdscript_argument};

    run_single_test(argv_1, argv_2, argv_1[0], argv_2[0]);

    return 0;
}
