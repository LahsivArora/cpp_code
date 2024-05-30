#include <cmath>
#include <vector>
#include <string>
#include "black_scholes.h"

double norm(double value)
{
   return 0.5 *(1.0 + erf(value/sqrt(2.0)));
}

double normdiff(double value)
{
   return exp(-0.5*value*value) /sqrt(2.0*M_PI);
}


BlackScholes::BlackScholes(double spot, double vol, double rate, double time, double strike, OptType optType) {
    xspot = spot;
    xvol = vol;
    xrate = rate;
    xtime = time;
    xstrike = strike;
    xtype = optType;
}

std::vector<double> BlackScholes::price() 
{ 
    std::vector<double> ret;
    double price, delta, vega, gamma;
    double d1 = (log(xspot/xstrike) + ( xrate + (xvol*xvol/2.0))*xtime)/(xvol * sqrt(xtime));
    double d2 = d1 - (xvol * sqrt(xtime));

    if (xtype == OptType::CALL)
        {
        price = xspot*norm(d1) - xstrike*norm(d2)*exp(-1.0*xrate*xtime);
        delta = norm(d1);
        vega = xspot*sqrt(xtime)*normdiff(d1);
        gamma = normdiff(d1)/(xspot * xvol * sqrt(xtime));
        }
    else
        {
        price = xstrike*norm(-1.0*d2)*exp(-1.0*xrate*xtime) - xspot*norm(-1.0*d1);
        delta = norm(d1)-1.0;
        vega = xspot*sqrt(xtime)*normdiff(d1);
        gamma = normdiff(d1)/(xspot * xvol * sqrt(xtime));
        }

    ret.push_back(price);
    ret.push_back(delta);
    ret.push_back(vega);
    ret.push_back(gamma);
    return ret;
}
