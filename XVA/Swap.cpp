#include <vector>
#include <cmath>
#include "Swap.h"

VanillaSwap::VanillaSwap(){}

VanillaSwap::VanillaSwap(TradeType type,double maturity, double notional, Leg& Leg1, Leg& Leg2, NotionalExch exch, double endFxFwd){
    xType=type;
    xmaturity=maturity;
    xnotional=notional;
    xLeg1=Leg1;
    xLeg2=Leg2;
    xExch=exch;
    xEndFxFwd=endFxFwd;
}

double VanillaSwap::getEndFxFwd(){
    return xEndFxFwd;
}

Leg VanillaSwap::getLeg(int legNum){
    if (legNum == 1)
        return xLeg1;
    else
        return xLeg2;
}

TradeType VanillaSwap::getTradeType(){
    return xType;
}

double VanillaSwap::getMaturity(){
    return xmaturity;
}

double VanillaSwap::getNotional(){
    return xnotional;
}

double VanillaSwap::getAdjNotional(){
    return xnotional * (1.0-exp(-0.05*getMaturity()))/0.05;
}

double VanillaSwap::getRiskHorizon(){
    // margin period is min(maturity,1Y) for non-CSA/uncollateralized counterparty
    double marginPeriod = (getMaturity()<1.0?getMaturity():1.0);  
    return sqrt(marginPeriod);
}
