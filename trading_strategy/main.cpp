#include <iostream>
#include <queue>
#include <ctime>
#include "datamodel.h"
#include "calc.h"
#include "trade.h"
#include "backTestData.h"

int main()
{
try
{
    tick tick1 = {{20240620,130012.006},158.062};
    CreateTrade Trade1(tick1,100000.0,buySell::SELL,trigger::NEW);
    trade *trade1 = new trade;
    *trade1 = Trade1.get();
    std::cout << "tick buy price:" << tick1.buyPrice << std::endl;
    std::cout << "tick sell price:" << tick1.sellPrice << std::endl;

    tick tick2 = {{20240620,130015.000},158.018};
    CreateTrade Trade2(tick2,trade1->notional,buySell::BUY,trigger::TAKEPROFIT);
    trade *trade2 = new trade;
    *trade2 = Trade2.get();

    ProfitLoss realize(trade1, trade2);
    std::cout << "trade status:" << int(trade1->status) << std::endl;
    std::cout << "profit loss for trades:" << realize.calc() << std::endl;
    std::cout << "trade status:" << int(trade1->status) << std::endl;
    std::cout << "trade ids:" << trade1->tradeId << ":" << trade2->tradeId << std::endl;

    Rewind usdjpy_d("./data/extract.txt");
    std::queue<tick> mktData = usdjpy_d.load();
    std::cout << "size of tick queue:" << mktData.size() << std::endl;

    Replay usdjpy(mktData,"./data/tradeLog_"+std::to_string(std::time(0))+".txt");
    std::vector<trade> trades = usdjpy.use();
    std::cout << "size of trades vector:" << trades.size() << std::endl;

}   catch (std::string &err)
        {std::cout << "user defined exception caught:" << err << std::endl;}
    catch (...)
        {std::cout << "undefined exception caught" << std::endl;}
    return 0;
}