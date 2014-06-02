#include "Variable.h"
#include <iosfwd>
#include <iostream>


Variable::Variable()
{
}


Variable::~Variable()
{
}

void Variable::print( std::ostream* out ) const
{
    *out << mName;
}
