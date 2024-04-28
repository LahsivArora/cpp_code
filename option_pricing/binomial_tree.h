#ifndef BINOMIAL_TREE_H_
#define BINOMIAL_TREE_H_

#include <vector>
#include <string>

class generateTree{
private:
    double xspot,xvol,xtime;
    int xnsteps;
public:
    generateTree (double spot, double vol, double time, int nSteps);
    std::vector<std::vector<double>> get();
};

class priceTree{
private:
    std::vector<std::vector<double>> tree;
    double xstrike, xspot, xvol, xtime, xrate; 
    std::string xtype;
    int xsteps;
public:
    priceTree (std::vector<std::vector<double>> genTree, double strike, std::string optType,
                double spot, double vol, double time, int nSteps, double rate);
    std::vector<std::vector<double>> get();
};

#endif