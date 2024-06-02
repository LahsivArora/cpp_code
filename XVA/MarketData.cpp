#include "MarketData.h"

int MarketData::counter = 0;

MarketData::MarketData(std::vector<RateCurve*>* rateCurves,std::vector<CDSCurve*>* CDSCurves,std::vector<double*>* fxSpots){
    xRateCurves=rateCurves;
    xCDSCurves=CDSCurves;
    xFxSpots=fxSpots;
}

std::vector<RateCurve*>* MarketData::getRateCurves(){
    return xRateCurves;
}

std::vector<CDSCurve*>* MarketData::getCDSCurves(){
    return xCDSCurves;
}

std::vector<double*>* MarketData::getFxSpots(){
    return xFxSpots;
}