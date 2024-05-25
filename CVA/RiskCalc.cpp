#include "RiskCalc.h"
#include "CDSCurve.h"
#include "Exposure.h"

RiskCalc::RiskCalc(std::map<double,double> exposure, CDSCurve curve, double LGD){
    xExposure=exposure;
    xCurve=curve;
    xLGD=LGD;
}

double RiskCalc::CalcXVA(){

    double XVA;
    std::map<double,double> marginalPDs = xCurve.getMarginalPDs();

    // calc CVA and DVA as sum(EE*marginal PD*LGD)
    for (double j=0.25; j<=xExposure.size()*0.25; j+=0.25){
            XVA += xExposure[j]*marginalPDs[j]*xLGD ;
    }

    return XVA;

}
