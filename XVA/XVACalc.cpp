#include "XVACalc.h"
#include "CDSCurve.h"
#include "Exposure.h"
#include "RiskEngine.h"

int XVACalc::counter = 0;

XVACalc::XVACalc(ExposureCalc* exposureObj, CDSCurve* curve, double LGD, RiskType type){
    xExposureObj=exposureObj;
    xCurve=curve;
    xLGD=LGD;
    xType=type;
    xNetSet=xExposureObj->getNettingSet();
    xBaseCurve=xExposureObj->getBaseCurve();
    ++counter;
}

double XVACalc::calcXVA(){
    double XVA = 0.0;
    std::map<double,double> marginalPDs = xCurve->calcMarginalPDs();
    std::map<double,double> xExposure = xExposureObj->calcEEProfile(xType);

    // calc CVA and DVA as sum(EE*marginal PD*LGD)
    for (double j=0.25; j<=xExposure.size()*0.25; j+=0.25){
            XVA += xExposure[j]*marginalPDs[j]*xLGD ;
    }
    return XVA;
}

double XVACalc::calcRWA(){
    // calculation based on SA-CCR (standardized approach for counterparty credit risk)
    double riskWeight = 0.5; // assuming 50% risk weight for a BBB+/BBB- bank counterparty
    double RWA = xExposureObj->calcEAD() * riskWeight; // 
    return RWA;
}

double XVACalc::calcInitialMargin(MarketData* mktData){
    // calculation based on SIMM (standardized initial margin model) by ISDA
    double IM = 0.0;
    std::vector<double> SIMMtemplate = {0.038462,0.083333,0.25,0.5,1.0,2.0,3.0,5.0,10.0};
    RateCurve* SIMMCurve = new RateCurve;
    SIMMCurve = xBaseCurve->templateTransform(SIMMtemplate);

    MarketData* transformMktData = new MarketData;
    transformMktData = mktData->createBumpedMarktData(SIMMCurve);

    RiskEngine riskCalc(xNetSet, transformMktData, xBaseCurve->getName());
    std::map<double,double> irDelta = riskCalc.calcIRDelta();

    delete transformMktData;
    return IM;
}
