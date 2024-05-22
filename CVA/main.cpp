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
    // Leg1 is fixed rate with annual payments and 5% fixed rate
    // Leg2 is floating rate with semi-annual payment and 1% spread

    VanillaSwap Swap1(6.0,100000.0, {1.0,0.0381},{2.0,0.01});
    std::vector<double> flow1 = Swap1.getLegFlows(1);
    std::vector<double> flow2 = Swap1.getLegFlows(2);

    // Defining rate curve with pillars as 6m, 1Y, 2Y, 5Y, 7Y, 10Y
    RateCurve SOFR({{0.5,0.0225},{1.0,0.03},{2.0,0.035},{5.0,0.0525},{7.0,0.0575},{10.0,0.06}});

    std::vector<double> Leg1disc = SOFR.getDiscFactors(flow1);
    std::vector<double> Leg2disc = SOFR.getDiscFactors(flow2);
    std::vector<double> Leg2fwd  = SOFR.getFwdRates(flow2);

    // Pricing trade with above swap object and rate curve (discount factors and fwd rates)
    SwapPricer price1(Swap1,Leg1disc,Leg2disc,Leg2fwd);
    double leg1PV = price1.getLeg1NPV();
    double leg2PV = price1.getLeg2NPV();
    double tradePV = price1.getTradeNPV();

    std::cout << "Leg1PV:" << leg1PV << " Leg2PV:" << leg2PV << " tradePV:" << tradePV << std::endl;

    // input fixed PD and LGD   
    return 0;
}