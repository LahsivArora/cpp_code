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
    // Step1: defining 6.5Y swap with 1,000,000 notional receive fixed-pay float
    // Leg1 receives 4.65% Quarterly and Leg2 pays SOFR-25bps Quarterly
    VanillaSwap Swap1(6.5,1000000.0, {4.0,0.0465},{4.0,-0.0025});

    // Step2: defining rate curve with pillars as 6m, 1Y, 2Y, 5Y, 7Y, 10Y. For now using zero rates
    // curve has hump at 2Y point i.e. slowly downward sloping from 2Y to 10Y
    RateCurve SOFR({{0.5,0.0225},{1.0,0.0375},{2.0,0.05},{5.0,0.049},{7.0,0.0475},{10.0,0.045}});

    // Step3: pricing trade with Swap and RateCurve objects 
    SwapPricer price1(Swap1,SOFR);
    double basePV = price1.getTradeNPV();

    // Step4: simulate curves using base RateCurve object + simulation params 
    double rateVol = 0.15; // i.e. 15% annual vol. constant for now.
    double simPaths = 5; // hardcoding simulation for now
    SimulateRate simEngine(SOFR, rateVol, simPaths);
    std::vector<RateCurve> simCurves = simEngine.getSimulatedCurves();
    std::vector<double> simNPVs = simEngine.getSimulatedBaseNPVs(Swap1);

    // Step5: generate exposure profile using Swap and Simulated curves
    // assuming Quarterly time steps in exposure calculation
    ExposureCalc swapExProfile(Swap1,simCurves);
    std::map<double,double> EEprofile = swapExProfile.getEEProfile();

    for (double j=0.25; j<EEprofile.size()*0.25; j+=0.25){
            std::cout << "base exposure at "<< j << " :" << EEprofile[j] << std::endl;
    }

/*  // input fixed PD and LGD
    double LGD = (1.0-0.6); // loss given default (LGD) assuming recovery rate (RR) is 60%
    double lambda = 250.0/10000.0/LGD ; // using constant hazard rate + CDS spread of 250bp
    double CuPD450 = 1.0 - exp(-1.0*lambda*4.50);
    double CuPD475 = 1.0 - exp(-1.0*lambda*4.75);
    double MarPD = CuPD475 - CuPD450;
    std::cout << "hazard rate:" << lambda << std::endl;
    std::cout << "marginal probability of default:" << MarPD << std::endl;*/

    return 0;
}