#ifndef EUROPEAN_H
#define EUROPEAN_H
#include "Option.h"

class European : public Option
{
    public:
        European();
        European(double maturity, double spot, double strike, int underlying,bool call): Option(maturity, spot, strike, underlying,call){};
        virtual ~European();

    protected:

    private:
};

#endif // EUROPEAN_H
