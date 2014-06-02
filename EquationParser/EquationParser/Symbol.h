#pragma once

enum Tokens
{
#define TOK_DCL(name,...) name,
#include "OperatorList.h"
#undef TOK_DCL
    tkMaxOperator,
};

class Symbol
{
public:
    Symbol();
    ~Symbol();
};

