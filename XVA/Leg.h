#ifndef LEG_H_
#define LEG_H_

#include <vector>
#include <string>
#include "Enums.h"
#include <iostream>

class Leg{
private:
    LegType xType;
    Currency xLegCcy;
    double xFreq, xRate;
    std::string xCurveName;
public:
    static int counter;
    // for Leg, choose fixed or float leg type and enter cashflow freq/year and fixed rate/float spread
    Leg(LegType type = LegType::Fixed, Currency ccy = Currency::USD, double freq = 0.0, double rate = 0.0, std::string curveName = "");
    std::vector<double> getLegFlows(double maturity);
    LegType getLegType();
    double getLegRate();
    double getLegFreq();
    std::string getLegCurveName();
};

#endif