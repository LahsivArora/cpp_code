#include <iostream>
#include <vector>
#include <cmath>
#include <string>

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

std::vector<std::vector<double>> optionPriceTree (const std::vector<std::vector<double>>& priceTree,
                                 const double& prob, const double& strike, const std::string& optType,
                                 const double& rate, const double& step) { 

std::vector<std::vector<double>> optPrices;
int steps = priceTree.size();
std::vector<double> dummy(steps, 0.0),fwdPrices, fwdPricesPrev=dummy;
int z = steps;

// backward induction
while (z > 0){
    double priceN, priceT;
    std::vector<double> optPricesN;
    fwdPrices = priceTree[z-1];
    int nodes = fwdPrices.size();
    optPricesN.clear();

    for (int j=0; j < z; j++){
        if (optType == "call")
            priceT = (fwdPrices[j] > strike)? (fwdPrices[j] - strike): 0.0;
        else
            priceT = (fwdPrices[j] < strike)? (strike - fwdPrices[j]): 0.0;

        priceN = (prob*fwdPricesPrev[j+1] + (1.0-prob)*fwdPricesPrev[j])*exp(-1.0*rate*step) ;
        double optVal = (z==steps)? priceT: priceN; 
        optPricesN.push_back(optVal);
    }
    optPrices.push_back(optPricesN);
    fwdPricesPrev = optPricesN;
    z = z-1;
}
return optPrices;
};

double norm(double value)
{
   return 0.5 *(1.0 + erf(value/sqrt(2.0)));
}

double normdiff(double value)
{
   return exp(-0.5*value*value) /sqrt(2.0*M_PI);
}


std::vector<double> optionPriceBS (const double& spot, const double& vol, const double& rate, const double& time,
                                 const double& strike, const std::string& optType) { 

std::vector<double> ret;
double price, delta, vega, gamma;
double d1 = (log(spot/strike) + ( rate + (vol*vol/2.0))*time)/(vol * sqrt(time));
double d2 = d1 - (vol * sqrt(time));

if (optType == "call")
    {
    price = spot*norm(d1) - strike*norm(d2)*exp(-1.0*rate*time);
    delta = norm(d1);
    vega = spot*sqrt(time)*normdiff(d1);
    gamma = normdiff(d1)/(spot * vol * sqrt(time));
    }
else
    {
    price = strike*norm(-1.0*d2)*exp(-1.0*rate*time) - spot*norm(-1.0*d1);
    delta = norm(d1)-1.0;
    vega = spot*sqrt(time)*normdiff(d1);
    gamma = normdiff(d1)/(spot * vol * sqrt(time));
    }

ret.push_back(price);
ret.push_back(delta);
ret.push_back(vega);
ret.push_back(gamma);
return ret;
}

int main()
{
    // define params for stock
    double s = 56.0; // spot price
    double vol = 0.30; // volatility of stock of 25%
    double r = 0.05; // risk free rate of 3%
    double T = 1.5; // maturity in number of years
    int n = 252.0 * T * 1.0; // no of steps in binomial tree; taken as daily steps

    // calculated params
    double dT = T/(n-1) ; // length of each time step
    double u = exp(vol*sqrt(dT));       // exp up move
    double d = exp(-1.0*vol*sqrt(dT));  // exp down move
    
    // generate tree
    std::vector<std::vector<double>> bintree = generateTree(s,u,d,n);

    // generate option price (European) comparison
    double K = 60.0;
    double p = (exp(r * dT) - d)/(u - d); // prob of up move
    std::vector<std::vector<double>> optPriceTree_call = optionPriceTree(bintree,p,K,"call",r,dT);
    std::vector<std::vector<double>> optPriceTree_put = optionPriceTree(bintree,p,K,"put",r,dT);

    std::cout << "==========================================================" << std::endl;
    std::cout << "Call option price (binomial): " << optPriceTree_call[n-1][0] << std::endl;
    std::cout << "Put option price (binomial) : " << optPriceTree_put[n-1][0] << std::endl;

    std::vector<double> optPriceBS_call = optionPriceBS(s,vol,r,T,K,"call");
    std::vector<double> optPriceBS_put = optionPriceBS(s,vol,r,T,K,"put");

    std::cout << "Call option price (black scholes): " << optPriceBS_call[0] << std::endl;
    std::cout << "Put option price (black scholes) : " << optPriceBS_put[0] << std::endl;


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
    return 0;
}