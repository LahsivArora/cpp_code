#include <iostream>
#include <vector>
#include <string>
#include "pricing.h"

int main()
{
    // define params for stock
    double s = 56.0; // spot price
    double vol = 0.30; // volatility of stock of 25%
    double r = 0.05; // risk free rate of 3%
    double T = 1.5; // maturity in number of years
    int n = int(252.0 * T) ; // no of steps in binomial tree; taken as daily steps
    double K = 60.0;

    std::vector<double> prices = pricingOpt(s, vol, T, n, K, r);
    std::cout << "==========================================================" << std::endl;
    std::cout << "Call option price (binomial): " << prices[0] << std::endl;
    std::cout << "Put option price (binomial) : " << prices[1] << std::endl;
    std::cout << "Call option price (black scholes): " << prices[2] << std::endl;
    std::cout << "Put option price (black scholes) : " << prices[3] << std::endl;
    std::cout << "==========================================================" << std::endl;

    // generate option delta comparison
    double spotbump = 0.01; // bump less than 1% gives almost 0 gamma with binomial tree
    std::vector<double> pricesD = pricingOpt(s*(1.0+spotbump), vol, T, n, K, r);

    double delta1CallTree = (pricesD[0]-prices[0])/(s*spotbump);
    double delta1PutTree = (pricesD[1]-prices[1])/(s*spotbump);
    double delta1CallBS = (pricesD[2]-prices[2])/(s*spotbump);
    double delta1PutBS = (pricesD[3]-prices[3])/(s*spotbump);

    std::cout << "Call option delta (binomial): " << delta1CallTree << std::endl;
    std::cout << "Put option delta (binomial) : " << delta1PutTree << std::endl;
    std::cout << "Call option delta (BS - analytical): " << prices[4] << std::endl;
    std::cout << "Put option delta (BS - analytical) : " << prices[5] << std::endl;
    std::cout << "Call option delta (BS - bump&reval): " << delta1CallBS << std::endl;
    std::cout << "Put option delta (BS - bump&reval) : " << delta1PutBS << std::endl;
    std::cout << "==========================================================" << std::endl;

    // generate option vega comparison
    double volbump = 0.001;
    std::vector<double> pricesV = pricingOpt(s, vol+volbump, T, n, K, r);

    double vega1CallTree = (pricesV[0]-prices[0])/(volbump);
    double vega1PutTree = (pricesV[1]-prices[1])/(volbump);
    double vega1CallBS = (pricesV[2]-prices[2])/(volbump);
    double vega1PutBS = (pricesV[3]-prices[3])/(volbump);

    std::cout << "Call option Vega (binomial): " << vega1CallTree << std::endl;
    std::cout << "Put option Vega (binomial) : " << vega1PutTree << std::endl;
    std::cout << "Call option Vega (BS - analytical): " << prices[6] << std::endl;
    std::cout << "Put option Vega (BS - analytical) : " << prices[7] << std::endl;
    std::cout << "Call option Vega (BS - bump&reval): " << vega1CallBS << std::endl;
    std::cout << "Put option Vega (BS - bump&reval) : " << vega1PutBS << std::endl;
    std::cout << "==========================================================" << std::endl;

    // generate option Gamma comparison
    std::vector<double> pricesG = pricingOpt(s*(1.0-spotbump), vol, T, n, K, r);

    double delta2CallTree = (prices[0]-pricesG[0])/(s*spotbump);
    double delta2PutTree = (prices[1]-pricesG[1])/(s*spotbump);
    double delta2CallBS = (prices[2]-pricesG[2])/(s*spotbump);
    double delta2PutBS = (prices[3]-pricesG[3])/(s*spotbump);

    std::cout << "Call option delta (binomial): " << (delta1CallTree-delta2CallTree)/(s*spotbump) << std::endl;
    std::cout << "Put option delta (binomial) : " << (delta1PutTree-delta2PutTree)/(s*spotbump) << std::endl;
    std::cout << "Call option delta (BS - analytical): " << prices[8] << std::endl;
    std::cout << "Put option delta (BS - analytical) : " << prices[9] << std::endl;
    std::cout << "Call option delta (BS - bump&reval): " << (delta1CallBS-delta2CallBS)/(s*spotbump) << std::endl;
    std::cout << "Put option delta (BS - bump&reval) : " << (delta1PutBS-delta2PutBS)/(s*spotbump) << std::endl;
    std::cout << "==========================================================" << std::endl;

    return 0;
}