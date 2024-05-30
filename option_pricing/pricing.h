#ifndef PRICING_H_
#define PRICING_H_

#include <vector>
#include <string>
#include "Enums.h"

class PricingOpt{
private:
    double xSpot, xVol, xTime, xStrike, xRate;
    unsigned int xSteps;
public:
    PricingOpt(double s, double vol, double T, unsigned int n, double K, double r);
    std::vector<double> calcPrice();
};
#endif