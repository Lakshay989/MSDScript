//
//  parse.hpp
//  MSDScript
//
//  Created by Lakshay Santosh Kucheriya on 2/20/23.
//

#ifndef parse_hpp
#define parse_hpp

#include "expr.hpp"
#include <stdio.h>
#include <iostream>


PTR(Expr) parse_expression(std::istream &in) ;

PTR(Expr) parse_expression_str(std::string str);

PTR(Expr) parse_num(std::istream &in);

PTR(Expr) parse_addend(std::istream &in);

PTR(Expr) parse_multicand(std::istream &in);

PTR(Expr) parse_variable(std::istream &in);

PTR(Expr) parse_let(std::istream &in);

PTR(Expr) parse_comprag(std::istream &in);

PTR(Expr) parse_if_expr(std::istream &in);

PTR(Expr) parse_inner_expression(std::istream &in);

PTR(Expr) parse_fun_expr(std::istream &in);

void parse_keyword(std::istream &in, std::string expectation);

void consume(std::istream &in, int expectation);

void skip_whitespace(std::istream &in);

std::string parse_and_locate_next_keyword(std::istream &in);

#endif /* parse_hpp */
