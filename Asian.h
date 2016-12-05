#ifndef ASIAN_H
#define ASIAN_H

#include "Option.h"


class Asian : public Option
{
    public:
        Asian();
        Asian(double maturity, double spot, double strike, int underlying, bool call): Option(maturity, spot, strike, underlying,call){};
        virtual ~Asian();
        Asian(const Asian& other);
        Asian& operator=(const Asian& other);

    protected:

    private:
};

#endif // ASIAN_H
