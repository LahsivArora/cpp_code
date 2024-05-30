#ifndef BLACK_SCHOLES_H_
#define BLACK_SCHOLES_H_

#include <cmath>
#include <vector>
#include <string>
#include "Enums.h"

double norm(double value);

double normdiff(double value);

class BlackScholes{
private:
    double xspot, xvol, xrate, xtime, xstrike;
    OptType xtype;
public:
    BlackScholes(double spot, double vol, double rate, double time, double strike, OptType optType) ;
    std::vector<double> price();
};
#endif