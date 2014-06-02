#include "Operator.h"

const Precedence Operator::opPrec[tkMaxOperator] = {
#define TOK_DCL(tk,pre,...) pre,
#include "OperatorList.h"
#undef TOK_DCL
};


Operator::Operator()
{
}


Operator::~Operator()
{
}
