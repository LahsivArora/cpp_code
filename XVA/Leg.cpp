#include "Leg.h"

Leg::Leg(){}

Leg::Leg(LegType type ,Currency ccy ,double freq, double rate){
    xType=type;
    xLegCcy=ccy;
    xFreq=freq;
    xRate=rate;
}

LegType Leg::getLegType(){
    return xType;
}

double Leg::getFixedRate(){
    if (xType == LegType::Fixed)
        return xRate;
    else
        return 0.0; 
}

double Leg::getFloatSpread(){
    if (xType == LegType::Float)
        return xRate;
    else
        return 0.0; 
}

double Leg::getFixedFreq(){
    if (xType == LegType::Fixed)
        return xFreq;
    else
        return 0.0; 
}

double Leg::getFloatFreq(){
    if (xType == LegType::Float)
        return xFreq;
    else
        return 0.0; 
}

std::vector<double> Leg::getLegFlows(double maturity){
    std::vector<double> flows;
    double n = maturity * xFreq;
    for (int i=0; i < n; i++){
        flows.push_back((i+1)/xFreq);
    }
    return flows;   
}
