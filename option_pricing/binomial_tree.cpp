#include <vector>
#include <string>
#include <cmath>
#include "binomial_tree.h"


generateTree::generateTree(double spot,double vol, double time, int nSteps){
    xspot=spot;
    xvol=vol;
    xtime=time;
    xnsteps=nSteps;
}

std::vector<std::vector<double>> generateTree::get(){
    // calculated params
    double dT = xtime/(xnsteps-1) ; // length of each time step
    double u = exp(xvol*sqrt(dT));       // exp up move
    double d = exp(-1.0*xvol*sqrt(dT));  // exp down move

    std::vector<std::vector<double>> tree;
    for (int i=1; i <= xnsteps; i++){
        std::vector<double> intd;
        for (int j=0; j < i; j++){
            double val = xspot*pow(u,j)*pow(d,i-j-1) ;
            intd.push_back(val);
        }
        tree.push_back(intd);
    }
    return tree;
}


priceTree::priceTree (std::vector<std::vector<double>> genTree, double strike, std::string optType,
                double spot, double vol, double time, int nSteps, double rate){
    tree = genTree;
    xstrike = strike;
    xtype = optType;
    xspot = spot;
    xvol = vol;
    xtime = time;
    xsteps= nSteps;
    xrate = rate; 
}

std::vector<std::vector<double>> priceTree::get() { 
    // calculated params
    double dT = xtime/(xsteps-1) ; // length of each time step
    double u = exp(xvol*sqrt(dT));       // exp up move
    double d = exp(-1.0*xvol*sqrt(dT));  // exp down move
    double prob = (exp(xrate * dT) - d)/(u - d); // prob of up move

    // function starts
    std::vector<std::vector<double>> optPrices;
    int steps = tree.size();
    std::vector<double> dummy(steps, 0.0),fwdPrices, fwdPricesPrev=dummy;
    int z = xsteps;

    // backward induction
    while (z > 0){
        double priceN, priceT;
        std::vector<double> optPricesN;
        fwdPrices = tree[z-1];
        int nodes = fwdPrices.size();
        optPricesN.clear();

        for (int j=0; j < z; j++){
            if (xtype == "call")
                priceT = (fwdPrices[j] > xstrike)? (fwdPrices[j] - xstrike): 0.0;
            else
                priceT = (fwdPrices[j] < xstrike)? (xstrike - fwdPrices[j]): 0.0;

            priceN = (prob*fwdPricesPrev[j+1] + (1.0-prob)*fwdPricesPrev[j])*exp(-1.0*xrate*dT) ;
            double optVal = (z==steps)? priceT: priceN; 
            optPricesN.push_back(optVal);
        }
        optPrices.push_back(optPricesN);
        fwdPricesPrev = optPricesN;
        z = z-1;
    }
    return optPrices;
};
