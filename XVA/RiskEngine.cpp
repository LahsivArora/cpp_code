#include "RiskEngine.h"

int RiskEngine::counter = 0;

RiskEngine::RiskEngine(NettingSet* netSet, MarketData* mktData, std::string bumpCurve){
    xNetSet=netSet;
    xMktData=mktData;
    xBumpCurve=bumpCurve;
    for (auto it = xMktData->getRateCurves()->begin(); it != xMktData->getRateCurves()->end(); it++){
        if ((*it)->getName() == xBumpCurve )
            xCurve = *it;
    }
    ++counter;
}

RiskEngine::RiskEngine(Swap* swap, MarketData* mktData, std::string bumpCurve){
    xMktData=mktData;
    xSwap=swap;
    xSwaps = new std::vector<Swap *>;
    xSwaps->push_back(xSwap);
    xNetSet= new NettingSet(xSwaps);
    xBumpCurve=bumpCurve;
    for (auto it = xMktData->getRateCurves()->begin(); it != xMktData->getRateCurves()->end(); it++){
        if ((*it)->getName() == xBumpCurve )
            xCurve = *it;
    }
    ++counter;
}

std::map<double,double> RiskEngine::calcIRDelta(){

    std::map<double,double> bumpedPVs;
    std::map<double,double> delta;
    std::map<double,double> xRates = xCurve->getRates();
    double bump = 0.0001; // +1 basis point
    SwapPricer *priceBase = new SwapPricer(xNetSet,xMktData);
    double basePV = priceBase->calcTradeNPV();

    // reverse cumulative calculation

    for (auto it=xRates.rbegin(); it != xRates.rend(); it++){
        double tenor = it->first;
        xRates[tenor]= it->second + bump;
        RateCurve *bumpedCurve = new RateCurve(xCurve->getName(),xRates);
        MarketData* bumpedMktData = new MarketData;
        bumpedMktData = xMktData->createBumpedMarktData(bumpedCurve);
        SwapPricer *priceBump = new SwapPricer(xNetSet, bumpedMktData);
        double bumpedPV = priceBump->calcTradeNPV();
        bumpedPVs.insert(std::pair<double,double>(tenor,bumpedPV-basePV));
        delete bumpedMktData;
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
    Swap* current = (xNetSet->getTrades())->front();
    if ( current->getTradeType() == TradeType::IrSwap) 
        delta = 1.0;
    else
        throw std::string("code only handles RWA for VanillaSwap; code to be enhanced");

    return delta; 
}