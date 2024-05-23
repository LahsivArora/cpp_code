#include <vector>
#include "RateCurve.h"
#include "Swap.h"
#include "Pricer.h"
#include "Exposure.h"


ExposureCalc::ExposureCalc(Portfolio pfolio, std::vector<RateCurve> simCurves){
    xSimCurves=simCurves;
    xPfolio=pfolio;
}

std::map<double,double> ExposureCalc::getEEProfile(){

    std::map<double,double> EEprofile;
    double maxMaturity = xPfolio.getMaxMaturity()+1.0;
    double n = xSimCurves.size();
    double trades = xPfolio.getNoOfTrades();
    std::vector<VanillaSwap> tradeObjs = xPfolio.getTrades();

    // Exposure calc. profile matches expectations. using quarterly calc for exposures
    for (double i=0.25; i<maxMaturity; i+=0.25){
        double exposure = 0.0;
        for (double k=0; k<trades; k++){
            for (int j=0; j<n; j++){
                SwapPricer priceLag(tradeObjs[k],xSimCurves[j],i);
                double positiveExposure = (priceLag.getTradeNPV()>0?priceLag.getTradeNPV():0);
                exposure += positiveExposure;
            }
        }
        EEprofile.insert(std::pair<double,double>(i,exposure/n));        
    }
    return EEprofile;
}