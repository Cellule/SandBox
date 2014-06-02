#pragma once
#include "Symbol.h"
#include <string>
class Variable :
    public Symbol
{
public:
    Variable();
    ~Variable();

    virtual void print( std::ostream* out ) const;

    std::string mName;
};

