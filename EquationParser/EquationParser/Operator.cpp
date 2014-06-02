#include "Operator.h"
#include <iostream>

const OperatorDef Operator::opDefs[tkMaxOperator] = {
#define TOK_DCL(tk,pre,val,n,...) OperatorDef(tk,pre,val,n),
#include "OperatorList.h"
#undef TOK_DCL
};


Operator::Operator()
{
}


Operator::~Operator()
{
}

void Operator::print( std::ostream* out ) const
{
    *out << opDefs[mOp].val.c_str();
}
