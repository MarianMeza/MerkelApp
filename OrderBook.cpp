#include "OrderBook.h"
#include "CSVReader.h"
#include <map>
#include <iostream>
#include <algorithm> 

/** construct, reading a csv data file */
OrderBook::OrderBook(std::string filename)
{
    entries = CSVReader::readCSV(filename);
}

/** return vector of all know products in the dataset */    
std::vector<std::string> OrderBook::getKnownProducts()
{
    std::vector<std::string> products; 

    std::map<std::string, bool> prodMap;

    // look for unique key values (products) in order book entries
    for (OrderBookEntry& e : entries)
    {
        prodMap[e.product] = true;
    }

    // now flatten the map to a vector of strings
    for (auto const& e : prodMap)
    {
        products.push_back(e.first);
    }

    return products;
}      

/** return vector of Orders according to the sent filters */
std::vector<OrderBookEntry> OrderBook::getOrders
(  
    OrderBookType type, 
    std::string product, 
    std::string timestamp
)
{
    std::vector<OrderBookEntry> entries_sub;
    
    for (OrderBookEntry& e : entries)
    {
        if (e.orderType == type && 
            e.product == product &&
            e.timestamp == timestamp)
            {
                entries_sub.push_back(e);
            }
    }

    return entries_sub;
}


double OrderBook::getHighPrice(std::vector<OrderBookEntry>& entries)
{
    double max = entries[0].price;

    for (OrderBookEntry& e : entries)
    {
        if (e.price > max) max = e.price;
    }

    return max;
}

double OrderBook::getLowPrice(std::vector<OrderBookEntry>& entries)
{
    double min = entries[0].price;

    for (OrderBookEntry& e : entries)
    {
        if (e.price < min) min = e.price;
    }

    return min;
}

double OrderBook::getMaxPriceChange(std::vector<OrderBookEntry>& entries, std::vector<OrderBookEntry>& previousEntries)
{
    double maxPrice = getHighPrice(entries);
    double maxPrevPrice = 0.0;

    if (previousEntries.size() == 0) maxPrevPrice = maxPrice; 
    else maxPrevPrice = getHighPrice(previousEntries);

    return maxPrice - maxPrevPrice;
}

double OrderBook::getMinPriceChange(std::vector<OrderBookEntry>& entries, std::vector<OrderBookEntry>& previousEntries)
{
    double minPrice = getLowPrice(entries);
    double minPrevPrice = 0.0;

    if (previousEntries.size() == 0) minPrevPrice = minPrice;
    else minPrevPrice = getLowPrice(previousEntries);

    return minPrice - minPrevPrice;
}


std::string OrderBook::getEarliestTime()
{
    return entries[0].timestamp;
}

std::string OrderBook::getNextTime(std::string timestamp)
{
    std::string next_timestamp = "";
    for (OrderBookEntry& e : entries)
    {
        if (e.timestamp > timestamp) 
        {
            next_timestamp = e.timestamp;
            break;
        }
    }
    if (next_timestamp == "")
    {
        next_timestamp = entries[0].timestamp;
    }
    return next_timestamp;
}

void OrderBook::insertOrder(OrderBookEntry& entry)
{
    entries.push_back(entry);
    std::sort(entries.begin(), entries.end(), OrderBookEntry::compareByTimestamp);
}

std::vector<OrderBookEntry> OrderBook::matchAsksToBids
(
    std::string product, 
    std::string timestamp
)
{   
    std::vector<OrderBookEntry> asks = getOrders
    (
        OrderBookType::ask, 
        product, 
        timestamp
    );
    std::vector<OrderBookEntry> bids = getOrders
    (
        OrderBookType::bid, 
        product, 
        timestamp
    );

    std::vector<OrderBookEntry> sales;

    // check to ensure we have bids and asks to process.
    if (asks.size() == 0 || bids.size() == 0)
    {
        std::cout << " OrderBook::matchAsksToBids: no bids or asks" << std::endl;
        return sales;
    }

    // sort lowest asks first
    std::sort(asks.begin(),asks.end(),OrderBookEntry::compareByPriceAsc);

    // sort highest bids first
    std::sort(bids.begin(),bids.end(),OrderBookEntry::compareByPriceDesc);

    std::cout << "max ask " << asks[asks.size()-1].price << std::endl;
    std::cout << "min ask " << asks[0].price << std::endl;
    std::cout << "max bid " << bids[0].price << std::endl;
    std::cout << "min bid " << bids[bids.size()-1].price << std::endl;

    for (OrderBookEntry& a : asks)
    {
        for (OrderBookEntry& b : bids)
        {
            // we have a match
            if (b.price >= a.price)
            {
                OrderBookEntry sale
                {
                    a.price,
                    0,
                    timestamp,
                    product,
                    OrderBookType::askSale
                };

                if (b.username == "simUser")
                {
                    sale.username = "simUser";
                    sale.orderType = OrderBookType::bidSale;
                }
                if (a.username == "simUser")
                {
                    sale.username = "simUser";
                    sale.orderType = OrderBookType::askSale;
                }

                // work out how much was sold and create new bids and asks covering anything that was not sold 
                if (b.amount == a.amount)
                {
                    sale.amount = a.amount;
                    sales.push_back(sale);
                    // make sure bid is not processed again
                    b.amount = 0;
                    // ask fullfilled, next ask
                    break;
                }

                // ask is completely gone, slice the bid
                if (b.amount > a.amount)
                {
                    sale.amount = a.amount;
                    sales.push_back(sale);
                    // so it can be used to process next ask
                    b.amount = b.amount - a.amount;
                    // ask fullfilled, next ask
                    break;
                }

                // bid is completely gone, slice the ask
                if (b.amount < a.amount && 
                    b.amount > 0)
                {
                    sale.amount = b.amount;
                    sales.push_back(sale);
                    // update the ask
                    // and allow further bids to process the remaining amount
                    a.amount = a.amount - b.amount;
                    // make sure bid is not processed again
                    b.amount = 0;
                    // some ask remains, so go to next bid
                    continue;
                }
            } 
        }
    }

    return sales;
}


