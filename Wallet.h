#pragma once

#include <string>
#include <map>
#include "OrderBookEntry.h"

class Wallet 
{
    public:
        /** constructor */
        Wallet();

        /** insert currency to the wallet */
        void insertCurrency(std::string type, double amount);

        /** remove currency from the wallet */
        bool removeCurrency(std::string type, double amount);

        /** check if the wallet contains this much currency of more */
        bool containsCurrency(std::string type, double amount);
        
        /** checks if the wallet can cope with this ask or bid */
        bool canFulfillOrder(OrderBookEntry entry);

        /** update the contents of the wallet
         * assumes the order was made by the owner of the wallet
         */
        void processSale(OrderBookEntry& sale);
        
        /** generate a string representation of the wallet */
        std::string toString();
        
    private:
        /** stores type of currency and amount */
        std::map<std::string,double> currencies;


};