#include <vector>
#include <string>
#include "black_scholes.h"
#include "binomial_tree.h"
#include "pricing.h"


PricingOpt::PricingOpt(double s, double vol, double T, unsigned int n, double K, double r){
    xSpot=s;
    xVol=vol;
    xTime=T;
    xSteps=n;
    xStrike=K;
    xRate=r;
}

std::vector<double> PricingOpt::calcPrice(){
    std::vector<double> prices;

    // generate tree
    generateTree tree(xSpot,xVol,xTime,xSteps);
    std::vector<std::vector<double>> bintree = tree.get();

//    priceTree (std::vector<std::vector<double>> genTree, double strike, std::string optType,
//                double spot, double vol, double time, unsigned int nSteps, double rate);

    // generate option price (European) comparison
    priceTree priceCall(bintree,xStrike,OptType::CALL,xSpot,xVol,xTime,xSteps,xRate);
    priceTree pricePut(bintree,xStrike,OptType::PUT,xSpot,xVol,xTime,xSteps,xRate);
    std::vector<std::vector<double>> optPriceTree_call = priceCall.calc();
    std::vector<std::vector<double>> optPriceTree_put = pricePut.calc();

    BlackScholes BS_call(xSpot,xVol,xRate,xTime,xStrike,OptType::CALL);
    BlackScholes BS_put(xSpot,xVol,xRate,xTime,xStrike,OptType::PUT);
    std::vector<double> optPriceBS_call = BS_call.price();
    std::vector<double> optPriceBS_put = BS_put.price();

    prices.push_back(optPriceTree_call[xSteps-1][0]);
    prices.push_back(optPriceTree_put[xSteps-1][0]);
    prices.push_back(optPriceBS_call[0]);
    prices.push_back(optPriceBS_put[0]);
    // Delta (analytical)
    prices.push_back(optPriceBS_call[1]);
    prices.push_back(optPriceBS_put[1]);
    // Vega (analytical)
    prices.push_back(optPriceBS_call[2]);
    prices.push_back(optPriceBS_put[2]);
    // Gamma (analytical)
    prices.push_back(optPriceBS_call[3]);
    prices.push_back(optPriceBS_put[3]);

    return prices;
}