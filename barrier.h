#ifndef BARRIER_H
#define BARRIER_H
#include "Option.h"


class barrier : public Option
{
    public:
        barrier();

        barrier(double maturity, double spot, double strike, int underlying, bool call): Option(maturity, spot, strike, underlying,call){};
        virtual ~barrier();
        barrier(const barrier& other);
        barrier& operator=(const barrier& other);

        int GetProperty() { return Property; }
        void SetProperty(int val) { Property = val; }
        int GetBar(){return bar;}
        void SetBar(double Bar) {bar = Bar;}

    protected:

    private:
        int Property;
        double bar;
};

#endif // BARRIER_H
