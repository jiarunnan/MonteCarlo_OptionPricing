#include "MonteCarloOP.h"
#include "Option.h"
#include<iostream>
int main(){
    European eur = European(1, 100, 100, 1);//maturity,spot,strike,type
	Asian asian = Asian(1, 100, 90, 0);
    //cout<<eur.GetStrike();
    MonteCarloOP mc = MonteCarloOP(0.01, 1000, 0.15, 0.1);//timestep,numpath,r,vol
	
    cout<<mc.CalculatePrice(asian);
}
