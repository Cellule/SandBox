#pragma once
#include "Symbol.h"

enum Precedence
{
    Cma
    , Asg
    , Que
    , Lor
    , Lan
    , Bor
    , Xor
    , Ban
    , Equ
    , Cmp
    , Shf
    , Add
    , Mul
    , No
};


class Operator :
    public Symbol
{
public:
    Operator();
    ~Operator();

    static const Precedence opPrec[tkMaxOperator];
};

