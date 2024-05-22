#include <iostream>
#include <vector>
#include <string>
#include "RateCurve.h"
#include "Swap.h"
#include "Exposure.h"

int main()
{
    // defining 5Y swap with 100,000 notional receive fixed-pay float
    // Leg1 is fixed rate with annual payments and 5% fixed rate
    // Leg2 is floating rate with semi-annual payment and 1% spread

    VanillaSwap Swap1(5.0,100000.0, {1.0,0.05},{2.0,0.01});
    std::vector<double> flow1 = Swap1.getLegFlows(1);
    std::vector<double> flow2 = Swap1.getLegFlows(2);

    // Defining rate curve with pillars as 6m, 1Y, 2Y, 5Y, 7Y, 10Y
    RateCurve SOFR({{0.5,0.0325},{1.0,0.0355},{2.0,0.04},{5.0,0.07},{7.0,0.0525},{10.0,0.055}});

    std::vector<double> Leg1disc = SOFR.getDiscFactors(flow1);
    std::vector<double> Leg2disc = SOFR.getDiscFactors(flow2);
    std::vector<double> Leg2fwd  = SOFR.getFwdRates(flow2);

    for (int i = 0; i < Leg2fwd.size(); ++i) {
        std::cout << "tenor is:" << flow2[i] << " and interpolated disc rate is:" << Leg2fwd[i] << std::endl;
    }


    // input fixed PD and LGD


    return 0;
}