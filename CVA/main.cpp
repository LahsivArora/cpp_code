#include <iostream>
#include <vector>
#include <cmath>
#include "RateCurve.h"
#include "Swap.h"
#include "Pricer.h"
#include "SimModel.h"
#include "Exposure.h"

int main()
{
    // defining 5Y swap with 100,000 notional receive fixed-pay float
    // Leg1 receives 3.81% annually and Leg2 pays SOFR+1% semi-annually
    VanillaSwap Swap1(6.0,1000000.0, {1.0,0.0381},{2.0,0.01});

    // Defining rate curve with pillars as 6m, 1Y, 2Y, 5Y, 7Y, 10Y. For now using zero rates
    RateCurve SOFR({{0.5,0.0225},{1.0,0.03},{2.0,0.035},{5.0,0.0525},{7.0,0.0575},{10.0,0.06}});

    // Pricing trade with above swap object and rate curve (discount factors and fwd rates)
    SwapPricer price1(Swap1,SOFR);
    double expectedPV = price1.getTradeNPV();
    std::cout << "base tradePV:" << expectedPV << std::endl;

    // simulation parameters
    double rateVol = 0.15; // i.e. 15% annual vol. constant for now.
    double simPaths = 5; // hardcoding simulation for now
    SimulateRate simEngine(SOFR, rateVol, simPaths);
    std::vector<RateCurve> simCurves = simEngine.getSimulatedCurves();

    for (int i=0; i <simCurves.size(); i++){
        SwapPricer price2(Swap1,simCurves[i]);
        double simPrice = price2.getTradeNPV();
        expectedPV += simPrice;
        std::cout << "sim"+std::to_string(i)+" tradePV:" << simPrice << std::endl;
    }
    std::cout << "expected tradePV:" << expectedPV/6.0 << std::endl;

/*    // exposure parameters
    double timeSteps = 0.25; // i.e. quarterly

    // input fixed PD and LGD
    double LGD = (1.0-0.6); // loss given default (LGD) assuming recovery rate (RR) is 60%
    double lambda = 250.0/10000.0/LGD ; // using constant hazard rate + CDS spread of 250bp
    double CuPD450 = 1.0 - exp(-1.0*lambda*4.50);
    double CuPD475 = 1.0 - exp(-1.0*lambda*4.75);
    double MarPD = CuPD475 - CuPD450;
    std::cout << "hazard rate:" << lambda << std::endl;
    std::cout << "marginal probability of default:" << MarPD << std::endl;*/

    return 0;
}