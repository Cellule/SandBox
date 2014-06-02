#include "ConstantInt.h"


ConstantInt::ConstantInt()
{
}


ConstantInt::~ConstantInt()
{
}

void ConstantInt::print( std::ostream* out ) const
{
    *out << mValue;
}
