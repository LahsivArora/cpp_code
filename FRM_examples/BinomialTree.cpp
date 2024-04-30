#include <iostream>
#include <vector>
#include <cmath>
#include <bits/stdc++.h>

int NCR(int n, int r)
{
    if (r == 0) return 1;
    if (r > n / 2) return NCR(n, n - r); 

    long res = 1; 
    for (int k = 1; k <= r; ++k)
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
};

std::vector<std::vector<double>> optionPrice (const std::vector<std::vector<double>>& priceTree,
                                 const double& prob, const double& strike, const double& rate, const double& step) { 

std::vector<std::vector<double>> optPrices;
int steps = priceTree.size();
std::vector<double> dummy(steps, 0.0),fwdPrices, fwdPricesPrev=dummy;
int z = steps;

// backward induction
while (z > 0){
    double priceE, priceA;
    std::vector<double> optPricesN;
    fwdPrices = priceTree[z-1];
    int nodes = fwdPrices.size();
    optPricesN.clear();

    for (int j=0; j < z; j++){
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
};

double optionPriceE (const std::vector<std::vector<double>>& priceTree,
                                 const double& prob, const double& strike, const double& rate, const double& step) { 

double optPrice=0.0, total;
int steps = priceTree.size();
std::vector<double> treeNnode = priceTree.back();
total = pow(2.0,(steps-1));

// backward induction
for (int j=0; j < steps; j++){
    double priceA = (treeNnode[j] < strike)? (strike - treeNnode[j]): 0.0;
    double repeat = NCR((steps-1),j);
    //double priceE =  priceA*pow(prob,j)*pow((1.0-prob),steps-j-1)*exp(-1.0*rate*step*steps)*repeat ;
    double priceE =  priceA*exp(-1.0*rate*step*(steps-1))*(repeat/total) ;
    //std::cout << priceA << " " << repeat << " " << priceE << std::endl;
    optPrice += priceE; 
}
return optPrice;
};


int main()
{
    // define params for stock
    double s = 29.0; // spot price
    int n = 15; // no of steps in binomial tree
    double vol = 0.25; // volatility of stock of 25%
    double r = 0.03; // risk free rate of 3%
    double T = 1.0; // maturity in number of years

    // calculated params
    double dT = T/(n-1) ; // length of each time step
    double u = exp(vol*sqrt(dT));       // exp up move
    double d = exp(-1.0*vol*sqrt(dT));  // exp down move
    
    // generate tree
    std::vector<std::vector<double>> bintree = generateTree(s,u,d,n);

    // generate put option price (American)
    double K = 22.0;
    double p = (exp(r * dT) - d)/(u - d); // prob of up move
    std::vector<std::vector<double>> optPriceTree = optionPrice(bintree,p,K,r,dT);
    std::cout << "option price: " << optPriceTree[n-1][0] << std::endl;

    double optPriceE = optionPriceE(bintree,p,K,r,dT);
    std::cout << "option price: " << optPriceE << std::endl;

    return 0;
}