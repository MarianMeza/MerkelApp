#include "MerkelMain.h"
#include <iostream>
#include <vector>
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include "OrderBook.h"
#include <limits>

MerkelMain::MerkelMain()
{

}

void MerkelMain::init()
{
    int input;
    currentTime = orderBook.getEarliestTime();
    
    wallet.insertCurrency("BTC",10);
    
    while (true)
    {
        printMenu();
        input = getUserOption();
        processUserOption(input);
    }
}

void MerkelMain::printMenu() //Prints the menu for the Merkel app
{
    std::cout << "-------------- MERKEL APP MENU -------------" << std::endl;
    std::cout << "1. Print help " << std::endl;
    std::cout << "2. Print market stats " << std::endl;
    std::cout << "3. Make an ask " << std::endl;
    std::cout << "4. Make a bid " << std::endl;
    std::cout << "5. Print wallet " << std::endl;
    std::cout << "6. Continue " << std::endl;
    std::cout << "--------------------------------------------" << std::endl;
    std::cout << "Current time is: " << currentTime << std::endl << std::endl;
}

void MerkelMain::printInvalidInput() //Prints invalid input message
{
    std::cout << std::endl << "--------------------------------------------" << std::endl;
    std::cout << "Invalid input! Type a number between 1 and 6." << std::endl;
    std::cout << "--------------------------------------------" << std::endl << std::endl;
}

void MerkelMain::printHelp() //Prints help message
{
    std::cout << std::endl;
    std::cout << "-------------------- HELP ------------------" << std::endl;
    std::cout << "Your aim is to make money. Analyse the market and make bids and offers." << std::endl;
    std::cout << "--------------------------------------------" << std::endl << std::endl;
}

void MerkelMain::printExchangeStatus() //Prints market stats
{
    std::cout << std::endl << "---------------- MARKET STATS --------------" << std::endl << std::endl;
    
    for (const std::string& p : orderBook.getKnownProducts())
    {
        std::cout << "Product: " << p << std::endl;
        std::vector<OrderBookEntry> entries = orderBook.getOrders
        (
            OrderBookType::ask, 
            p, 
            currentTime
        );
        std::vector<OrderBookEntry> previousEntries = orderBook.getOrders
        (
            OrderBookType::ask, 
            p, 
            previousTime
        );
        std::cout << "Asks seen: " << entries.size() << std::endl; 
        std::cout << "Max ask: " << OrderBook::getHighPrice(entries) << std::endl;
        std::cout << "Min ask: " << OrderBook::getLowPrice(entries) << std::endl;
        std::cout << "Max change: " << OrderBook::getMaxPriceChange(entries, previousEntries) << std::endl;
        std::cout << "Min change: " << OrderBook::getMinPriceChange(entries, previousEntries) << std::endl;
        std::cout << "--------------------------------------------" << std::endl << std::endl; 
    }

    
}

void MerkelMain::makeAsk() //Allows you to make an offer/ask
{
    std::cout << std::endl;
    std::cout << "---------------- MAKE AN ASK -----------------" << std::endl;
    std::cout << "Enter the product, price and amount. For example: ETH/BTC,200,0.5." << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if(tokens.size() != 3)
    {
        std::cout << "MerkelMain::makeAsk: Bad input: " << input << std::endl;
    }
    else 
    {
        try 
        {
            OrderBookEntry obe = CSVReader::stringstoOBE
            (
                tokens[1],
                tokens[2],
                currentTime,
                tokens[0],
                OrderBookType::ask
            );
            obe.username = "simUser";

            if (wallet.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks good! " << std::endl;
                orderBook.insertOrder(obe);
            }
            else 
            {
                std::cout << "Wallet has insufficient funds! " << std::endl;
            }
        }
        catch (const std::exception& e) 
        {
            std::cout << "MerkelMain::makeAsk: Bad input!" << std::endl;
        }
        
    }
    
    std::cout << "--------------------------------------------" << std::endl << std::endl;

}

void MerkelMain::makeBid() //Allows you to make a bid
{
    std::cout << std::endl;
    std::cout << "---------------- MAKE A BID ----------------" << std::endl;
    std::cout << "Enter the product, price and amount. For example: ETH/BTC,200,0.5." << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if(tokens.size() != 3)
    {
        std::cout << "MerkelMain::makeBid: Bad input: " << input << std::endl;
    }
    else 
    {
        try 
        {
            OrderBookEntry obe = CSVReader::stringstoOBE
            (
                tokens[1],
                tokens[2],
                currentTime,
                tokens[0],
                OrderBookType::bid
            );
            obe.username = "simUser";

            if (wallet.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks good! " << std::endl;
                orderBook.insertOrder(obe);
            }
            else 
            {
                std::cout << "Wallet has insufficient funds! " << std::endl;
            }
        }
        catch (const std::exception& e) 
        {
            std::cout << "MerkelMain::makeBid: Bad input!" << std::endl;
        }
        
    }

    std::cout << "--------------------------------------------" << std::endl << std::endl;

}

void MerkelMain::printWallet() //Prints your wallet info
{
    std::cout << std::endl;
    std::cout << "------------------ WALLET ------------------" << std::endl;
    std::cout << wallet.toString();
    std::cout << "--------------------------------------------" << std::endl << std::endl;
}

void MerkelMain::goToNextTimeframe() //Continue
{
    previousTime = currentTime;
    currentTime = orderBook.getNextTime(currentTime);

    std::cout << std::endl;
    std::cout << "----------------- CONTINUE -----------------" << std::endl;
    std::cout << "Going to next time frame." << std::endl;

    std::vector<OrderBookEntry> sales = orderBook.matchAsksToBids("ETH/BTC", currentTime);
    std::cout << "Sales: " << sales.size() << std::endl;
    for (OrderBookEntry& s : sales)
    {
        std::cout << "Sale price: " << s.price << " amount " << s.amount << std::endl;
        if (s.username == "simUser")
        {
            //update the wallet
            wallet.processSale(s);
        }
    }

    std::cout << "--------------------------------------------" << std::endl << std::endl;
}

int MerkelMain::getUserOption() //Receives user input for the menu
{
    std::string line;
    int userOption = 0;

    std::cout << "Type in 1-6" << std::endl;
    std::getline(std::cin,line);
    try
    {
        userOption = std::stoi(line);   
    }
    catch (const std::exception& e) 
    { 
        userOption = 0;
    }

    //validate user input (number between 1-6)
    if (userOption > 6)
    {
        userOption = 0;
    }

    return userOption;
}

void MerkelMain::processUserOption(int userOption) //Processes user option and calls the adequate function
{

    if (userOption == 0)  //bad input
    {
        printInvalidInput();
    }
    if (userOption == 1) //1. Help
    {
        printHelp();
    }
    if (userOption == 2) //2. Stats
    {
        printExchangeStatus();
    }
    if (userOption == 3) //3. Ask
    {
        makeAsk();
    }
    if (userOption == 4) //4. Bid
    {
        makeBid();
    }
    if (userOption == 5) //5.  Wallet
    {
        printWallet();
    }
    if (userOption == 6) //6. Continue
    {
        goToNextTimeframe();
    }
}

/* double MerkelMain::computeAveragePrice(std::vector<OrderBookEntry>& entries)
{
    double sum = 0;

    for (unsigned int i = 0; i < entries.size(); ++i)
    {
        sum += entries[i].price;
    }

    return sum/entries.size();
} */

/* double MerkelMain::computeLowPrice(std::vector<OrderBookEntry>& entries)
{
    double max = 0;

    for (unsigned int i = 0; i < entries.size(); ++i)
    {
        if (entries[i].price > max)
        {
            max = entries[i].price;
        }
    }

    return max;
} */

/* double MerkelMain::computeHighPrice(std::vector<OrderBookEntry>& entries)
{
    double min = entries[0].price;

    for (unsigned int i = 0; i < entries.size(); ++i)
    {
        if (entries[i].price < min)
        {
            min = entries[i].price;
        }
    }

    return min;
} */

/* double MerkelMain::computePriceSpread(std::vector<OrderBookEntry>& entries)
{
    return 0;
} */
