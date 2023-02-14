//
//  expr.cpp
//  MSDScript
//
//  Created by Lakshay Santosh Kucheriya on 1/23/23.
//

/**
* \file expr.cpp
* \brief Contains expression class definition
*
* \author Lakshay Santosh Kucheriya
*/

#include "expr.hpp"

/**
* \brief Constructor
*/
Num::Num(int val)
{
    this->val = val;
}


/**
* \brief Checks for the equality between the left hand side and the right hand side
* \param e Expression
* \return boolean value of LHS = RHS
*/
bool Num::equals(Expr *e)
{
    Num *n = dynamic_cast<Num*>(e);
    if (n == NULL)
        return false;
    else
        return val == n->val;
}


/**
* \brief This function interprets the value of the integer
* \return integer value of the number
*/
int Num::interp()
{
    return int(val);
}


/**
* \brief This function determines if the the expression consists of a variable or not.
* \return boolean value for the number has a variable or not
*/
bool Num::has_variable()
{
    return false;
}


/**
* \brief This function substitutes the expression with the combination of sub-expressions if possible.
* \param s first argument, string which can be replaced as a part of the expression
* \param e second argument, Expression
* \return Expression which is modified if combination of sub expressions was possible
*/
Expr* Num::subst(std::string s, Expr* e)
{
    return this ;
}

/**
* \brief Prints the expression
* \param [out] out output stream
*/
void Num::print(std::ostream &out) {
    out << std::to_string(val);
}

/**
* \brief Prints the expression with more clarity
* \param [out] out output stream
* \param position second argument, position to track the alignment
*/
void Num::pretty_print(std::ostream &out, int position) {
    this->pretty_print_at(out, precedence_none, false);
}

void Num::pretty_print_at(std::ostream &out, precedence_t precedence, bool paranthesis, int position) {
    out << std::to_string(val);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/**
* \brief Constructor
*/
Add::Add(Expr *lhs, Expr *rhs)
{
    this->lhs = lhs;
    this->rhs = rhs;
}
    

/**
* \brief Checks for the equality between the left hand side and the right hand side
* \param e Expression
* \return boolean value of LHS = RHS
*/
bool Add::equals(Expr *e) // Add Equals
{
    Add *a = dynamic_cast<Add*>(e);
    if (a == NULL)
        return false;
    else
        return (lhs->equals(a->lhs)
                && rhs->equals(a->rhs));
}


/**
* \brief This function interprets the value of the expression
* \return integer value of the computed expression
*/
int Add::interp()
{
    return (int)lhs->interp() + (int)rhs->interp() ;
}


/**
* \brief This function determines if the the expression consists of a variable or not.
* \return boolean value for the expression has a variable or not
*/
bool Add::has_variable()
{
    return (lhs->has_variable() || rhs->has_variable()) ;
}


/**
* \brief This function substitutes the expression with the combination of sub-expressions if possible.
* \param s first argument, string which can be replaced as a part of the expression
* \param e second argument, Expression
* \return Expression which is modified if combination of sub expressions was possible
*/
Expr* Add::subst(std::string s, Expr* e)
{
    return new Add(lhs->subst(s, e), rhs->subst(s, e));
}


/**
* \brief Prints the expression
* \param [out] out output stream
*/
void Add::print(std::ostream &out) {
    out << "(";
    this->lhs->print(out);
    out << "+";
    this->rhs->print(out);
    out << ")";
}


/**
* \brief Prints the expression with more clarity
* \param [out] out output stream
* \param position second argument, position to track the alignment
*/
void Add::pretty_print(std::ostream &out, int position) {
    this->lhs->pretty_print_at(out, precedence_add, true, position);
    out << " + ";
    this->rhs->pretty_print_at(out, precedence_none, false, position);
}

void Add::pretty_print_at(std::ostream &out, precedence_t precedence, bool paranthesis, int position) {
    if (precedence >= precedence_add) {
        out << "(";
    }
    out << this->lhs->to_pretty_string(position) << " + " << this->rhs->to_pretty_string(position);
    if (precedence >= precedence_add) {
        out << ")";
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* \brief Constructor
*/
Mult::Mult(Expr *lhs, Expr *rhs)
{
    this->lhs = lhs;
    this->rhs = rhs;
}


/**
* \brief Checks for the equality between the left hand side and the right hand side
* \param e Expression
* \return boolean value of LHS = RHS
*/
bool Mult::equals(Expr* e) { // Mult equals
  Mult* m = dynamic_cast<Mult*>(e);
  if (m == NULL)
    return false;
  else
    return (lhs->equals(m->lhs)
            && rhs->equals(m->rhs));
}


/**
* \brief This function interprets the value of the expression
* \return integer value of the computed expression
*/
int Mult::interp()
{
    return (int)lhs->interp() * (int)rhs->interp();
}


/**
* \brief This function determines if the the expression consists of a variable or not.
* \return boolean value for the expression has a variable or not
*/
bool Mult::has_variable()
{
    return (lhs->has_variable() || rhs->has_variable());
}


/**
* \brief This function substitutes the expression with the combination of sub-expressions if possible.
* \param s first argument, string which can be replaced as a part of the expression
* \param e second argument, Expressiont
* \return Expression which is modified if combination of sub expressions was possible
*/
Expr* Mult::subst(std::string s, Expr* e)
{
    return new Mult(lhs->subst(s, e), rhs->subst(s, e));
}


/**
* \brief Prints the expression
* \param [out] out output stream
*/
void Mult::print(std::ostream &out) {
    out << "(";
    this->lhs->print(out);
    out << "*";
    this->rhs->print(out);
    out << ")";
}


/**
* \brief Prints the expression with more clarity
* \param [out] out output stream
* \param position second argument, position to track the alignment
*/
void Mult::pretty_print(std::ostream &out, int position) {
    this->lhs->pretty_print_at(out, precedence_mult, true, position);
    out << " * ";
    this->rhs->pretty_print_at(out, precedence_add, false, position);
}

void Mult::pretty_print_at(std::ostream &out, precedence_t precedence, bool paranthesis, int position) {
    if (precedence >= precedence_mult) {
        out << "(";
    }
    out << this->lhs->to_pretty_string(position) << " * " << this->rhs->to_pretty_string(position);
    if (precedence >= precedence_mult) {
        out << ")";
    }
}
     
     
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* \brief Constructor
*/
Var::Var(std::string name)
{
    this->name = name ;
}


/**
* \brief Checks for the equality between the left hand side and the right hand side
* \param e Expression
* \return boolean value of LHS = RHS
*/
bool Var::equals(Expr *e)  // Var equals
{
    Var* v = dynamic_cast<Var*>(e);
    if (v == nullptr)
        return false;
    else
        return name == v->name;
}


/**
* \brief This function interprets the value of the Variable
* \return error message as value of the variable cannot be iterpreted
*/
int Var::interp()
{
    throw std::runtime_error("interp does not work with variable expressions !!");
}


/**
* \brief This function determines if the the expression consists of a variable or not.
* \return boolean value for the variable  is a variable or not
*/
bool Var::has_variable()
{
    return true;
}


/**
* \brief This function substitutes the expression with the combination of sub-expressions if possible.
* \param s first argument, string which can be replaced as a part of the expression
* \param e second argument, Expressiont
* \return Expression which is modified if combination of sub expressions was possible
*/
Expr* Var::subst(std::string s, Expr* e)
{
    if(name == s)
    {
        return e ;
    }
    else
    {
        return this ;
    }
}


/**
* \brief Prints the expression
* \param [out] out output stream
*/
void Var::print(std::ostream &out) {
    out << this->name;
}


/**
* \brief Prints the expression with more clarity
* \param [out] out output stream
* \param position second argument, position to track the alignment
*/
void Var::pretty_print(std::ostream &out, int position) {
    this->pretty_print_at(out, precedence_none, false);
}

void Var::pretty_print_at(std::ostream &out, precedence_t precedence, bool paranthesis, int position) {
    out << this->name;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* \brief Constructor
*/
Let::Let(Var *lhs, Expr *rhs, Expr *body)
{
    this->lhs = lhs ;
    this->rhs = rhs ;
    this->body = body ;
}


/**
* \brief Checks for the equality between the left hand side and the right hand side
* \param e Expression
* \return boolean value of LHS = RHS
*/
bool Let::equals( Expr *e)
{
    Let* let = dynamic_cast<Let*>(e);
    
    if(let == nullptr)
    {
        return false;
    }
    else
    {
        return (let->lhs == this->lhs && let->rhs->equals(this->rhs) && let->body->equals(this->body) ) ;
    }
}


/**
* \brief This function interprets the value of the Variable
* \return value of the variable assigned using Let
*/
int Let::interp()
{
    return this->body->subst(this->lhs->to_string(), this->rhs)->interp();;
}


/**
* \brief This function determines if the the expression consists of a variable or not.
* \return boolean value for the expression has a variable or not
*/
bool Let::has_variable()
{
    return (this->rhs->has_variable() || this->body->has_variable()) ;
}


/**
* \brief This function substitutes the expression with the combination of sub-expressions if possible.
* \param subt first argument, string which can be replaced as a part of the expression
* \param exp second argument, Expression
* \return Expression which is modified if combination of sub expressions was possible
*/
Expr* Let::subst(std::string subt, Expr* exp)
{
    if( this->lhs->name == subt )
    {
        return new Let(this->lhs, this->rhs, this->body);
    }
    else
    {
        return new Let(this->lhs, this->rhs, this->body->subst(subt, exp));  // ->subst(subt, exp)
    }
}


/**
* \brief Prints the expression
* \param [out] out output stream
*/
void Let::print(std::ostream &out)
{
    out<< "(_let " << this->lhs->to_string() << "=" << this->rhs->to_string() << " _in " << this->body->to_string() << ")" ;
}


/**
* \brief Prints the expression with more clarity
* \param [out] out first argument, output stream
* \param position second argument, position to track the alignment
*/
void Let::pretty_print(std::ostream &out, int position)
{
    this->pretty_print_at(out, precedence_none, false, position);
}

void Let::pretty_print_at(std::ostream &out, precedence_t precedence, bool parentheses, int position)
{
    if (parentheses) {
            out << "(";
            position += 1;
        }
        out << "_let " << this->lhs->to_pretty_string() << " = " << this->rhs->to_pretty_string() << "\n";
        int letLine_position = out.tellp();

        for (int i = 0; i < position; i++) {
            out << " ";
        }
        out << "_in  ";
        int inLine_position = out.tellp();
        out << this->body->to_pretty_string(inLine_position - letLine_position);
        if (parentheses) {
            out << ")";
        }
}






