#include <vector>
#include "Pricer.h"

int SwapPricer::counter = 0;

SwapPricer::SwapPricer(NettingSet* netSet, MarketData* mktData, double lag){
    xNetSet=netSet;
    xMktData=mktData;
    xCurves=xMktData->getRateCurves();
    xFxSpot=xMktData->getFxSpots()->front();
    xLag=lag;
    ++counter;
}

SwapPricer::SwapPricer(Swap* swap, MarketData* mktData, double lag){
    xSwap=swap;
    xSwaps= new std::vector<Swap *>;
    xSwaps->push_back(xSwap);
    xNetSet= new NettingSet(xSwaps);
    xMktData=mktData;
    xCurves=xMktData->getRateCurves();
    xFxSpot=xMktData->getFxSpots()->front();
    xLag=lag;
    ++counter;
}

double SwapPricer::calcLegNPV(int legNum){
    double npv = 0.0;
    Leg* calcLeg = xSwap->getLeg(legNum);
    double periodAdj = 1.0/calcLeg->getLegFreq();
    double notional = (legNum==1?1.0:-1.0)*xSwap->getNotional()*(legNum==1?1.0:xSwap->getEndFxFwd()); // add conversion for xccy
    double rate = calcLeg->getLegRate();
    std::vector<double> flow = calcLeg->getLegFlows(xSwap->getMaturity());
    RateCurve* pricingCurve; 

    if (calcLeg->getLegCurveName().size() == 0)
        throw std::string("Leg curveName: is empty");
    pricingCurve = xMktData->getRateCurve(calcLeg->getLegCurveName());
    if (pricingCurve->getName().size() == 0)
        throw std::string("Leg curveName:"+calcLeg->getLegCurveName()+" doesnt match curves in marketdata");

    std::vector<double> xLegdisc = pricingCurve->getDiscFactors(flow);
    std::vector<double> xLegfwd = pricingCurve->getFwdRates(flow);

    if (calcLeg->getLegType() == LegType::Fixed){
        for (unsigned int i=0; i < xLegdisc.size(); i++){
            if (flow[i] > xLag){
                npv += (notional * rate * periodAdj * xLegdisc[i]);
                if(i==0 && xSwap->getNotionalExch() == NotionalExch::YES){
                    npv += (-1.0*notional);}
                if(i+1==xLegdisc.size() && xSwap->getNotionalExch() == NotionalExch::YES){
                    npv += (notional * xLegdisc[i]);}
            }
        }
    }
    else if (calcLeg->getLegType() == LegType::Float){
        for (unsigned int i=0; i < xLegdisc.size(); i++){
            if (flow[i] > xLag){
                npv += (notional * (xLegfwd[i] + rate) * periodAdj * xLegdisc[i]);
                if(i==0 && xSwap->getNotionalExch() == NotionalExch::YES){
                    npv += (-1.0*notional);}
                if(i+1==xLegdisc.size() && xSwap->getNotionalExch() == NotionalExch::YES){
                    npv += (notional * xLegdisc[i]);}
            }
        }
    }
    else
        throw std::string("Leg type is not supported");

    return npv;
}


double SwapPricer::calcTradeNPV(){

    double npv = 0.0;

    for (auto it = (xNetSet->getTrades())->begin(); it != (xNetSet->getTrades())->end(); it++){
        xSwap = *it;
        if (xSwap->getTradeType() == TradeType::IrSwap)
            npv += this->calcLegNPV(1) + this->calcLegNPV(2);
        else if (xSwap->getTradeType() == TradeType::XccySwap)
            npv += this->calcLegNPV(1)*(*xFxSpot) + this->calcLegNPV(2); // converting to Leg2 ccy. USD in this case
        else
            throw std::string("TradeType is not supported");
    }
    return npv;
}

double SwapPricer::calcFVA(RateCurve& fundCurve){
    RateCurve *FVACurve = new RateCurve;
    *FVACurve = fundCurve.nameTransform("USD.SOFR");
    MarketData* replaceMktData = new MarketData;
    replaceMktData = xMktData->replaceRateCurve(FVACurve);
    SwapPricer fundPV(xNetSet,replaceMktData);
    return fundPV.calcTradeNPV() - calcTradeNPV();
}