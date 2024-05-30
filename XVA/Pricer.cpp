#include <vector>
#include "Pricer.h"
#include "Leg.h"

SwapPricer::SwapPricer(NettingSet& netSet, RateCurve& curve1, RateCurve& curve2, double FxSpot, double lag){
    xNetSet=netSet;
    xCurve1=curve1;
    xCurve2=curve2;
    xFxSpot=FxSpot;
    xLag=lag;
}

SwapPricer::SwapPricer(Swap& swap, RateCurve& curve1, RateCurve& curve2, double FxSpot, double lag){
    xSwap=swap;
    xSwaps.push_back(xSwap);
    xNetSet=NettingSet(xSwaps);
    xCurve1=curve1;
    xCurve2=curve2;
    xFxSpot=FxSpot;
    xLag=lag;
}

double SwapPricer::calcLegNPV(int legNum){
    double npv = 0.0;
    Leg calcLeg = xSwap.getLeg(legNum);
    double periodAdj = 1.0/calcLeg.getLegFreq();
    double notional = (legNum==1?1.0:-1.0)*xSwap.getNotional()*(legNum==1?1.0:xSwap.getEndFxFwd()); // add conversion for xccy
    double rate = calcLeg.getLegRate();
    std::vector<double> flow = calcLeg.getLegFlows(xSwap.getMaturity());
    RateCurve pricingCurve;
    if (calcLeg.getLegCurveName() == xCurve1.getName())
        pricingCurve = xCurve1;
    else if (calcLeg.getLegCurveName() == xCurve2.getName())
        pricingCurve = xCurve2;

    std::vector<double> xLegdisc = pricingCurve.getDiscFactors(flow);

    if (calcLeg.getLegType() == LegType::Fixed){
        for (unsigned int i=0; i < xLegdisc.size(); i++){
            if (flow[i] > xLag){
                npv += (notional * rate * periodAdj * xLegdisc[i]);
                if(i==0 && xSwap.getNotionalExch() == NotionalExch::YES){
                    npv += (-1.0*notional);}
                if(i+1==xLegdisc.size() && xSwap.getNotionalExch() == NotionalExch::YES){
                    npv += (notional * xLegdisc[i]);}
            }
        }
    }
    else if (calcLeg.getLegType() == LegType::Float){
        std::vector<double> xLegfwd = pricingCurve.getFwdRates(flow);
        for (unsigned int i=0; i < xLegdisc.size(); i++){
            if (flow[i] > xLag){
                npv += (notional * (xLegfwd[i] + rate) * periodAdj * xLegdisc[i]);
                if(i==0 && xSwap.getNotionalExch() == NotionalExch::YES){
                    npv += (-1.0*notional);}
                if(i+1==xLegdisc.size() && xSwap.getNotionalExch() == NotionalExch::YES){
                    npv += (notional * xLegdisc[i]);}
            }
        }
    }
    return npv;
}


double SwapPricer::calcTradeNPV(){

    double npv = 0.0;

    for (unsigned int i=0; i < xNetSet.getNoOfTrades(); i++){
        xSwap = xNetSet.getTrades()[i];
        if (xSwap.getTradeType() == TradeType::IrSwap)
            npv += this->calcLegNPV(1) + this->calcLegNPV(2);
        else if (xSwap.getTradeType() == TradeType::XccySwap)
            npv += this->calcLegNPV(1)*xFxSpot + this->calcLegNPV(2); // converting to Leg2 ccy. USD in this case
    }
        return npv;
}

double SwapPricer::calcFVA(RateCurve& fundCurve){
    fundCurve.nameTransform("USD.SOFR");
    SwapPricer *fundPV = new SwapPricer(xNetSet,fundCurve,fundCurve,1.0);
    return fundPV->calcTradeNPV() - this->calcTradeNPV();
}