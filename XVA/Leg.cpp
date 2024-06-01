#include "Leg.h"

Leg::Leg(LegType type ,Currency ccy ,double freq, double rate, std::string curveName){
    xType=type;
    xLegCcy=ccy;
    xFreq=freq;
    xRate=rate;
    xCurveName=curveName;
}

std::string Leg::getLegCurveName(){
    return this->xCurveName;
}

LegType Leg::getLegType(){
    return this->xType;
}

double Leg::getLegRate(){
    return this->xRate;
}

double Leg::getLegFreq(){
    return this->xFreq;
}

std::vector<double> Leg::getLegFlows(double maturity){
    std::vector<double> flows;
    double n = maturity * this->xFreq;
    for (int i=0; i < n; i++){
        flows.push_back((i+1)/this->xFreq);
    }
    return flows;   
}
