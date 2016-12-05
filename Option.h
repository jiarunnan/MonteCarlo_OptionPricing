#ifndef OPTION_H
#define OPTION_H


class Option
{
    public:
        Option();
        Option(double maturity, double spot, double strike, int underlying,bool call): maturity(maturity), Spot(spot), Strike(strike), Underlying(underlying),call(call){};
        virtual ~Option();
        Option(const Option& other);
        Option& operator=(const Option& other);

        virtual double GetPrice() { return Price; }
        void SetPrice(double val) { Price = val; }
        virtual double Getmaturity() { return maturity; }
        void Setmaturity(double val) { maturity = val; }
        virtual double GetSpot() { return Spot; }
        void SetSpot(double val) { Spot = val; }
        virtual double GetStrike() { return Strike; }
        void SetStrike(double val) { Strike = val; }
        virtual int GetUnderlying() { return Underlying; }
        void SetUnderlying(int val) { Underlying = val; }
		virtual bool isCall() { return call; }
        //bool isCall(){ if (call) return true; return false;};

    protected:
        double Price;
        double maturity;
        double Spot;
        double Strike;
        int Underlying;
        bool call;
		
    private:


};

#endif // OPTION_H
