#include "XVACalc.h"
#include "CDSCurve.h"
#include "Exposure.h"

XVACalc::XVACalc(ExposureCalc &exposureObj, CDSCurve curve, double LGD, RiskType type){
    xExposureObj=exposureObj;
    xCurve=curve;
    xLGD=LGD;
    xType=type;
}

double XVACalc::calcXVA(){
    double XVA = 0.0;
    std::map<double,double> marginalPDs = xCurve.getMarginalPDs();
    std::map<double,double> xExposure = xExposureObj.calcEEProfile(xType);

    // calc CVA and DVA as sum(EE*marginal PD*LGD)
    for (double j=0.25; j<=xExposure.size()*0.25; j+=0.25){
            XVA += xExposure[j]*marginalPDs[j]*xLGD ;
    }
    return XVA;
}

double XVACalc::calcRWA(){
    // calculation based on SA-CCR (standardized approach for counterparty credit risk)
    double riskWeight = 0.5; // assuming 50% risk weight for a BBB+/BBB- bank counterparty
    double RWA = xExposureObj.calcEAD() * riskWeight; // 
    return RWA;
}


double XVACalc::calcInitialMargin(){
    // calculation based on SIMM (standardized initial margin model) by ISDA
    double IM = 0.0;

    return IM;
}
