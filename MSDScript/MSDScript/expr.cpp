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

#include "val.hpp"
#include "expr.hpp"

/**
* \brief Constructor
*/
NumExpr::NumExpr(int val)
{
    this->val = val;
}


/**
* \brief Checks for the equality between the left hand side and the right hand side
* \param e Expression
* \return boolean value of LHS = RHS
*/
bool NumExpr::equals(Expr *e)
{
    NumExpr *n = dynamic_cast<NumExpr*>(e);
    if (n == NULL)
        return false;
    else
        return val == n->val;
}


/**
* \brief This function interprets the value of the integer
* \return integer value of the number
*/
Val* NumExpr::interp()
{
    return new NumVal(this -> val);
}


/**
* \brief This function determines if the the expression consists of a variable or not.
* \return boolean value for the number has a variable or not
*/
bool NumExpr::has_variable()
{
    return false;
}


/**
* \brief This function substitutes the expression with the combination of sub-expressions if possible.
* \param s first argument, string which can be replaced as a part of the expression
* \param e second argument, Expression
* \return Expression which is modified if combination of sub expressions was possible
*/
Expr* NumExpr::subst(std::string s, Expr* e)
{
    return new NumExpr(this->val) ;
}

/**
* \brief Prints the expression
* \param [out] out output stream
*/
void NumExpr::print(std::ostream &out) {
    out << std::to_string(val);
}

/**
* \brief Prints the expression with more clarity
* \param [out] out output stream
*/
//void NumExpr::pretty_print(std::ostream &out) {
//    this->pretty_print_at(out, precedence_none, false, false, out.tellp());
//}

void NumExpr::pretty_print_at(std::ostream &out, precedence_t precedence, bool parenthesis_let, bool parenthesis_eq, int position) {
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
Val* Add::interp()
{
    return lhs->interp()->add_to(rhs->interp()) ;
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


void Add::pretty_print_at(std::ostream &out, precedence_t precedence, bool parenthesis_let, bool parenthesis_eq, int position) {
    if (precedence >= precedence_add) {
        out << "(";
    }
    this->lhs->pretty_print_at(out, precedence_add, true, true, position);
        out << " + ";
        this->rhs->pretty_print_at(out, precedence_none, false, true, position);
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
Val* Mult::interp()
{
    return lhs->interp()->mult_with( rhs->interp() );
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


void Mult::pretty_print_at(std::ostream &out, precedence_t precedence, bool parenthesis_let, bool parenthesis_eq,  int position) {
    if (precedence >= precedence_mult) {
        out << "(";
    }
    //out <<
    this->lhs->pretty_print_at(out, precedence_mult, true, true, position);
    out << " * " ;
    this->rhs->pretty_print_at(out, precedence_add, !(precedence >= precedence_mult) && parenthesis_let, true, position);
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
Val* Var::interp()
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


void Var::pretty_print_at(std::ostream &out, precedence_t precedence, bool parenthesis_let, bool parenthesis_eq, int position) {
    out << this->name;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* \brief Constructor
*/
Let::Let(std::string lhs, Expr *rhs, Expr *body)
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
Val* Let::interp() // More Testing rhs->interp()
{
    return this->body->subst(this->lhs, this->rhs)->interp();
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
    if( this->lhs == subt )
    {
        return new Let(this->lhs, this->rhs->subst(subt, exp), this->body);
    }
    else
    {
        return new Let(this->lhs, this->rhs->subst(subt, exp), this->body->subst(subt, exp));  // ->subst(subt, exp)
    }
}


/**
* \brief Prints the expression
* \param [out] out output stream
*/
void Let::print(std::ostream &out)
{
    out<< "(_let " << this->lhs << "=" << this->rhs->to_string() << " _in " << this->body->to_string() << ")" ;
}


void Let::pretty_print_at(std::ostream &out, precedence_t precedence, bool parenthesis_let, bool parenthesis_eq, int position)
{
    if (parenthesis_let) {
            out << "(";
            //position += 1;
        }
    int spaces = (int) out.tellp() - position ;
    out << "_let " << this->lhs << " = " ;
    this->rhs->pretty_print_at(out, precedence, false, false, position) ;
    out << "\n" ;
    position = out.tellp() ;
    out << std::string(spaces, ' ') << "_in  " ;
    this->body->pretty_print_at(out, precedence_none, false, false, position);
    if (parenthesis_let)
    {
        out << ")";
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


BoolExpr::BoolExpr(bool rep)
{
    this->rep = rep ;
}
    
bool BoolExpr::equals(Expr *e)
{
    auto *other = dynamic_cast<BoolExpr *>(e) ;
    if(other == nullptr)
    {
        return false;
    }
    return  this->rep == other->rep;
}

Val *BoolExpr::interp()
{
    return new BoolVal(this->rep);
}

bool BoolExpr::has_variable()
{
    return false;
}

Expr *BoolExpr::subst(std::string s, Expr *e)
{
    return new BoolExpr(this->rep) ;
}

void BoolExpr::print(std::ostream &out)
{
    if(this->rep)
    {
        out << "_true" ;
    }
    else
    {
        out << "_false" ;
    }
}

void BoolExpr::pretty_print_at(std::ostream &out, precedence_t precedence, bool parenthesis_let, bool parenthesis_eq, int position)
{
    if(this->rep)
    {
        out << "_true" ;
    }
    else
    {
        out << "_false" ;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


IfExpr::IfExpr(Expr *condition, Expr *then_expr, Expr *else_expr)
{
    this->condition = condition;
    this->then_expr = then_expr;
    this->else_expr = else_expr;
    }
    
bool IfExpr::equals(Expr *e)
{
    auto *other = dynamic_cast<IfExpr *>(e);
    if (other == nullptr) {
        return false;
    }
    return this->condition->equals(other->condition) && this->then_expr->equals(other->then_expr) &&
           this->else_expr->equals(other->else_expr);
}

Val *IfExpr::interp()
{
    if (this->condition->interp()->is_true()) {
        return this->then_expr->interp();
    } else {
        return this->else_expr->interp();
    }
}

bool IfExpr::has_variable()
{
    return this->condition->has_variable() || this->then_expr->has_variable() || this->else_expr->has_variable();
}

Expr *IfExpr::subst(std::string s, Expr *e)
{
    Expr *subst_condition = this->condition->subst(s, e);
    Expr *subst_then_expr = this->then_expr->subst(s, e);
    Expr *subst_else_expr = this->else_expr->subst(s, e);
    return new IfExpr(subst_condition, subst_then_expr, subst_else_expr);
}

void IfExpr::print(std::ostream &out)
{
    out << "(_if ";
    //auto *condition = new BoolVal(this->condition);
    out << this->condition->to_string();
    out << " _then ";
    this->then_expr->print(out);
    out << " _else ";
    this->else_expr->print(out);
    out << ")";
}


void IfExpr::pretty_print_at(std::ostream &out, precedence_t precedence, bool parenthesis_let, bool parenthesis_eq, int position)
{

    int blank_spaces_backoff = (int) out.tellp() - position;
        out << "_if ";
        this->condition->pretty_print_at(out, precedence_none, false, false, position);
        out << "\n";
        position = out.tellp();
        out << std::string(blank_spaces_backoff, ' ') << "_then ";
        this->then_expr->pretty_print_at(out, precedence_none, false, false, position);
        out << "\n";
        position = out.tellp();
        out << std::string(blank_spaces_backoff, ' ') << "_else ";
        this->else_expr->pretty_print_at(out, precedence_none, false, false, position);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EqExpr::EqExpr(Expr *lhs, Expr *rhs)
{
    this->lhs = lhs ;
    this->rhs = rhs ;
}

bool EqExpr::equals(Expr *e)
{
    auto *other = dynamic_cast<EqExpr *>(e);
        if (other == nullptr) {
            return false;
        }
        return this->lhs->equals(other->lhs) && this->rhs->equals(other->rhs);
}

Val *EqExpr::interp()
{
    auto *lhs = this->lhs->interp();
    auto *rhs = this->rhs->interp();
    bool result = lhs->equals(rhs);
    return new BoolVal(result);
}

bool EqExpr::has_variable()
{
    return this->lhs->has_variable() || this->rhs->has_variable();
}

Expr *EqExpr::subst(std::string s, Expr *e)
{
    Expr *subst_lhs = this->lhs->subst(s, e);
    Expr *subst_rhs = this->rhs->subst(s, e);
    return new EqExpr(subst_lhs, subst_rhs);
}

void EqExpr::print(std::ostream &out)
{
    out << "(";
    this->lhs->print(out);
    out << "==";
    this->rhs->print(out);
    out << ")";
}

void EqExpr::pretty_print_at(std::ostream &out, precedence_t precedence, bool parenthesis_let, bool parenthesis_eq, int position)
{
    if (parenthesis_let) {
        out << "(";
    }
    this->lhs->pretty_print_at(out, precedence_none, true, true, position);
    out << " == ";
    this->rhs->pretty_print_at(out, precedence_none, false, false, position);
    if (parenthesis_eq)
    {
        out << ")";
    }
    
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////








