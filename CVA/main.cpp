#include <iostream>
#include <vector>
#include <string>
#include "RateCurve.h"
#include "Swap.h"
#include "Pricer.h"
#include "Exposure.h"

int main()
{
    // defining 5Y swap with 100,000 notional receive fixed-pay float
    // Leg1 receives 3.81% annually and Leg2 pays SOFR+1% semi-annually
    VanillaSwap Swap1(6.0,100000.0, {1.0,0.0381},{2.0,0.01});

    // Defining rate curve with pillars as 6m, 1Y, 2Y, 5Y, 7Y, 10Y. For now using zero rates
    RateCurve SOFR({{0.5,0.0225},{1.0,0.03},{2.0,0.035},{5.0,0.0525},{7.0,0.0575},{10.0,0.06}});

    // Pricing trade with above swap object and rate curve (discount factors and fwd rates)
    SwapPricer price1(Swap1,SOFR);
    double leg1PV = price1.getLeg1NPV();
    double leg2PV = price1.getLeg2NPV();
    double tradePV = price1.getTradeNPV();

    std::cout << "Leg1PV:" << leg1PV << " Leg2PV:" << leg2PV << " tradePV:" << tradePV << std::endl;

    // input fixed PD and LGD   
    return 0;
}