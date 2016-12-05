#include "barrier.h"

barrier::barrier()
{
    //ctor
}

barrier::~barrier()
{
    //dtor
}

barrier::barrier(const barrier& other)
{
    //copy ctor
}

barrier& barrier::operator=(const barrier& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}
