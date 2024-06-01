#include <vector>
#include <cmath>
#include "Swap.h"

int Swap::counter = 0;

Swap::Swap(TradeType type,double maturity, double notional, Leg& Leg1, Leg& Leg2, NotionalExch exch, double endFxFwd) {
    xType=type;
    xmaturity=maturity;
    xnotional=notional;
    xLeg1=Leg1;
    xLeg2=Leg2;
    xExch=exch;
    xEndFxFwd=endFxFwd;
    counter++;
}

NotionalExch Swap::getNotionalExch(){
    return this->xExch;
}

double Swap::getEndFxFwd(){
    return this->xEndFxFwd;
}

Leg Swap::getLeg(int legNum){
    if (legNum == 1)
        return this->xLeg1;
    else
        return this->xLeg2;
}

TradeType Swap::getTradeType(){
    return this->xType;
}

double Swap::getMaturity(){
    return this->xmaturity;
}

double Swap::getNotional(){
    return this->xnotional;
}

double Swap::getAdjNotional(){
    return this->xnotional * (1.0-exp(-0.05*this->getMaturity()))/0.05;
}

double Swap::getRiskHorizon(){
    // margin period is min(maturity,1Y) for non-CSA/uncollateralized counterparty
    double marginPeriod = (this->getMaturity()<1.0?this->getMaturity():1.0);  
    return sqrt(marginPeriod);
}
