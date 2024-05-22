#ifndef PRICING_H_
#define PRICING_H_

#include <vector>
#include <string>

std::vector<double> pricingOpt(double s, double vol, double T, int n, double K, double r);

#endif