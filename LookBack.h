#ifndef LOOKBACK_H
#define LOOKBACK_H

#include "Option.h"


class LookBack : public Option
{
    public:
        LookBack();
        LookBack(double maturity, double spot, double strike, int underlying, bool call): Option(maturity, spot, strike, underlying, call){};
        virtual ~LookBack();
        LookBack(const LookBack& other);
        LookBack& operator=(const LookBack& other);

        bool isFloat(){if (floatt) return true; return false; };

    protected:

    private:
        bool floatt;
};

#endif // LOOKBACK_H
