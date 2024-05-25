#include <vector>
#include "RateCurve.h"
#include "Swap.h"
#include "Pricer.h"
#include "Exposure.h"

ExposureCalc::ExposureCalc(){}

ExposureCalc::ExposureCalc(Portfolio pfolio, std::vector<RateCurve> simCurves){
    xSimCurves=simCurves;
    xPfolio=pfolio;
}

std::vector<std::map<double,double>> ExposureCalc::calc(){

    std::vector<std::map<double,double>> output;
    std::map<double,double> EPEprofile;
    std::map<double,double> ENEprofile;
    double maxMaturity = xPfolio.getMaxMaturity()+1.0;
    double n = xSimCurves.size();
    double trades = xPfolio.getNoOfTrades();
    std::vector<VanillaSwap> tradeObjs = xPfolio.getTrades();

    // Exposure calc. profile matches expectations. using quarterly calc for exposures
    for (double i=0.25; i<maxMaturity; i+=0.25){
        double exposurePos = 0.0;
        double exposureNeg = 0.0;
        for (double k=0; k<trades; k++){
            for (int j=0; j<n; j++){
                SwapPricer priceLag(tradeObjs[k],xSimCurves[j],i);
                double positiveExposure = (priceLag.getTradeNPV()>0?priceLag.getTradeNPV():0);
                double negativeExposure = (priceLag.getTradeNPV()<0?priceLag.getTradeNPV():0);
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

std::map<double,double> ExposureCalc::getEEProfile(RiskType type){

    std::vector<std::map<double,double>> output = calc();
    if (type == RiskType::CTPY)
        return output[0];
    else
        return output[1];

}