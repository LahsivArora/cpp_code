#include <random>
#include <iostream>
#include <vector>
#include <fstream>

int main()
{
    // Loss frequency params. Note this is a Poisson distribution
    std::mt19937 gen; 
    int n = 4;
    std::poisson_distribution<int> pos(n); 
    int x;

    // Loss Severity inputs. Note this is lognormal dist and we need to convert to norm dist
    std::mt19937 gen1;  
    double mean = 80.0;
    double stddev = 40.0;
    std::vector<double> samples;

    // parameters for norm dist Loss severity dist
    double w = (stddev * stddev)/(mean * mean);
    double mean_norm = 0.5 * log( (mean * mean) / (1+w));
    double stddev_norm = sqrt(log((1+w)));
    std::normal_distribution<double> norm(mean_norm, stddev_norm);    
    //std::cout << "new mean is " << mean_norm << " and new stddev is " << stddev_norm << std::endl;

    
    std::ofstream outFile("C:/Users/aparn/Desktop/Job Search/cpp code/LossDist.csv");

    for (int j = 0; j < 10000; j++)
    {
        double val;
        double convert = 0;

        // sample from poisson dist for # of loss event
        x = pos(gen);

        // sample from Loss severity x times
        for (int i =0; i < x; i++)
            {
                val = norm(gen);
                convert += exp(val);
            }

        samples.push_back(convert);
        outFile << convert << std::endl;
        //std::cout << "iter#" << j+1 <<": sampled loss event is " << x <<" and Total loss for sampled scenario is " << samples[j] << std::endl;
    } 

    return 0;
}