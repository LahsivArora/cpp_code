#include <iostream>
#include <vector>
#include <cmath>
#include <bits/stdc++.h>

unsigned int NCR(unsigned int n, unsigned int r)
{
    if (r == 0) return 1;
    if (r > n / 2) return NCR(n, n - r); 

    unsigned int res = 1; 
    for (unsigned int k = 1; k <= r; ++k)
    {
        res *= n - k + 1;
        res /= k;
    }
    return res;
}

std::vector<std::vector<double>> generateTree (const double& spot,
                                               const double& u,
                                               const double& d,
                                               const double& noSteps) {

std::vector<std::vector<double>> tree;
for (int i=1; i <= noSteps; i++){
    std::vector<double> intd;
    for (int j=0; j < i; j++){
        double val = spot*pow(u,j)*pow(d,i-j-1) ;
        intd.push_back(val);
    }
    tree.push_back(intd);
}
return tree;
}

std::vector<std::vector<double>> optionPrice (const std::vector<std::vector<double>>& priceTree,
                                 const double& prob, const double& strike, const double& rate, const double& step) { 

std::vector<std::vector<double>> optPrices;
unsigned int steps = priceTree.size();
std::vector<double> dummy(steps, 0.0),fwdPrices, fwdPricesPrev=dummy;
unsigned int z = steps;

// backward induction
while (z > 0){
    double priceE, priceA;
    std::vector<double> optPricesN;
    fwdPrices = priceTree[z-1];
    //unsigned int nodes = fwdPrices.size();
    optPricesN.clear();

    for (unsigned int j=0; j < z; j++){
        priceA = (fwdPrices[j] < strike)? (strike - fwdPrices[j]): 0.0;
        priceE = (prob*fwdPricesPrev[j+1] + (1.0-prob)*fwdPricesPrev[j])*exp(-1.0*rate*step) ;
        //double optVal = (priceA > priceE)? priceA: priceE; 
        double optVal = (z==steps)? priceA: priceE; 
        optPricesN.push_back(optVal);
    }
    optPrices.push_back(optPricesN);
    fwdPricesPrev = optPricesN;
    z = z-1;
}
return optPrices;
}

double optionPriceE (const std::vector<std::vector<double>>& priceTree,
                                 const double& prob, const double& strike, const double& rate, const double& step) { 

double optPrice=0.0, total;
unsigned int steps = priceTree.size();
std::vector<double> treeNnode = priceTree.back();
total = pow(2,(steps-1));

// backward induction
for (unsigned int j=0; j < steps; j++){
    double priceA = (treeNnode[j] < strike)? (strike - treeNnode[j]): 0.0;
    unsigned int repeat = NCR((steps-1),j);
    //double priceE =  priceA*pow(prob,j)*pow((1.0-prob),steps-j-1)*exp(-1.0*rate*step*steps)*repeat ;
    double priceE =  priceA*exp(-1.0*rate*step*(steps-1))*(repeat/total) ;
    //std::cout << priceA << " " << repeat << " " << priceE << std::endl;
    optPrice += priceE; 
}
return optPrice;
}


int main()
{
    // define params for stock
    double s = 29.0; // spot price
    unsigned int n = 15; // no of steps in binomial tree
    double vol = 0.25; // volatility of stock of 25%
    double r = 0.05; // risk free rate of 3%
    double T = 1.0; // maturity in number of years

    // calculated params
    double dT = T/(n-1) ; // length of each time step
    double u = exp(vol*sqrt(dT));       // exp up move
    double d = exp(-1.0*vol*sqrt(dT));  // exp down move
    
    // generate tree
    std::vector<std::vector<double>> bintree = generateTree(s,u,d,n);
    std::vector<std::vector<double>> optPriceTree;
    double optPriceE;

    // generate put option price (American)
    double K = 30.0;
    double p = (exp(r * dT) - d)/(u - d); // prob of up move
    double r1 = log(0.5*(u-d)+d)/dT;
    std::cout << "rate: " << r << "; rate for 0.5 prob:" << r1 << std::endl;
    std::cout << "************************************************" << std::endl;
    optPriceTree = optionPrice(bintree,p,K,r,dT);
    std::cout << "option price (traversing tree): " << optPriceTree[n-1][0] << std::endl;
    optPriceE = optionPriceE(bintree,p,K,r,dT);
    std::cout << "option price(optimization): " << optPriceE << std::endl;
    std::cout << "price diff (in percent): " << (optPriceE-optPriceTree[n-1][0])/optPriceTree[n-1][0]*100.0 << std::endl;


    // recalc option prices for prob=0.5 and new rate r1
    // NOTE: the optimization only works for prob=0.5 
    // NOTE: for all other cases, we need to traverse tree to get European option price
    std::cout << "************************************************" << std::endl;
    optPriceTree = optionPrice(bintree,0.5,K,r1,dT);
    std::cout << "option price (traversing tree): " << optPriceTree[n-1][0] << std::endl;
    optPriceE = optionPriceE(bintree,0.5,K,r1,dT);
    std::cout << "option price(optimization): " << optPriceE << std::endl;
    std::cout << "price diff (in percent): " << (optPriceE-optPriceTree[n-1][0])/optPriceTree[n-1][0]*100.0 << std::endl;

/*
rate: 0.05; rate for 0.5 prob:0.0312268
************************************************
option price (traversing tree): 2.65931
option price(optimization): 2.89732
price diff (in percent): 8.95004
************************************************
option price (traversing tree): 2.95223
option price(optimization): 2.95223
price diff (in percent): -1.50425e-014
*/
    return 0;
}