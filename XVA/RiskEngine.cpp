#include "RiskEngine.h"

RiskEngine::RiskEngine(NettingSet netSet, RateCurve curve){
    xNetSet=netSet;
    xCurve=curve;
}

std::map<double,double> RiskEngine::calcIRDelta(){

    std::map<double,double> delta;

    return delta;
}