#pragma once


#include <iostream>

class Symbol
{
public:
    Symbol();
    virtual ~Symbol();

    virtual void print( std::ostream* out ) const = 0;
};

