//
//  parse.cpp
//  MSDScript
//
//  Created by Lakshay Santosh Kucheriya on 2/20/23.
//

#include "parse.hpp"

/**
 Return an expresion according to grammar:
 <expr> =  <addend>
         | <addend> +  <expr>
 */
Expr *parse_expression(std::istream &in)
{
    Expr *addend = parse_addend(in) ;
    skip_whitespace(in) ;
    int ch = in.peek();
    
    if (ch == '+')   // test
    {
        consume(in, '+');
        Expr *second_expr = parse_expression(in);
        addend = new Add(addend, second_expr);
        skip_whitespace(in);
        ch = in.peek();
    }
    
    if (ch != EOF && ch != ')' && ch != '_' && ch != '\n')
    {
        throw std::runtime_error("invalid input");
    }
    
    return addend;
}


Expr *parse_expression_str(std::string str)
{
    std::stringstream ss(str);
    return parse_expression(ss);
}

Expr *parse_num(std::istream &in)
{
    int num = 0;
    bool is_negative = false;
    if (in.peek() == '-')
    {
        is_negative = true;
        consume(in, '-');
        if (!isdigit(in.peek()))
        {
            throw std::runtime_error("number should come right after -");
        }
    }
    int ch;

    while ((ch = in.peek()) && isdigit(ch))
    {
        consume(in, ch);
        num = num * 10 + (ch - '0');
    }

    if (is_negative)
    {
        num *= -1;
    }
    return new Num(num);
}


/**
 Return an expresion according to grammar:
 <addend> = <multicand>
          | <multicand> * <addend>
 */
Expr *parse_addend(std::istream &in)
{
    Expr *multicand = parse_multicand(in) ;
    skip_whitespace(in);
    int ch = in.peek() ;
    if(ch != '*')
    {
        return multicand;
    }
    consume(in, '*');
    skip_whitespace(in) ;
    Expr *addend = parse_addend(in);
    return new Mult(multicand,addend);
}


/**
 Return an expresion according to grammar:
 <multicand> =      <number>
               |  <expr>
               | <variable>
               |  _let  <variable> = <expr> _in <expr>
 */
Expr *parse_multicand(std::istream &in)
{
    skip_whitespace(in);
    int ch = in.peek();
    if ((ch == '-') || isdigit(ch))
    {
        return parse_num(in);
    }
    if (ch == '(') {
        consume(in, '(');
        Expr *sub_expression = parse_expression(in);
        skip_whitespace(in);
        
        if (in.peek() != ')')
        {
            throw std::runtime_error("missing close parenthesis");
        }
        else
        {
            consume(in, ')') ;
        }
        return sub_expression;
    }
    
    if (isalpha(ch))
    {
        Expr *variable = parse_variable(in);
        skip_whitespace(in);
        return variable;
    }

    if (ch == '_')
    {
        return parse_let(in);
    }
    
    consume(in, ch);
    throw std::runtime_error("invalid input");
}
    

Expr *parse_variable(std::istream &in)
{
    int ch;
    std::string str ;
    
    while ((ch = in.peek()) && isalpha(ch))
    {
        consume(in, ch) ;
        str += (char) ch ;
    }
    ch = in.peek() ;
    
    if (ch != ' ' && !(ch == '+' || ch == '*' || ch == ')' || in.eof())) // eof returns -1 in C++
    {
        throw std::runtime_error("unexpected character in variable");
    }
    return new Var(str);
}

Expr *parse_let(std::istream &in)
{
    parse_keyword(in, "_let");
    skip_whitespace(in) ;
    
    Expr *lhs = parse_variable(in) ;
    skip_whitespace(in) ;
    parse_keyword(in, "=") ;
    skip_whitespace(in);
    
    Expr *rhs = parse_expression(in) ;
    skip_whitespace(in) ;
    parse_keyword(in, "_in");
    skip_whitespace(in) ;
    
    Expr *body = parse_expression(in) ;
    skip_whitespace(in) ;
    return new Let(lhs->to_string(), rhs, body) ;
}

void parse_keyword(std::istream &in, std::string expectation)
{
    for (char ch: expectation)
    {
        if (in.peek() != ch)
        {
            throw std::runtime_error("invalid input");
        }
        consume(in, ch);
    }
}

void consume(std::istream &in, int expectation)
{
    int ch = in.get();
    if (ch != expectation) {
        throw std::runtime_error("consume mismatch");
    }
}

void skip_whitespace(std::istream &in)
{
    int ch ;
    while ((ch = in.peek()) && isspace(ch))
    {
        consume(in, ch);
    }
}