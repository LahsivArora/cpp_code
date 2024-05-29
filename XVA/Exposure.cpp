#include <vector>
#include <cmath>
#include "RateCurve.h"
#include "Swap.h"
#include "Pricer.h"
#include "Exposure.h"
#include "RiskEngine.h"

ExposureCalc::ExposureCalc(){}

ExposureCalc::ExposureCalc(NettingSet& netSet, std::vector<RateCurve>& simCurves){
    xSimCurves=simCurves;
    xNetSet=netSet;
}

NettingSet ExposureCalc::getNettingSet(){
    return xNetSet;
}

RateCurve ExposureCalc::getBaseCurve(){
    return xSimCurves[0];
}

std::vector<std::map<double,double>> ExposureCalc::calc(){

    std::vector<std::map<double,double>> output;
    std::map<double,double> EPEprofile;
    std::map<double,double> ENEprofile;
    double maxMaturity = xNetSet.getMaxMaturity()+1.0;
    double n = xSimCurves.size();
    unsigned int trades = xNetSet.getNoOfTrades();
    std::vector<Swap> tradeObjs = xNetSet.getTrades();

    // Exposure calc. profile matches expectations. using quarterly calc for exposures
    for (double i=0.25; i<maxMaturity; i+=0.25){
        double exposurePos = 0.0;
        double exposureNeg = 0.0;
        for (unsigned int k=0; k<trades; k++){
            for (unsigned int j=0; j<n; j++){
                SwapPricer priceLag(tradeObjs[k],xSimCurves[j],xSimCurves[j],1.0,i);
                double positiveExposure = (priceLag.calcTradeNPV()>0?priceLag.calcTradeNPV():0);
                double negativeExposure = (priceLag.calcTradeNPV()<0?priceLag.calcTradeNPV():0);
                exposurePos += positiveExposure;
                exposureNeg += negativeExposure;
            }
        }
        EPEprofile.insert(std::pair<double,double>(i,exposurePos/n));        
        ENEprofile.insert(std::pair<double,double>(i,-1.0*exposureNeg/n));        
    }
    output.push_back(EPEprofile);
    output.push_back(ENEprofile);

    return output;
}

std::map<double,double> ExposureCalc::calcEEProfile(RiskType type){

    std::vector<std::map<double,double>> output = calc();
    if (type == RiskType::CTPY)
        return output[0];
    else
        return output[1];

}

double ExposureCalc::calcEAD(){

    double EAD = 0.0;
    std::vector<Swap> trades = xNetSet.getTrades();
    RateCurve basecurve = getBaseCurve();

    SwapPricer basePV(xNetSet,basecurve, basecurve,1.0);
    double netSetPV = basePV.calcTradeNPV();
    double replacementCost = (netSetPV>0.0?netSetPV:0.0);

    double Dsub1Y = 0.0;
    double D1Y5Y = 0.0;
    double Dabove5Y = 0.0; // define buckets by maturity of swaps
    double PFE = 0.0;

    for (auto it = trades.begin(); it != trades.end(); it++) {
        RiskEngine trade(*it, basecurve);
        double D = trade.calcRWADelta() * it->getRiskHorizon() * it->getAdjNotional();

        if (it->getMaturity() <= 1.0)
            Dsub1Y += D;
        else if (it->getMaturity() > 1.0 && it->getMaturity() <= 5.0)
            D1Y5Y += D;
        else
            Dabove5Y += D;
    }
    PFE = sqrt(pow(Dsub1Y,2) + pow(D1Y5Y,2) + pow(Dabove5Y,2) + 1.4*Dsub1Y*D1Y5Y + 1.4*D1Y5Y*Dabove5Y + 0.6*Dsub1Y*Dabove5Y);
    double addOn = 0.005 * PFE; // supervisory factor for IRS is 0.5%

    // EAD = alpha * (Replacement Cost + multiplier * AddOn)
    EAD = 1.4 * (replacementCost + 1.0 * addOn);
    return EAD;
}