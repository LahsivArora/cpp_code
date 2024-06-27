#include <iostream>
#include <queue>
#include <ctime>
#include "datamodel.h"
#include "calc.h"
#include "backTestData.h"

// setup ccyPair. definitions available in DataModel file
ccyPairDef USDJPY = {ccyPairs::USDJPY, 0.01, 0.4, 0.70, 0.4, 10.0};
ccyPairDef EURUSD = {ccyPairs::EURUSD, 0.0001, 0.4, 0.70, 0.4, 8.0};

int main()
{
try
{
    std::vector<std::string> files = {"Jan2024","Feb2024","Mar2024","Apr2024","May2024"};
    std::vector<dataType> types = {dataType::SEC, dataType::TICK};
    std::vector<ccyPairs> ccies = {ccyPairs::USDJPY, ccyPairs::EURUSD};

    for (unsigned int x=0; x < files.size(); x++){
        for (unsigned int y=0; y < types.size(); y++){
            for (unsigned int z=0; z < ccies.size(); z++){
                std::string name = (ccies[z]==ccyPairs::USDJPY?"USDJPY":"EURUSD") ; 
                name = name + "_" + files[x] + "_" + (types[y]==dataType::SEC?"1s":"t");
                Rewind mktData((ccies[z]==ccyPairs::USDJPY?USDJPY:EURUSD),"./data/"+name+".csv",types[y]);
                std::queue<tick> loadedMktData = mktData.load();
                std::cout << "size of tick queue:" << loadedMktData.size() << std::endl;
                std::string launchTime = std::to_string(std::time(0));
                Replay replay(&loadedMktData, ccies[z]==ccyPairs::USDJPY?USDJPY:EURUSD,"./data/tradeLog_"+launchTime+".txt");
                std::pair<std::vector<trade>,std::map<std::string,double[2]>> result = replay.use();    
                printResult(result,"./data/pnl_"+name+"_"+launchTime+".txt");
            }
        }
    }

}   catch (std::string &err)
        {std::cout << "user defined exception caught:" << err << std::endl;}
    catch (std::exception &ex)
        {std::cout << "system defined exception caught:" << ex.what() << std::endl;}
    catch (...)
        {std::cout << "undefined exception caught" << std::endl;}
    return 0;
}