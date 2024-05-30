#ifndef BINOMIAL_TREE_H_
#define BINOMIAL_TREE_H_

#include <vector>
#include <string>
#include "Enums.h"

class generateTree{
private:
    double xspot,xvol,xtime;
    unsigned int xnsteps;
public:
    generateTree (double spot, double vol, double time, unsigned int nSteps);
    std::vector<std::vector<double>> get();
};

class priceTree{
private:
    std::vector<std::vector<double>> xtree;
    double xstrike, xspot, xvol, xtime, xrate; 
    OptType xtype;
    unsigned int xsteps;
public:
    priceTree (std::vector<std::vector<double>> genTree, double strike, OptType optType,
                double spot, double vol, double time, unsigned int nSteps, double rate);
    std::vector<std::vector<double>> calc();
};

#endif