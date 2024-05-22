#include <vector>
#include "SimModel.h"
#include "Pricer.h"

SimulateRate::SimulateRate(RateCurve curve, double vol, double simPaths){
    xCurve=curve;
    xVol=vol;
    xSimPaths=simPaths;
}

std::vector<RateCurve> SimulateRate::getSimulatedCurves(){

    std::vector<RateCurve> curves;
    RateCurve sim01({{0.5,0.019},{1.0,0.025},{2.0,0.029},{5.0,0.042},{7.0,0.049},{10.0,0.052}});
    RateCurve sim02({{0.5,0.0205},{1.0,0.027},{2.0,0.033},{5.0,0.0515},{7.0,0.0565},{10.0,0.059}});
    RateCurve sim03({{0.5,0.0215},{1.0,0.029},{2.0,0.034},{5.0,0.0515},{7.0,0.0565},{10.0,0.059}});
    RateCurve sim04({{0.5,0.0245},{1.0,0.035},{2.0,0.035},{5.0,0.0475},{7.0,0.0535},{10.0,0.058}});
    RateCurve sim05({{0.5,0.0265},{1.0,0.035},{2.0,0.04},{5.0,0.0575},{7.0,0.0625},{10.0,0.065}});

    curves.push_back(xCurve);
    curves.push_back(sim01);
    curves.push_back(sim02);
    curves.push_back(sim03);
    curves.push_back(sim04);
    curves.push_back(sim05);

    return curves;
}

std::vector<double> SimulateRate::getSimulatedBaseNPVs(VanillaSwap swap1){

    std::vector<RateCurve> curves = getSimulatedCurves();
    std::vector<double> simNPVs;

    for (int i=0; i <curves.size(); i++){
        SwapPricer price2(swap1,curves[i]);
        double simPrice = price2.getTradeNPV();
        simNPVs.push_back(simPrice);
    }
    return simNPVs;
}
