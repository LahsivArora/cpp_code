#include <vector>
#include <cmath>
#include "RateCurve.h"
#include "Swap.h"
#include "Pricer.h"
#include "Exposure.h"
#include "RiskEngine.h"

int ExposureCalc::counter = 0;

ExposureCalc::ExposureCalc(NettingSet* netSet, std::vector<RateCurve *>* simCurves){
    xSimCurves=simCurves;
    xNetSet=netSet;
    ++counter;
}

NettingSet* ExposureCalc::getNettingSet(){
    return this->xNetSet;
}

RateCurve* ExposureCalc::getBaseCurve(){
    return *(xSimCurves->begin());
}

std::vector<std::map<double,double>> ExposureCalc::calc(){

    std::vector<std::map<double,double>> output;
    std::map<double,double> EPEprofile;
    std::map<double,double> ENEprofile;
    double maxMaturity = xNetSet->getMaxMaturity()+1.0;
    double n = xSimCurves->size();
    std::vector<Swap *>* tradeObjs = xNetSet->getTrades();

    // Exposure calc. profile matches expectations. using quarterly calc for exposures
    for (double i=0.25; i<maxMaturity; i+=0.25){
        double exposurePos = 0.0;
        double exposureNeg = 0.0;
        for (auto it1 = tradeObjs->begin(); it1 != tradeObjs->end(); it1++){
            for (auto it2 = xSimCurves->begin(); it2 != xSimCurves->end(); it2++){
                SwapPricer *priceLag = new SwapPricer(*it1,*it2,*it2,1.0,i);
                double positiveExposure = (priceLag->calcTradeNPV()>0?priceLag->calcTradeNPV():0);
                double negativeExposure = (priceLag->calcTradeNPV()<0?priceLag->calcTradeNPV():0);
                delete priceLag;
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
    else if (type == RiskType::OWN)
        return output[1];
    else 
        throw std::string("Risk type is not supported");
}

double ExposureCalc::calcEAD(){

    double EAD = 0.0;
    std::vector<Swap *>* trades = xNetSet->getTrades();
    RateCurve* basecurve = getBaseCurve();

    SwapPricer *basePV = new SwapPricer(xNetSet,basecurve, basecurve,1.0);
    double netSetPV = basePV->calcTradeNPV();
    double replacementCost = (netSetPV>0.0?netSetPV:0.0);
    delete basePV;

    double Dsub1Y = 0.0;
    double D1Y5Y = 0.0;
    double Dabove5Y = 0.0; // define buckets by maturity of swaps
    double PFE = 0.0;

    for (auto it = trades->begin(); it != trades->end(); it++) {
        Swap* current = *it ;
        RiskEngine trade(current, basecurve);
        double D = trade.calcRWADelta() * current->getRiskHorizon() * current->getAdjNotional();

        if (current->getMaturity() <= 1.0)
            Dsub1Y += D;
        else if (current->getMaturity() > 1.0 && current->getMaturity() <= 5.0)
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