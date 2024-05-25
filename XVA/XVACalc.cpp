#include "XVACalc.h"
#include "CDSCurve.h"
#include "Exposure.h"

XVACalc::XVACalc(std::map<double,double> exposure, CDSCurve curve, double LGD){
    xExposure=exposure;
    xCurve=curve;
    xLGD=LGD;
}

double XVACalc::calcXVA(){
    double XVA;
    std::map<double,double> marginalPDs = xCurve.getMarginalPDs();

    // calc CVA and DVA as sum(EE*marginal PD*LGD)
    for (double j=0.25; j<=xExposure.size()*0.25; j+=0.25){
            XVA += xExposure[j]*marginalPDs[j]*xLGD ;
    }
    return XVA;
}

double XVACalc::calcRWA(){
    // calculation based on SA-CCR (standardized approach for counterparty credit risk)
    double RWA = 0.0;

    return RWA;
}


double XVACalc::calcInitialMargin(){
    // calculation based on SIMM (standardized initial margin model) by ISDA
    double IM = 0.0;

    return IM;
}
