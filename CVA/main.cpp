#include <iostream>
#include <vector>
#include <cmath>
#include "RateCurve.h"
#include "Swap.h"
#include "Pricer.h"
#include "SimModel.h"
#include "Exposure.h"
#include "CDSCurve.h"

int main()
{
    // Step1: defining 6.5Y swap with 1.5m notional receive fixed-pay float
    // Leg1 receives 4.65% Quarterly and Leg2 pays SOFR-25bps Quarterly
    VanillaSwap Swap1(6.5,1500000.0, {4.0,0.0465},{4.0,-0.0025});

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
    // assuming Quarterly time steps in exposure calculation; exposure is already discounted to today
    ExposureCalc swapExProfile(Swap1,simCurves);
    std::map<double,double> EEprofile = swapExProfile.getEEProfile();

    // Step6: create CDS curve with marginal default probabilities assuming contant hazard rate    
    double CDSSpread = 300.0; // assuming constant CDS spread of 250bps for counterparty
    double LGD = (1.0-0.6); // loss given default (LGD) assuming recovery rate (RR) is 60%
    double timesteps = 0.25; // quarterly steps to match exposure profile
    double maxmaturity = 10.0; // generate marginal PDs upto 10Y

    CDSCurve cds(CDSSpread,LGD,maxmaturity,timesteps);
    std::map<double,double> marginalPDs = cds.getMarginalPDs();

    // Step7: calculate CVA as sum(EE*marginal PD*LGD)
    double CVA = 0.0;
    for (double j=0.25; j<=EEprofile.size()*0.25; j+=0.25){
            //std::cout << "base exposure at "<< j << " :" << EEprofile[j] << " and marginal PD:" << marginalPDs[j] << std::endl;
            CVA += EEprofile[j]*marginalPDs[j]*LGD ;
    }

    std::cout << "CVA for given trade and market data (as $ amount):" << CVA << std::endl;
    std::cout << "CVA for given trade and market data (bps of notional):" << CVA*10000.0/abs(Swap1.getNotional()) << std::endl;

    return 0;
}