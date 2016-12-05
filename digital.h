#ifndef DIGITAL_H
#define DIGITAL_H
#include "Option.h"


class digital : public Option
{
    public:
        digital();
        digital(double maturity, double spot, double strike, int underlying,bool call): Option(maturity, spot, strike, underlying,call){};
        virtual ~digital();
        digital(const digital& other);
        digital& operator=(const digital& other);

    protected:

    private:
};

#endif // DIGITAL_H
