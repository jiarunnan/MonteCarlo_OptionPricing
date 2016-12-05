#include "Option.h"

Option::Option()
{
    //ctor
}

Option::~Option()
{
    //dtor
}

Option::Option(const Option& other)
{
    //copy ctor
}

Option& Option::operator=(const Option& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}
