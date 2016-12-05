#ifndef MONTECARLOOP_H
#define MONTECARLOOP_H
#include "Option.h"
#include "European.h"
#include "barrier.h"
#include "LookBack.h"
#include "Asian.h"
#include "digital.h"
#include <vector>

using namespace std;
class MonteCarloOP
{
    public:
        MonteCarloOP();


        MonteCarloOP(barrier Barrier, double timestep, int pathnum, double InterestRate, double Volatility);
        MonteCarloOP(digital Digital, double timestep, int pathnum, double InterestRate, double Volatility);
        MonteCarloOP(LookBack lookback, double timestep, int pathnum,double InterestRate, double Volatility);
        MonteCarloOP(Asian asian, double timestep, int pathnum,double InterestRate, double Volatility);
        MonteCarloOP(European& eur, double timestep, int pathnum,double InterestRate, double Volatility);
        MonteCarloOP( double timestep, int pathnum,double InterestRate, double Volatility);

        virtual ~MonteCarloOP();
        MonteCarloOP(const MonteCarloOP& other);
        MonteCarloOP& operator=(const MonteCarloOP& other);

        double Getr() { return r; }
        void Setr(double val) { r = val; }
        double Getvol() { return vol; }
        void Setvol(double val) { vol = val; }
        double GetTime() { return Time; }
        void SetTime(double val) { Time = val; }
        double GetTimeStep() { return TimeStep; }
        void SetTimeStep(double val) { TimeStep = val; }

        double CalculatePrice(barrier& Barrier);
        double CalculatePrice(digital& Digital);
        double CalculatePrice(European& Eur);
        double CalculatePrice(Asian& asian);
        double CalculatePrice(LookBack& lookb);


        vector<vector<double>> GeneratePath();
		
        vector<vector<double>> GeneratePathCommodity();

    protected:

    private:
        double r;
        double vol;
        double Time;
        double TimeStep;
        double NumPath;
        double spot;
        double strike;
        int optype;
};

#endif // MONTECARLOOP_H
