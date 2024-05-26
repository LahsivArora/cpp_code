#include <iostream>
#include <vector>
#include <cmath>
#include "RateCurve.h"
#include "Swap.h"
#include "Pricer.h"
#include "SimModel.h"
#include "Exposure.h"
#include "CDSCurve.h"
#include "NettingSet.h"
#include "Enums.h"
#include "XVACalc.h"
#include "RiskEngine.h"


int main()
{
    // Step1a: defining portfolio of swaps
    // +ive notional means receive fixed-pay float and 4 means Quarterly payments
    VanillaSwap Swap1(7,-1500000.0, {4.0,0.0480},{4.0,-0.0025});
    VanillaSwap Swap2(4.5,1000000.0, {4.0,0.0500},{4.0,-0.0025});
    VanillaSwap Swap3(8.5,1500000.0, {4.0,0.0475},{4.0,-0.0025});
    VanillaSwap Swap4(2.5,-1000000.0, {4.0,0.0480},{4.0,-0.0025});

    // Step1b: define portfolio with 3 swaps 
    std::vector<VanillaSwap> trades;
    trades.push_back(Swap1);
    trades.push_back(Swap2);
    trades.push_back(Swap3);
    trades.push_back(Swap4);
    NettingSet netSet(trades); 

    // Step2: defining rate curve with pillars as 6m, 1Y, 2Y, 5Y, 7Y, 10Y. For now using zero rates
    // curve has hump at 2Y point i.e. slowly downward sloping from 2Y to 10Y
    RateCurve SOFR({{0.5,0.0225},{1.0,0.0375},{2.0,0.05},{5.0,0.049},{7.0,0.0475},{10.0,0.045}});
    RateCurve FundingCurve({{0.5,0.025},{1.0,0.04},{2.0,0.0525},{5.0,0.0515},{7.0,0.0525},{10.0,0.05}}); //25bp upto 5y; 50bp thereafter

    // Step3: pricing netting set (with Swaps and RateCurve objects) 
    double netSetbasePV = netSet.getTradesNPV(SOFR);
    double netSetFundPV = netSet.getTradesNPV(FundingCurve);
    RiskEngine riskSet(netSet, SOFR);
    std::map<double,double> irDelta = riskSet.calcIRDelta();

    for (auto it=irDelta.begin(); it != irDelta.end(); it++){
        std::cout << "Tenor:" << it->first << " irDelta:" << it->second << std::endl;
    }

    // Step4: simulate curves using base RateCurve object + simulation params. using HW model
    double rateVol = 0.15; // i.e. 15% annual vol. constant for now.
    double simPaths = 5; // hardcoding simulation for now
    double a = 0.05; // speed of mean reversion 
    SimulateRate simEngine(SOFR, rateVol, a, simPaths);
    std::vector<RateCurve> simCurves = simEngine.getSimulatedCurves();
    std::vector<double> simNPVs = simEngine.getSimulatedBaseNPVs(Swap1);

    // Step5: generate exposure profile using Swap and Simulated curves
    // assuming Quarterly time steps in exposure calculation; exposure is already discounted to today
    ExposureCalc netSetExProfile(netSet,simCurves);
    std::map<double,double> EPEprofile = netSetExProfile.getEEProfile(RiskType::CTPY);
    std::map<double,double> ENEprofile = netSetExProfile.getEEProfile(RiskType::OWN);

    // Step6: create CDS curve with marginal default probabilities assuming contant hazard rate    
    double timesteps = 0.25; // quarterly steps to match exposure profile
    double maxmaturity = 10.0; // generate marginal PDs upto 10Y

    double ctpyCDSSpread = 300.0; // assuming constant CDS spread of 250bps for counterparty
    double ctpyLGD = (1.0-0.6); // loss given default (LGD) assuming recovery rate (RR) is 60%
    CDSCurve ctpyCDS(ctpyCDSSpread,ctpyLGD,maxmaturity,timesteps);

    double ownLGD = (1.0-0.8); // loss given default (LGD) assuming recovery rate (RR) is 80%
    double ownCDSSpread = 150.0; // assuming constant CDS spread of 150bps for own
    CDSCurve ownCDS(ownCDSSpread,ownLGD,maxmaturity,timesteps);

    // Step7: calculate CVA, DVA and RWA 
    XVACalc CVA(EPEprofile,ctpyCDS,ctpyLGD);
    XVACalc DVA(ENEprofile,ownCDS,ownLGD);

    std::cout << "For given netting set and market data (all XVA are in $ amount):" << std::endl;
    std::cout << "FVA (+ive means charge to client):" << netSetbasePV-netSetFundPV << std::endl;
    std::cout << "CVA (+ive means charge to client):" << CVA.calcXVA() << std::endl;
    std::cout << "DVA (+ive means benefit to bank):" << DVA.calcXVA() << std::endl;
    std::cout << "RWA (using SA-CCR):" << CVA.calcRWA() << std::endl; 

    return 0;
}