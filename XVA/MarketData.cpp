#include "MarketData.h"

int MarketData::counter = 0;

MarketData::MarketData(std::vector<RateCurve*>* rateCurves,std::vector<CDSCurve*>* CDSCurves,std::vector<double*>* fxSpots){
    xRateCurves=rateCurves;
    xCDSCurves=CDSCurves;
    xFxSpots=fxSpots;
    ++counter;
}

std::vector<RateCurve*>* MarketData::getRateCurves(){
    return xRateCurves;
}

RateCurve* MarketData::getRateCurve(std::string name){
    RateCurve* getCurve = new RateCurve;
    for (auto it=xRateCurves->begin(); it != xRateCurves-> end(); it++){
        if (name == (*it)->getName())
            getCurve = *it;
    }
    return getCurve;
}

std::vector<CDSCurve*>* MarketData::getCDSCurves(){
    return xCDSCurves;
}

CDSCurve* MarketData::getCDSCurve(std::string name){
    CDSCurve* getCurve = new CDSCurve;
    for (auto it=xCDSCurves->begin(); it != xCDSCurves-> end(); it++){
        if (name == (*it)->getName())
            getCurve = *it;
    }
    return getCurve;
}

std::vector<double*>* MarketData::getFxSpots(){
    return xFxSpots;
}

MarketData* MarketData::replaceRateCurve(RateCurve* changedCurve){
    std::vector<RateCurve*>* changedCurves = new std::vector<RateCurve*>;

    for (auto it=getRateCurves()->begin(); it != getRateCurves()->end(); it++){
        if ((*it)->getName() == changedCurve->getName())
            changedCurves->push_back(changedCurve);
        else
            changedCurves->push_back(*it);
    }
    MarketData* changedMktData = new MarketData(changedCurves,this->getCDSCurves(),this->getFxSpots());
    return changedMktData;
}

MarketData* MarketData::replaceFxSpot(double* changedSpot){
    std::vector<double*>* changedSpots = new std::vector<double*>;
    changedSpots->push_back(changedSpot);
    MarketData* changedMktData = new MarketData(this->getRateCurves(),this->getCDSCurves(),changedSpots);
    return changedMktData;
}