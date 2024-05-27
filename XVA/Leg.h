#ifndef LEG_H_
#define LEG_H_

#include <vector>
#include <string>
#include "Enums.h"

class Leg{
private:
    LegType xType;
    Currency xLegCcy;
    double xFreq, xRate;
    std::string xCurveName;
public:
    // for Leg, choose fixed or float leg type and enter cashflow freq/year and fixed rate/float spread
    Leg();
    Leg(LegType type ,Currency ccy ,double freq, double rate, std::string curveName = "");
    std::vector<double> getLegFlows(double maturity);
    LegType getLegType();
    double getLegRate();
    double getLegFreq();
    std::string getLegCurveName();
};

#endif