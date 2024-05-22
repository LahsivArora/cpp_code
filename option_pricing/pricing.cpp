#include <vector>
#include <string>
#include "black_scholes.h"
#include "binomial_tree.h"
#include "pricing.h"

std::vector<double> pricingOpt(double s, double vol, double T, int n, double K, double r){
    std::vector<double> prices;

    // generate tree
    generateTree tree(s,vol,T,n);
    std::vector<std::vector<double>> bintree = tree.get();

    // generate option price (European) comparison
    priceTree priceCall(bintree,K,"call",s,vol,T,n,r);
    priceTree pricePut(bintree,K,"put",s,vol,T,n,r);
    std::vector<std::vector<double>> optPriceTree_call = priceCall.get();
    std::vector<std::vector<double>> optPriceTree_put = pricePut.get();

    BlackScholes BS_call(s,vol,r,T,K,"call");
    BlackScholes BS_put(s,vol,r,T,K,"put");
    std::vector<double> optPriceBS_call = BS_call.price();
    std::vector<double> optPriceBS_put = BS_put.price();

    prices.push_back(optPriceTree_call[n-1][0]);
    prices.push_back(optPriceTree_put[n-1][0]);
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