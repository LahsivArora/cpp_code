#include <iostream>
#include <vector>
#include <cmath>

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
                                 const double& prob, const double& strike) { 

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
        priceE = prob*fwdPricesPrev[j+1] + (1.0-prob)*fwdPricesPrev[j] ;
        double optVal = (priceA > priceE)? priceA: priceE; 
        optPricesN.push_back(optVal);
    }
    optPrices.push_back(optPricesN);
    fwdPricesPrev = optPricesN;
    z = z-1;
}
return optPrices;
};



int main()
{
    // define params for stock
    double s = 29.0; // spot price
    int n = 250; // no of steps in binomial tree
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
    double K = 30.0;
    double p = (exp(r * dT) - d)/(u - d); // prob of up move
    std::vector<std::vector<double>> optPriceTree = optionPrice(bintree,p,K);
    std::cout << "option price: " << optPriceTree[n-1][0] << std::endl;

    // for delta calculation
    std::vector<std::vector<double>> bintreeD = generateTree(s*1.0001,u,d,n);
    std::vector<std::vector<double>> optPriceTreeD = optionPrice(bintreeD,p,K);
    std::cout << "option Delta: " << (optPriceTreeD[n-1][0]-optPriceTree[n-1][0])/(s*0.0001) << std::endl;

    return 0;
}