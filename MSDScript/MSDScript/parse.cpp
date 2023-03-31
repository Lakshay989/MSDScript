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
PTR(Expr) parse_expression(std::istream &in)
{
    PTR(Expr) comprag = parse_comprag(in) ;
    skip_whitespace(in);
        int ch = in.peek();
        if (ch == '=') {
            parse_keyword(in, "==");
            PTR(Expr) second_expr = parse_expression(in);
            comprag = NEW EqExpr(comprag, second_expr);
            skip_whitespace(in);
            ch = in.peek();
        }
    if (ch != EOF && ch != ')' && ch != '_' && ch != '\n')
    {
        throw std::runtime_error("invalid input");
    }
    
    return comprag;
}


PTR(Expr) parse_expression_str(std::string str)
{
    std::stringstream ss(str);
    return parse_expression(ss);
}

PTR(Expr) parse_comprag(std::istream &in)
{
    PTR(Expr) addend = parse_addend(in);
    skip_whitespace(in) ;
    char ch = in.peek();
    if(ch == '+'){
        consume(in, '+');
        PTR(Expr) second_expr = parse_comprag(in);
        addend = NEW Add(addend, second_expr);
        skip_whitespace(in);
    }
    return addend;
}


PTR(Expr) parse_num(std::istream &in)
{
    unsigned int num = 0;
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
    return NEW (NumExpr)((int) num);
}


/**
 Return an expresion according to grammar:
 <addend> = <multicand>
          | <multicand> * <addend>
 */
PTR(Expr) parse_addend(std::istream &in)
{
    PTR(Expr) multicand = parse_multicand(in) ;
    skip_whitespace(in);
    int ch = in.peek() ;
    if(ch != '*')
    {
        return multicand;
    }
    consume(in, '*');
    skip_whitespace(in) ;
    PTR(Expr) addend = parse_addend(in);
    return NEW Mult(multicand,addend);
}


/**
 Return an expresion according to grammar:
 <multicand> =      <number>
               |  <expr>
               | <variable>
               |  _let  <variable> = <expr> _in <expr>
 */
PTR(Expr) parse_multicand(std::istream &in)
{
    skip_whitespace(in);

    PTR(Expr) expr = parse_inner_expression(in);
    
    //std::cout << "inner: " << expr->to_string() << "\n";
    skip_whitespace(in);
    while (!in.eof() && in.peek() == '(')
    {
        consume(in, '(');
        skip_whitespace(in);
        PTR(Expr) actual_arg = parse_expression(in);
        
        skip_whitespace(in);
        consume(in, ')');
        expr = NEW CallExpr(expr, actual_arg);
    }
        return expr;

}
    

PTR(Expr) parse_variable(std::istream &in)
{
    int ch;
    std::string str ;
    
    while ((ch = in.peek()) && isalpha(ch))
    {
        consume(in, ch) ;
        str += (char) ch ;
    }
    ch = in.peek() ;
    
    if (!isspace(ch) && !(ch == '+' || ch == '*' || ch == ')' || ch =='(' || ch == '=' || in.eof())) // eof returns -1 in C++
    {
        throw std::runtime_error("unexpected character in variable");
    }
    return NEW Var(str);
}

PTR(Expr) parse_let(std::istream &in)
{
    //parse_keyword(in, "_let");
    
    skip_whitespace(in) ;
    PTR(Expr) lhs = parse_variable(in) ;
    skip_whitespace(in) ;
    parse_keyword(in, "=") ;
    
    skip_whitespace(in);
    PTR(Expr) rhs = parse_comprag(in) ;
    skip_whitespace(in) ;
    parse_keyword(in, "_in");
    
    skip_whitespace(in) ;
    PTR(Expr) body = parse_comprag(in) ;
    skip_whitespace(in) ;
    
    return NEW Let(lhs->to_string(), rhs, body) ;
}

PTR(Expr) parse_if_expr(std::istream &in)
{
    skip_whitespace(in);
    PTR(Expr) condition = parse_expression(in);
    skip_whitespace(in);
    parse_keyword(in, "_then");
    
    skip_whitespace(in);
    PTR(Expr) then_expr = parse_expression(in);
    skip_whitespace(in);
    parse_keyword(in, "_else");
    
    skip_whitespace(in);
    PTR(Expr) else_expr = parse_expression(in);
    skip_whitespace(in);
    
    return NEW IfExpr(condition, then_expr, else_expr);
}

PTR(Expr) parse_inner_expression(std::istream &in)
{
    skip_whitespace(in) ;
    int ch = in.peek();
    if ((ch == '-') || isdigit(ch))
    {
        return parse_num(in);
    }
    if (ch == '(')
    {
        consume(in, '(');
        PTR(Expr) sub_expression = parse_expression(in);
        skip_whitespace(in);
        
        if (in.peek() != ')')
        {
            if (in.eof()) {
                throw std::runtime_error("missing close parenthesis");
            }
        }
        else
        {
            consume(in, ')') ;
        }
        return sub_expression;
    }
    
    if (isalpha(ch))
    {
        PTR(Expr) variable = parse_variable(in);
        skip_whitespace(in);
        return variable;
    }
    
    if (ch == '_')
    {
        std::string next_keyword = parse_and_locate_next_keyword(in);
        if (next_keyword == "_let")
        {
            return parse_let(in);
        }
        
        else if (next_keyword == "_false")
        {
            return NEW BoolExpr(false);
        }
        
        else if (next_keyword == "_true")
        {
            return NEW BoolExpr(true);
        }
        
        else if (next_keyword == "_if")
        {
            return parse_if_expr(in);
        }
        else if(next_keyword == "_fun")
        {
            return parse_fun_expr(in);
        }
    }
    
    consume(in, ch);
    throw std::runtime_error("invalid input");
}

PTR(Expr) parse_fun_expr(std::istream &in)
{
    skip_whitespace(in);
    consume(in, '(');
    skip_whitespace(in);
    PTR(Expr) variable = parse_variable(in);
    
    skip_whitespace(in);
    consume(in, ')');
    PTR(Expr) body = parse_expression(in);
    
    return NEW FunExpr(variable->to_string(), body);
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

std::string parse_and_locate_next_keyword(std::istream &in) {
    if (in.peek() != '_' || in.eof()) {
        throw std::runtime_error("not a keyword");
    }
    consume(in, '_');
    std::string keyword = "_";
    while (isalpha(in.peek()))
    {
        keyword += (char) in.peek();
        consume(in, in.peek());
    }
    return keyword;
}

