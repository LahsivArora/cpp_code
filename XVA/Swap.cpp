#include <vector>
#include "Swap.h"

VanillaSwap::VanillaSwap(){}

VanillaSwap::VanillaSwap(TradeType type,double maturity, double notional, std::vector<double> Leg1, std::vector<double> Leg2){
    xType=type;
    xmaturity=maturity;
    xnotional=notional;
    xLeg1=Leg1;
    xLeg2=Leg2;
}

std::vector<double> VanillaSwap::getLegFlows(int j){
    std::vector<double> flows;
    double freq;

    if (j == 1) 
        freq = xLeg1[0];
    else
        freq = xLeg2[0];

    double n = xmaturity * freq;
    for (int i=0; i < n; i++){
        flows.push_back((i+1)/freq);
    }
    return flows;
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

double VanillaSwap::getFixedRate(){
    return xLeg1[1];
}

double VanillaSwap::getFloatSpread(){
    return xLeg2[1];
}

double VanillaSwap::getFixedFreq(){
    return xLeg1[0];
}

double VanillaSwap::getFloatFreq(){
    return xLeg2[0];
}