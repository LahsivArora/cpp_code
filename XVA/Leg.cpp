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

double Leg::getLegRate(){
    return xRate;
}

double Leg::getLegFreq(){
    return xFreq;
}

std::vector<double> Leg::getLegFlows(double maturity){
    std::vector<double> flows;
    double n = maturity * xFreq;
    for (int i=0; i < n; i++){
        flows.push_back((i+1)/xFreq);
    }
    return flows;   
}
