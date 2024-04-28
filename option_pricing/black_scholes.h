#ifndef BLACK_SCHOLES_H_
#define BLACK_SCHOLES_H_

#include <cmath>
#include <vector>
#include <string>

double norm(double value);

double normdiff(double value);

class BlackScholes{
private:
    double xspot, xvol, xrate, xtime, xstrike;
    std::string xtype;
public:
    BlackScholes(double spot, double vol, double rate, double time, double strike, std::string optType) ;
    std::vector<double> price();
};
#endif