#include "RiskEngine.h"

RiskEngine::RiskEngine(){}

RiskEngine::RiskEngine(NettingSet& netSet, RateCurve& curve){
    xNetSet=netSet;
    xCurve=curve;
}

RiskEngine::RiskEngine(VanillaSwap& swap, RateCurve& curve){
    xSwap=swap;
    xCurve=curve;
    std::vector<VanillaSwap> dummy;
    dummy.push_back(xSwap);
    NettingSet netSetDummy(dummy);
    xNetSet=netSetDummy;
}

std::map<double,double> RiskEngine::calcIRDelta(){

    std::map<double,double> bumpedPVs;
    std::map<double,double> delta;
    std::map<double,double> xRates = xCurve.getRates();
    double bump = 0.0001; // +1 basis point
    double basePV = xNetSet.getTradesNPV(xCurve);

    // reverse cumulative calculation

    for (auto it=xRates.rbegin(); it != xRates.rend(); it++){
        double tenor = it->first;
        xRates[tenor]= it->second + bump;
        RateCurve bumpedCurve(xRates);
        double bumpedPV = xNetSet.getTradesNPV(bumpedCurve);
        bumpedPVs.insert(std::pair<double,double>(tenor,bumpedPV-basePV));
    }

    auto it1st = bumpedPVs.rbegin();
    delta.insert(std::pair<double,double>(it1st->first,it1st->second));
    for (auto it= next(it1st,1); it != bumpedPVs.rend(); it++){
        double tenor = it->first;
        double deltaTenor = it->second - prev(it,1)->second;
        delta.insert(std::pair<double,double>(tenor,deltaTenor));
    }

    return delta;
}

double RiskEngine::calcRWADelta(){
    
    double delta;
    
    if (xNetSet.getTrades()[0].getTradeType() == TradeType::IrSwap) 
        delta = 1.0;

    return delta; 
}