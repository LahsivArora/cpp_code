#ifndef LEG_H_
#define LEG_H_

#include <vector>
#include "Enums.h"

class Leg{
private:
    LegType xType;
    Currency xLegCcy;
    double xFreq, xRate;
public:
    Leg();
    Leg(LegType type ,Currency ccy ,double freq, double rate);
    std::vector<double> getLegFlows(double maturity);
    LegType getLegType();
    double getFixedRate();
    double getFloatSpread();
    double getFixedFreq();
    double getFloatFreq();
};

#endif