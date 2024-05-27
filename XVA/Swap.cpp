#include <vector>
#include <cmath>
#include "Swap.h"

Swap::Swap(){}

Swap::Swap(TradeType type,double maturity, double notional, Leg& Leg1, Leg& Leg2, NotionalExch exch, double endFxFwd){
    xType=type;
    xmaturity=maturity;
    xnotional=notional;
    xLeg1=Leg1;
    xLeg2=Leg2;
    xExch=exch;
    xEndFxFwd=endFxFwd;
}

double Swap::getEndFxFwd(){
    return xEndFxFwd;
}

Leg Swap::getLeg(int legNum){
    if (legNum == 1)
        return xLeg1;
    else
        return xLeg2;
}

TradeType Swap::getTradeType(){
    return xType;
}

double Swap::getMaturity(){
    return xmaturity;
}

double Swap::getNotional(){
    return xnotional;
}

double Swap::getAdjNotional(){
    return xnotional * (1.0-exp(-0.05*getMaturity()))/0.05;
}

double Swap::getRiskHorizon(){
    // margin period is min(maturity,1Y) for non-CSA/uncollateralized counterparty
    double marginPeriod = (getMaturity()<1.0?getMaturity():1.0);  
    return sqrt(marginPeriod);
}
