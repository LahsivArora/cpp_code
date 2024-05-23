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
    // defining 6.5Y swap with 1,000,000 notional receive fixed-pay float
    // Leg1 receives 4.65% Quarterly and Leg2 pays SOFR-25bps Quarterly
    VanillaSwap Swap1(6.5,1000000.0, {4.0,0.0465},{4.0,-0.0025});

    // Defining rate curve with pillars as 6m, 1Y, 2Y, 5Y, 7Y, 10Y. For now using zero rates
    // curve has hump at 2Y point i.e. slowly downward sloping from 2Y to 10Y
    RateCurve SOFR({{0.5,0.0225},{1.0,0.0375},{2.0,0.05},{5.0,0.049},{7.0,0.0475},{10.0,0.045}});

    // Pricing trade with above swap object and rate curve (discount factors and fwd rates)
    SwapPricer price1(Swap1,SOFR);
    double basePV = price1.getTradeNPV();

    // simulation parameters
    double rateVol = 0.15; // i.e. 15% annual vol. constant for now.
    double simPaths = 5; // hardcoding simulation for now
    SimulateRate simEngine(SOFR, rateVol, simPaths);
    std::vector<RateCurve> simCurves = simEngine.getSimulatedCurves();
    std::vector<double> simNPVs = simEngine.getSimulatedBaseNPVs(Swap1);

    std::map<double,double> EEprofile;
    double maxMaturity = Swap1.getMaturity()+1.0;
    double n = simCurves.size();
    // Exposure calc. profile matches expectations. using quarterly calc for exposures
    for (double i=0.25; i<maxMaturity; i+=0.25){
        double exposure = 0.0;
        for (int j=0; j<simCurves.size(); j++){
            SwapPricer priceLag(Swap1,simCurves[j],i);
            double positiveExposure = (priceLag.getTradeNPV()>0?priceLag.getTradeNPV():0);
            exposure += positiveExposure;
        }
        EEprofile.insert(std::pair<double,double>(i,exposure/n));        
    }

    for (double j=0; j<EEprofile.size()*0.25; j+=0.25){
            std::cout << "base exposure at "<< j << " :" << EEprofile[j] << std::endl;
    }
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