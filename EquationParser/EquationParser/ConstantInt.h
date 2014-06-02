#pragma once
#include "Constant.h"
class ConstantInt :
    public Constant
{
public:
    ConstantInt();
    ~ConstantInt();

    virtual void print( std::ostream* out ) const;

    int mValue;
};

