#include "MonteCarloOP.h"
#include "Option.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>
#include<string>
#include<sstream>
#include<stdlib.h>
#include <numeric>
#include <random>

using namespace std;

MonteCarloOP::MonteCarloOP()
{
    //ctor
}

MonteCarloOP::MonteCarloOP( double timestep, int pathnum,double InterestRate, double Volatility)
{
    //ctor
    optype = 5;
    TimeStep = timestep;
    NumPath = pathnum;
    r = InterestRate;
    vol = Volatility;
}

MonteCarloOP::MonteCarloOP( barrier Barrier, double timestep, int pathnum, double InterestRate, double Volatility)
{
    //ctor
    optype = 0;
    Time = Barrier.Getmaturity();
    TimeStep = timestep;
    NumPath = pathnum;
    r= InterestRate;
    vol = Volatility;

}

MonteCarloOP::MonteCarloOP(digital Digital, double timestep, int pathnum, double InterestRate, double Volatility)
{
    //ctor
    optype = 1;
    Time = Digital.Getmaturity();
    TimeStep = timestep;
    NumPath = pathnum;
    r= InterestRate;
    vol = Volatility;
}

MonteCarloOP::MonteCarloOP(LookBack lockback, double timestep, int pathnum,double InterestRate, double Volatility)
{
    //ctor
    optype = 2;
    Time = lockback.Getmaturity();
    TimeStep = timestep;
    NumPath = pathnum;
    r= InterestRate;
    vol = Volatility;
}

MonteCarloOP::MonteCarloOP(Asian asian, double timestep, int pathnum,double InterestRate, double Volatility)
{
    //ctor
    optype = 3;
    Time = asian.Getmaturity();
    TimeStep = timestep;
    NumPath = pathnum;
    r = InterestRate;
    vol = Volatility;
}

MonteCarloOP::MonteCarloOP(European& eur, double timestep, int pathnum,double InterestRate, double Volatility)
{
    //ctor
    optype = 4;
    Time = eur.Getmaturity();
    TimeStep = timestep;
    NumPath = pathnum;
    r = InterestRate;
    vol = Volatility;
}


MonteCarloOP::~MonteCarloOP()
{
    //dtor
}

MonteCarloOP::MonteCarloOP(const MonteCarloOP& other)
{
    //copy ctor
}

MonteCarloOP& MonteCarloOP::operator=(const MonteCarloOP& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}

vector<vector<double>>  MonteCarloOP:: GeneratePath()
{
	//Normal Random Generator
	default_random_engine generator;
	normal_distribution<double> distribution(0.0,1.0);
	vector<vector<double>> Paths;
	//double spot = 100, Time = 1, TimeStep = 252, vol = 0.05, r = 0.02;
	double S_plus;
	for (int i = 0 ; i< NumPath;i++)
	{
		double S_plus= spot;
		vector <double> Price_in_Path;
		Price_in_Path.push_back(S_plus);

		for (int  index = 0; index < (Time/TimeStep); index++)
		{
			double dW = distribution(generator);//Standard normal random variable
			S_plus=S_plus*exp(r*TimeStep+0.5*vol*sqrt(TimeStep)*dW);
			Price_in_Path.push_back(S_plus);
		}
		Paths.push_back(Price_in_Path);
	}
	return Paths;
}

vector<vector<double>> MonteCarloOP::GeneratePathCommodity() {
	//int main(){
	//int NumPath = 10;
	string readline;
	vector<double> ret;
	ifstream infile("Ret.csv");
	if (infile.fail()) {
		cerr << "Failed to open the file.\n";//fail to open
	}
	getline(infile, readline);//process the data
	while (getline(infile, readline)) {
			ret.push_back(stof(readline));
	}
	infile.close();
	double sum = accumulate(ret.begin(), ret.end(), 0.0);
	double sq_sum = 0;
	double jump = 0;
	double s = spot;
	//double r = 0.005;//interest rate

	double mean = sum / ret.size();
	for (int i = 0; i < ret.size(); i++) {
		sq_sum += pow((ret[i] - mean), 2);
	}
	double stdev = sqrt(sq_sum / (ret.size() - 1));
	//double sigma = sqrt(252 * pow(stdev, 2));//annualized volatility
	double sigma = vol;
	vector<double>M(252);
	int a = 0;
	for (int i = 0; i < ret.size(); i++) {
		if (abs(ret[i]) > stdev) {
			jump += 1;
			M[a] = ret[i];
			a++;
		}
	}
	double jump_mean = accumulate(M.begin(), M.end(), 0.0) / jump;
	double jump_sq_sum = 0;
	for (int i = 0; i < jump; i++) {
		jump_sq_sum += pow((M[i] - jump_mean), 2);
	}
	double jump_std = sqrt(jump_sq_sum / (jump - 1));
	double lambda = jump / ret.size();
	double kappa = exp(0.5*pow(jump_std, 2) + jump_mean) - 1.;
	double drift = (r - pow(sigma, 2) / 2 - lambda*kappa);

	//double Time = 252;
	//double dt = 1 / Time;
	double step = Time / TimeStep;
	double time = Time;
	double Timestep = TimeStep;
	vector < vector < double >> path_ln(NumPath, vector<double>(Time / TimeStep));
	vector < vector < double >> path(NumPath, vector<double>(Time / TimeStep));
	for (int i = 0; i < NumPath; i++) {
		path_ln[i][0] = log(s);
		path[i][0] = s;
	}
	default_random_engine generator;
	normal_distribution<double> distribution(0.0, 1.0);
	for (int i = 0; i < NumPath; i++) {
		for (int j = 1; j < Time / TimeStep; j++) {

			double randomNumber = 0, randomNumber2 = 0;
			randomNumber = distribution(generator);
			randomNumber2 = distribution(generator);
			double jump_chek = 0;
			int jump_mask = 0;
			double jump_size = 0;
			jump_chek = rand() / double(RAND_MAX);
			if (jump_chek <= lambda*TimeStep)
				jump_mask = 1;
			jump_size = (jump_mean + jump_std*randomNumber)*jump_mask;
			path_ln[i][j] = path_ln[i][j - 1] + drift*TimeStep + sigma*sqrt(TimeStep)*(randomNumber2)+jump_size;
			path[i][j] = exp(path_ln[i][j]);
		}
	}
	//return 0;
	return path;
}


double MonteCarloOP::CalculatePrice(barrier& Barrier) {
	int type = Barrier.isCall();
	vector<vector<double>> path;
	Time = Barrier.Getmaturity();
	spot = Barrier.GetSpot();
	double underlying_type = Barrier.GetUnderlying();
	if (underlying_type == 1) {
        path = GeneratePath();
	}
	else {
        path = GeneratePathCommodity();
	}

	double Bar = Barrier.GetBar();
	int n = Time / TimeStep;
	int property=Barrier.GetProperty();
	double strike = Barrier.GetStrike();
	double SumPrice = 0;
	//call option
	if (type == 1) {
		if (property == 1){
            bool signal = 0;
			for (int i = 0; i < NumPath; i++) {
				double price = 0;
				for (int j = 0; j < n; j++) {
					if (path[i][j] < Bar) {
						//down and in
						bool signal = 1;
						break;
					}
				}
				if (signal == 1 && path[i][n-1] > strike) {
					price = (path[i][n-1] - strike)*exp(-Time*r);
				}
				SumPrice += price;
			}
			return SumPrice;
        }


		if (property == 2) {
            //down and out
            bool signal = 1;
			for (int i = 0; i < NumPath; i++) {
				double price = 0;
				for (int j = 0; j < n; j++) {
					if (path[i][j] < Bar) {
						bool signal = 0;
						break;
					}
				}
				if (signal == 1 && path[i][n-1] > strike) {
					price = (path[i][n-1] - strike)*exp(-Time*r);
				}
				SumPrice += price;
			}
			return SumPrice;

		}


		if (property == 3) {
            //up and in
            bool signal = 0;
			for (int i = 0; i < NumPath; i++) {
				double price = 0;
				for (int j = 0; j < n; j++) {
					if (path[i][j] > Bar) {
						bool signal = 1;
						break;
					}
				}

				if (signal == 1 && path[i][n-1] > strike) {
					price = (path[i][n-1] - strike)*exp(-Time*r);
				}
				SumPrice += price;
			}
			return SumPrice;
		}

        if (property == 4) {
            //up and out
			bool signal = 1;
			for (int i = 0; i < NumPath; i++) {
				double price = 0;
				for (int j = 0; j < n; j++) {
					if (path[i][j] > Bar) {
						bool signal = 0;
						break;
					}
				}
				if (signal == 1 && path[i][n-1] > strike) {
					price = (path[i][n-1] - strike)*exp(-Time*r);
				}
				SumPrice += price;
			}
			return SumPrice;
        }
	}
	if (type == 0) {
		//put option
		if (property == 1){
            bool signal = 0;
			for (int i = 0; i < NumPath; i++) {
				double price = 0;
				for (int j = 0; j < n; j++) {
					if (path[i][j] < Bar) {
						//down and in
						bool signal = 1;
						break;
					}
				}
				if (signal == 1 && path[i][n-1] < strike) {
					price = (strike - path[i][n-1])*exp(-Time * r);
				}
				SumPrice += price;
			}
			return SumPrice;
		}


        if (property == 2){
            //down and out
			bool signal = 1;
			for (int i = 0; i < NumPath; i++) {
				double price = 0;
				for (int j = 0; j < n; j++) {
					if (path[i][j] < Bar) {
						bool signal = 0;
						break;
					}
				}
				if (signal == 1 && path[i][n-1] < strike) {
					price = (strike - path[i][n-1])* exp(-Time * r);
				}
				SumPrice += price;
			}
			return SumPrice;
        }

		if (property == 3) {
            //up and in
			bool signal = 0;
			for (int i = 0; i < NumPath; i++) {
				double price = 0;
				for (int j = 0; j < n; j++) {
					if (path[i][j] > Bar) {
						bool signal = 1;
						break;
					}
				}

				if (signal == 1 && path[i][n-1] < strike) {
					price = (strike - path[i][n-1])*exp(-Time*r);
				}
				SumPrice += price;
			}
			return SumPrice;
		}

        if (property == 4) {
            //up and out
			bool signal = 1;
			for (int i = 0; i < NumPath; i++) {
				double price = 0;
				for (int j = 0; j < n; j++) {
					if (path[i][j] > Bar) {
						bool signal = 0;
						break;
					}
				}
				if (signal == 1 && path[i][n-1] < strike) {
					price = (strike - path[i][n-1])*exp(-Time*r);
				}
				SumPrice += price;
			}
			return SumPrice;
        }

	}
}

double MonteCarloOP::CalculatePrice(digital& Digital) {
	vector<vector<double>> path;
    Time = Digital.Getmaturity();
    spot = Digital.GetSpot();
	double underlying_type = Digital.GetUnderlying();
	if (underlying_type == 1) {
        path = GeneratePath();
	}
	else {
        path = GeneratePathCommodity();
	}

	int n = Time / TimeStep;
	double strike = Digital.GetStrike();
	double SumPrice = 0;
	int type = Digital.isCall();
	if (type == 1) {
		for (int i = 0; i < NumPath; i++) {
			double price = 0;
			if (path[i][n-1] > strike) {
				price = 1 * exp(-r*Time);
			}
			SumPrice += price;
		}
		return SumPrice;
	}
	else {
		for (int i = 0; i < NumPath; i++) {
			double price = 0;
			if (path[i][n-1] < strike) {
				price = 1 * exp(-r*Time);
			}
			SumPrice += price;
		}
		return SumPrice;
	}
}




double MonteCarloOP::CalculatePrice(European& Eur){
	vector<vector <double>> path;
    Time = Eur.Getmaturity();
    spot = Eur.GetSpot();
	double underlying_type = Eur.GetUnderlying();
	if (underlying_type == 1) {
        path = GeneratePath();
	}
	else {
        path = GeneratePathCommodity();
	}

	//double Strike;
    double Strike = Eur.GetStrike();
	bool isCall;
	isCall = Eur.isCall();
	double Payoff = 0;
	int n = Time/TimeStep;
    if (isCall ) {
		for (int i = 0; i < NumPath; i++){
			if  ((path[i][n-1]-Strike) > 0){
                Payoff += exp(-r*Time)*(path[i][n-1]-Strike);
            }
        }
    }
    else {
		for (int i=0; i<NumPath; i++){
			if (Strike - path[i][n-1] > 0){
                Payoff += exp(-r*Time)* (Strike - path[i][n-1]);
			}
        }
    }
    double Price = Payoff/NumPath;
    return Price;
}

double MonteCarloOP::CalculatePrice(Asian& asian){
	vector<vector <double>> path;
	Time = asian.Getmaturity();
	spot = asian.GetSpot();
	double underlying_type = asian.GetUnderlying();
	if (underlying_type == 1) {
        path = GeneratePath();
	}
	else {
        path = GeneratePathCommodity();
	}

	double Strike;
	Strike = asian.GetStrike();
	bool isCall;
	isCall = asian.isCall();
	double Payoff = 0;
	int n = Time/TimeStep;

    if (isCall == true) {
        for (int i=0; i<NumPath; i++){
            double sum = 0;
            for (int j=0; j < n ; j ++) {
                sum += path[i][j];
            }
            double avg = sum/n;
            if (avg>Strike) {
                Payoff += exp(-r*Time)* (avg - Strike);
            }
        }
    }
    else {
        for (int i=0; i<NumPath; i++){
            double sum = 0;
            for (int j=0; j < n ; j ++) {
                sum += path[i][j];
            }
            double avg = sum/n;
            if (avg<Strike){
                Payoff += exp(-r*Time)* (Strike - avg);
            }
        }
    }
    double Price = Payoff/NumPath;
    return Price;
}


double MonteCarloOP::CalculatePrice(LookBack& lookb){
	vector<vector <double>> path;
	Time = lookb.Getmaturity();
	spot = lookb.GetSpot();
	double underlying_type = lookb.GetUnderlying();
	if (underlying_type == 1) {
        path = GeneratePath();
	}
	else {
        path = GeneratePathCommodity();
	}

	double Strike;
	Strike = lookb.GetStrike();
	bool isCall;
	isCall = lookb.isCall();
	bool isFloat;
	isFloat = lookb.isFloat();

	double Payoff = 0;
	int n = Time/TimeStep;
	if(isFloat == true){
    if (isCall == true) {
		for (int i=0; i<NumPath; i++){
			double min = path[i][0];
			for (int j=0; j < n ; j ++) {
				if (path[i][j] < min) {
                        min = path[i][j];
                }
            }
			Payoff += exp(-r*Time)* (path[i][n-1] -  min);
        }
    }
    else {
		for (int i=0; i<NumPath; i++){
			double max = path[i][0];
			for (int j=0; j < n ; j ++) {
				if (path[i][j]>max){ max = path[i][j];}
            }
			Payoff += exp(-r*Time)* ( max -path[i][n-1]);
        }
        }
        double Price = Payoff/NumPath;
        return Price;
    }
///////////////////////elseif fixed lookback
    else {
    if (isCall == false) {
		for (int i=0; i<NumPath; i++){
			double min = path[i][0];
			for (int j=0; j < n ; j ++) {
				if (path[i][j]<min){ min = path[i][j];}
            }
			Payoff += exp(-r*Time)* (Strike - min);
        }
    }
    else {
		for (int i=0; i<NumPath; i++){
			double max = path[i][0];
			for (int j=0; j < n ; j ++) {
				if (path[i][j]>max){ max = path[i][j];}
            }
			Payoff += exp(-r*Time)* (max -Strike);
        }
    }
    double Price = Payoff/NumPath;
    return Price;
    }
}

