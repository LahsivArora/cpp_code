#include <iostream>
#include <vector>
#include <string>
#include "black_scholes.h"
#include "binomial_tree.h"

int main()
{
    // define params for stock
    double s = 56.0; // spot price
    double vol = 0.30; // volatility of stock of 25%
    double r = 0.05; // risk free rate of 3%
    double T = 1.5; // maturity in number of years
    int n = int(252.0 * T) ; // no of steps in binomial tree; taken as daily steps

    // generate tree
    generateTree tree(s,vol,T,n);
    std::vector<std::vector<double>> bintree = tree.get();

    // generate option price (European) comparison
    double K = 60.0;
    priceTree priceCall(bintree,K,"call",s,vol,T,n,r);
    priceTree pricePut(bintree,K,"put",s,vol,T,n,r);
    std::vector<std::vector<double>> optPriceTree_call = priceCall.get();
    std::vector<std::vector<double>> optPriceTree_put = pricePut.get();

    std::cout << "==========================================================" << std::endl;
    std::cout << "Call option price (binomial): " << optPriceTree_call[n-1][0] << std::endl;
    std::cout << "Put option price (binomial) : " << optPriceTree_put[n-1][0] << std::endl;

    // (double spot, double vol, double rate, double time, double strike, std::string optType)
    BlackScholes BS_call(s,vol,r,T,K,"call");
    BlackScholes BS_put(s,vol,r,T,K,"put");
    std::vector<double> optPriceBS_call = BS_call.price();
    std::vector<double> optPriceBS_put = BS_put.price();

    std::cout << "Call option price (black scholes): " << optPriceBS_call[0] << std::endl;
    std::cout << "Put option price (black scholes) : " << optPriceBS_put[0] << std::endl;

/*
    // generate option delta comparison
    double spotbump = 0.05; // bump less than 1% gives almost 0 gamma with binomial tree
    std::vector<std::vector<double>> bintreeD = generateTree(s*(1.0+spotbump),u,d,n);
    std::vector<std::vector<double>> optPriceTree_callD = optionPriceTree(bintreeD,p,K,"call",r,dT);
    std::vector<std::vector<double>> optPriceTree_putD = optionPriceTree(bintreeD,p,K,"put",r,dT);
    double delta1CallTree = (optPriceTree_callD[n-1][0]-optPriceTree_call[n-1][0])/(s*spotbump);
    double delta1PutTree = (optPriceTree_putD[n-1][0]-optPriceTree_put[n-1][0])/(s*spotbump);
    std::vector<double> optPriceBS_callD = optionPriceBS(s*(1.0+spotbump),vol,r,T,K,"call");
    std::vector<double> optPriceBS_putD = optionPriceBS(s*(1.0+spotbump),vol,r,T,K,"put");

    std::cout << "==========================================================" << std::endl;
    std::cout << "Call option delta (binomial): " << delta1CallTree << std::endl;
    std::cout << "Put option delta (binomial) : " << delta1PutTree << std::endl;
    std::cout << "Call option delta (BS - analytical): " << optPriceBS_call[1] << std::endl;
    std::cout << "Put option delta (BS - analytical) : " << optPriceBS_put[1] << std::endl;
    std::cout << "Call option delta (BS - bump&reval): " << (optPriceBS_callD[0]-optPriceBS_call[0])/(s*spotbump) << std::endl;
    std::cout << "Put option delta (BS - bump&reval) : " << (optPriceBS_putD[0]-optPriceBS_put[0])/(s*spotbump) << std::endl;


    // generate option vega comparison
    double volbump = 0.005;
    double volV = vol + volbump; // 1bp bump to Vol to calculate bump and reval Vega
    double uV = exp(volV*sqrt(dT));       // exp up move
    double dV = exp(-1.0*volV*sqrt(dT));  // exp down move
    double pV = (exp(r * dT) - dV)/(uV - dV); // prob of up move

    // binomial tree Vega calculation
    std::vector<std::vector<double>> bintreeV = generateTree(s,uV,dV,n);
    std::vector<std::vector<double>> optPriceTree_callV = optionPriceTree(bintreeV,pV,K,"call",r,dT);
    std::vector<std::vector<double>> optPriceTree_putV = optionPriceTree(bintreeV,pV,K,"put",r,dT);

    // Black Scholes Vega (bump and reval) calculation
    std::vector<double> optPriceBS_callV = optionPriceBS(s,volV,r,T,K,"call");
    std::vector<double> optPriceBS_putV = optionPriceBS(s,volV,r,T,K,"put");

    std::cout << "==========================================================" << std::endl;
    std::cout << "Call option Vega (binomial): " << (optPriceTree_callV[n-1][0]-optPriceTree_call[n-1][0])/volbump << std::endl;
    std::cout << "Put option Vega (binomial) : " << (optPriceTree_putV[n-1][0]-optPriceTree_put[n-1][0])/volbump << std::endl;

    std::cout << "Call option delta (BS - analytical): " << optPriceBS_call[2] << std::endl;
    std::cout << "Put option delta (BS - analytical) : " << optPriceBS_put[2] << std::endl;

    std::cout << "Call option delta (BS - bump&reval): " << (optPriceBS_callV[0]-optPriceBS_call[0])/volbump << std::endl;
    std::cout << "Put option delta (BS - bump&reval) : " << (optPriceBS_putV[0]-optPriceBS_put[0])/volbump << std::endl;

    // generate option gamma comparison: gamma unstable for binomial tree below 5% bump of spot
    std::cout << "==========================================================" << std::endl;
    std::vector<std::vector<double>> bintreeG = generateTree(s*(1.0-spotbump),u,d,n);
    std::vector<std::vector<double>> optPriceTree_callG = optionPriceTree(bintreeG,p,K,"call",r,dT);
    std::vector<std::vector<double>> optPriceTree_putG = optionPriceTree(bintreeG,p,K,"put",r,dT);
    double delta2CallTree = (optPriceTree_callG[n-1][0]-optPriceTree_call[n-1][0])/(-1.0*s*spotbump);
    double delta2PutTree = (optPriceTree_putG[n-1][0]-optPriceTree_put[n-1][0])/(-1.0*s*spotbump);

    std::vector<double> optPriceBS_callG = optionPriceBS(s*(1.0-spotbump),vol,r,T,K,"call");
    std::vector<double> optPriceBS_putG = optionPriceBS(s*(1.0-spotbump),vol,r,T,K,"put");

    std::cout << "Call option gamma (binomial): " << (delta1CallTree-delta2CallTree)/(s*spotbump) << std::endl;
    std::cout << "Put option gamma (binomial) : " << (delta1PutTree-delta2PutTree)/(s*spotbump) << std::endl;
    std::cout << "Call option gamma (BS - bump&reval): " << (optPriceBS_callD[0]+optPriceBS_callG[0]-2*optPriceBS_call[0])/(s*spotbump*s*spotbump) << std::endl;
    std::cout << "Put option gamma (BS - bump&reval) : " << (optPriceBS_putD[0]+optPriceBS_putG[0]-2*optPriceBS_put[0])/(s*spotbump*s*spotbump) << std::endl;
    std::cout << "Call option gamma (BS - analytical): " << optPriceBS_call[3] << std::endl;
    std::cout << "Put option gamma (BS - analytical) : " << optPriceBS_put[3] << std::endl;

    std::cout << " ==========================================================" << std::endl;
*/
    return 0;
}