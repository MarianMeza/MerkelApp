#include "CSVReader.h"
#include "OrderBookEntry.h"
#include <vector>
#include<string>
#include <iostream>
#include <fstream>

CSVReader::CSVReader()
{

}

std::vector<OrderBookEntry> CSVReader::readCSV(std::string csvFilename)
{
    std::vector<OrderBookEntry> entries;

    std::ifstream csvFile{ csvFilename };
    std::string line;
    if (csvFile.is_open())
    {
        while (std::getline(csvFile, line))
        {   
            try 
            {
                OrderBookEntry obe = stringstoOBE(tokenise(line, ','));
                entries.push_back(obe);
            }
            catch (const std::exception& e) 
            {
                std::cout << "CSVReader::readCSV bad data" << std::endl;
            } 
        }// end of while
    }

    std::cout << "CSVReader::readCSV read " << entries.size() << " entries" << std::endl;
    return entries;
}

std::vector<std::string> CSVReader::tokenise(std::string csvLine, char separator)
{
    std::vector<std::string> tokens;
    signed int start, end;
    std::string token;

    //finds the start
    start = csvLine.find_first_not_of(separator, 0);

    do {
        //next separator after start
        end = csvLine.find_first_of(separator, start);

        //nothing more to find
        if (start == csvLine.length() || start == end) break;

        //we found the separator
        if (end >= 0) token = csvLine.substr(start, end - start);

        //end is invalid
        else token = csvLine.substr(start, csvLine.length() - start);

        //save the token
        tokens.push_back(token);

        //move past this token
        start = end + 1;

    //continue loop condition
    } while (end != std::string::npos);

    return tokens;
}

OrderBookEntry CSVReader::stringstoOBE(std::vector<std::string> tokens)
{
    double price, amount;

    // more or less than 5 tokens is a bad line
    if (tokens.size() != 5)
    {
        std::cout << "CSVReader::stringstoOBE: Bad line" << std::endl;
        throw std::exception{};
    }

    // we have 5 tokens
    try { // catch exception in string to double conversion
        price = std::stod(tokens[3]);
        amount = std::stod(tokens[4]);
    }
    catch (const std::exception& e) {
        std::cout << "CSVReader::stringstoOBE: Bad float! " << tokens[3] << std::endl;
        std::cout << "CSVReader::stringstoOBE: Bad float! " << tokens[4] << std::endl;
        throw;
    }

    // create OrderBookEntry object with tokens and converted doubles
	OrderBookEntry obe
    {
        price, 
        amount, 
        tokens[0], 
        tokens[1], 
        OrderBookEntry::stringToOBT(tokens[2])
    };

    // return OrderBookEntry object
	return obe;

}

OrderBookEntry CSVReader::stringstoOBE
(
    std::string priceString,
    std::string amountString, 
    std::string timestamp,
    std::string product,
    OrderBookType orderType
)
{
    double price, amount;

    try { // catch exception in string to double conversion
        price = std::stod(priceString);
        amount = std::stod(amountString);
    }
    catch (const std::exception& e) {
        std::cout << "CSVReader::stringstoOBE: Bad float! " << priceString << std::endl;
        std::cout << "CSVReader::stringstoOBE: Bad float! " << amountString << std::endl;
        throw;
    }

    // create OrderBookEntry object with tokens and converted doubles
	OrderBookEntry obe
    {
        price, 
        amount, 
        timestamp, 
        product, 
        orderType
    };

    // return OrderBookEntry object
	return obe;
}