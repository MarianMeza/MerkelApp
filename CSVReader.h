#pragma once

#include "OrderBookEntry.h"
#include <vector>
#include <string>

class CSVReader
{
	public:	
		CSVReader();

		static std::vector<OrderBookEntry> readCSV(std::string csvFile);
		static std::vector<std::string> tokenise(std::string csvLine, char separator);
		static OrderBookEntry stringstoOBE
		(
			std::string price,
			std::string amount, 
			std::string timestamp,
			std::string product,
			OrderBookType orderType
		);

	private:
		static OrderBookEntry stringstoOBE(std::vector<std::string> strings);
};

