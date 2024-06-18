#include <vector>
#include "SimModel.h"
#include "Pricer.h"

int SimulateRate::counter = 0;
int SimulateFxSpot::counter = 0;

SimulateRate::SimulateRate(MarketData* mktData, double vol, double meanRev, double simPaths){
    xMktData=mktData;
    xCurve=xMktData->getRateCurve("USD.SOFR");
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

SimulateFxSpot::SimulateFxSpot(MarketData* mktData, double vol, double simPaths){
    xMktData=mktData;
    xVol=vol;
    xSimPaths=simPaths;
    ++counter;
}

std::vector<double *> SimulateFxSpot::getSimulatedFxSpots(){

    std::vector<double *> spots;
    double *sim01 = new double(1.06);
    double *sim02 = new double(1.07);
    double *sim03 = new double(1.08);
    double *sim04 = new double(1.09);
    double *sim05 = new double(1.10);

    spots.push_back(xMktData->getFxSpots()->front());
    spots.push_back(sim01);
    spots.push_back(sim02);
    spots.push_back(sim03);
    spots.push_back(sim04);
    spots.push_back(sim05);

    return spots;
}
