#include <vector>
#include "SimModel.h"
#include "Pricer.h"

int SimulateRate::counter = 0;

SimulateRate::SimulateRate(RateCurve* curve, double vol, double meanRev, double simPaths){
    xCurve=curve;
    xVol=vol;
    xMeanRev=meanRev;
    xSimPaths=simPaths;
    ++counter;
}

std::vector<RateCurve *> SimulateRate::getSimulatedCurves(){

    std::vector<RateCurve *> curves;
    RateCurve *sim01 = new RateCurve("USD.SOFR",{{0.5,0.0233},{1.0,0.0383},{2.0,0.0508},{5.0,0.0498},{7.0,0.0483},{10.0,0.0458}});
    RateCurve *sim02 = new RateCurve("USD.SOFR",{{0.5,0.0218},{1.0,0.0368},{2.0,0.0493},{5.0,0.0483},{7.0,0.0468},{10.0,0.0443}});
    RateCurve *sim03 = new RateCurve("USD.SOFR",{{0.5,0.0240},{1.0,0.0390},{2.0,0.0513},{5.0,0.0500},{7.0,0.0483},{10.0,0.0458}});
    RateCurve *sim04 = new RateCurve("USD.SOFR",{{0.5,0.0210},{1.0,0.0360},{2.0,0.0488},{5.0,0.0480},{7.0,0.0468},{10.0,0.0443}});
    RateCurve *sim05 = new RateCurve("USD.SOFR",{{0.5,0.0240},{1.0,0.0385},{2.0,0.0505},{5.0,0.0480},{7.0,0.0460},{10.0,0.0430}});

    curves.push_back(xCurve);
    curves.push_back(sim01);
    curves.push_back(sim02);
    curves.push_back(sim03);
    curves.push_back(sim04);
    curves.push_back(sim05);

    return curves;
}

std::vector<double> SimulateRate::getSimulatedBaseNPVs(Swap* swap1){

    std::vector<RateCurve *> curves = this->getSimulatedCurves();
    std::vector<double> simNPVs;

    for (unsigned int i=0; i < curves.size(); i++){
        SwapPricer *price2 = new SwapPricer(swap1, curves[i], curves[i],1.0);
        double simPrice = price2->calcTradeNPV();
        simNPVs.push_back(simPrice);
    }
    return simNPVs;
}
