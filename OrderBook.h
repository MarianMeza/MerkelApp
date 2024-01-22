#pragma once
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include <string>
#include <vector>


class OrderBook 
{
    public:
        /** construct, reading a csv data file */
        OrderBook(std::string filename);

        /** returns vector of all known products in the dataset */
        std::vector<std::string> getKnownProducts();

        /** returns vector of orders according to the sent filters */
        std::vector<OrderBookEntry> getOrders
        (  
            OrderBookType type, 
            std::string product, 
            std::string timestamp
        );

        /** returns the earliest time in the orderbook */
        std::string getEarliestTime();
        
        /** returns the next time after the sent time in the orderbook 
         * if there is no next timestamp, wraps around to the start
        */
        std::string getNextTime(std::string timestamp);      

        /** insert a new entry to the Order Book */
        void insertOrder(OrderBookEntry& order);  

        /** matching algorithm */
        std::vector<OrderBookEntry> matchAsksToBids(std::string product, std::string timestamp);
        
        /** returns the highest price in the same timeframe*/
        static double getHighPrice(std::vector<OrderBookEntry>& entries);
        
        /** returns the lowest price in the same timeframe*/
        static double getLowPrice(std::vector<OrderBookEntry>& entries);
        
        /** returns the change in the value since the last time frame
         * it receives 2 OrderBook entries vector as a parameters, the first one with current values 
         * and the second one with previous values 
         * returns a double (difference in price)
         * if there is no difference between previous and current max/min price or there is no previous values, returns 0
        */
        static double getMaxPriceChange(std::vector<OrderBookEntry>& entries, std::vector<OrderBookEntry>& previousEntries);
        static double getMinPriceChange(std::vector<OrderBookEntry>& entries, std::vector<OrderBookEntry>& previousEntries);
    
    private:
        std::vector<OrderBookEntry> entries;
};