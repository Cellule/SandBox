#pragma once
#include "Constant.h"
class ConstantFloat :
    public Constant
{
public:
    ConstantFloat();
    ~ConstantFloat();

    virtual void print( std::ostream* out ) const;

    double mValue;
};

