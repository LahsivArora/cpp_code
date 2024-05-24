In Repository:

CVA:: implementing a CVA framework with: 1. creation of swap objects (currently Vanilla swap); 2. create portfolio object with vector of swap objects; 3. create Rate Curve object (currently with zero rates); 4. pricing object with pricing functions to calculate NPV of 1 trade and 1 portfolio (collection of trades); 5. Simulation engine to simulate n curves (currently hardcoded); 6. implement Exposure object/functions that calculates EE (expected exposure) for a portfolio + simulated curves combination; 7. implemenet CDS curve object to (currently with constant hazard rate) to provide PD; 8. calculate CVA (as sum(EE\*marginal PD\*LGD)) 

Option Pricing:: implementing binomial tree and black scholes pricing for European options; added risk calculations (both analytical and bump-and-reval based approach)

Old Code:: C++ example code to cover most used functionalities: 1. arrayMultiFunction: combine n arrays using function calls in loop; 2. convertNum: convert string to int (instead of using atoi); 3. histogram: example with STL containers and iterators; 4. Sudoku Combination: example with functions, vector and set

FRM Examples:: some tasks done while preparing for FRM exams: 1. Operational Loss EC calculation (using Monte Carlo) based on Basel2 regulations; 2. Binomial tree implementation with non-dividend stock; 3. Binomial tree code updated with Delta (stock and risk free rate) calculation

============================================================================================
Future ToDo:

CVA extension:: 1. extend swap object to support Xccy swap; 2. extend portfolio to have a mix of Vanilla and Xccy swaps; 3. extend Rate curve to input swap rates and bootstap curve to generate zero rates; 4. extend Simulation to use Hull White model for interest rates and Local vol model for FX; 6. extend Exposure to calculate EPE (at 95% confidence level); 7. extend CDS curve to input CDS spread for tenors and bootstrap to get hazard rate; 

