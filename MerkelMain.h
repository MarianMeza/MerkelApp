#pragma once

#include <vector>
#include "OrderBook.h"
#include "Wallet.h"

class MerkelMain
{
public:
    //constructor
    MerkelMain();

    //init function
    void init(); //Call this to start the sim

private:
    //main functions for mekel app
    void printMenu(); //Prints the menu for the Merkel app
    void printInvalidInput(); //Prints invalid input message
    void printHelp(); //Prints help message
    void printExchangeStatus(); //Prints market stats
    void makeAsk(); //Allows you to make an offer/ask
    void makeBid(); //Allows you to make a bid
    void printWallet(); //Prints your wallet info
    void goToNextTimeframe(); //Continue
    int getUserOption(); //Receives user input for the menu
    void processUserOption(int userOption); //Processes user option and calls the adequate function

    //Time varianles
    std::string currentTime;
    std::string previousTime;

    //Order Book object
    OrderBook orderBook{"dataset.csv"};

    //Wallet object
    Wallet wallet;

    //calculate extra functions
    //double computeAveragePrice(std::vector<OrderBookEntry>& entries);
    //double computeLowPrice(std::vector<OrderBookEntry>& entries);
    //double computeHighPrice(std::vector<OrderBookEntry>& entries);
    //double computePriceSpread(std::vector<OrderBookEntry>& entries);

};