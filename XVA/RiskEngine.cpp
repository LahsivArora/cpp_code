#include "RiskEngine.h"

int RiskEngine::counter = 0;

RiskEngine::RiskEngine(NettingSet* netSet, RateCurve* curve){
    xNetSet=netSet;
    xCurve=curve;
    ++counter;
}

RiskEngine::RiskEngine(Swap* swap, RateCurve* curve){
    xCurve=curve;
    xSwap=swap;
    xSwaps = new std::vector<Swap *>;
    xSwaps->push_back(xSwap);
    xNetSet= new NettingSet(*xSwaps);
    ++counter;
}

std::map<double,double> RiskEngine::calcIRDelta(){

    std::map<double,double> bumpedPVs;
    std::map<double,double> delta;
    std::map<double,double> xRates = xCurve->getRates();
    double bump = 0.0001; // +1 basis point
    SwapPricer *priceBase = new SwapPricer(this->xNetSet,this->xCurve,this->xCurve,1.0);
    double basePV = priceBase->calcTradeNPV();

    // reverse cumulative calculation

    for (auto it=xRates.rbegin(); it != xRates.rend(); it++){
        double tenor = it->first;
        xRates[tenor]= it->second + bump;
        RateCurve *bumpedCurve = new RateCurve(xCurve->getName(),xRates);
        SwapPricer *priceBump = new SwapPricer(this->xNetSet, bumpedCurve, bumpedCurve,1.0);
        double bumpedPV = priceBump->calcTradeNPV();
        bumpedPVs.insert(std::pair<double,double>(tenor,bumpedPV-basePV));
        delete bumpedCurve;
        delete priceBump;
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
    Swap current = *((*xNetSet).getTrades()[0]);
    if ( current.getTradeType() == TradeType::IrSwap) 
        delta = 1.0;

    return delta; 
}