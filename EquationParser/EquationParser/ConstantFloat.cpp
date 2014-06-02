#include "ConstantFloat.h"


ConstantFloat::ConstantFloat()
{
}


ConstantFloat::~ConstantFloat()
{
}

void ConstantFloat::print( std::ostream* out ) const
{
    *out << mValue;
}
