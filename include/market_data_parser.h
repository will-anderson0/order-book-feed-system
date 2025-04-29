#pragma once
#include <string>
#include "types.h"

/**
 * @brief Handles connecting to a market data feed and parsing incoming messages.
 */
class MarketDataParser {
  public:
    /**
     * @brief Parses a raw incoming market data message into an OrderEvent.
     * 
     * Expected message format: EVENT_TYPE,SIDE,PRICE,QUANTITY,ORDER_ID
     * Example: "ADD,B,101.25,5,order123"
     * 
     * @param message Raw market data message as a string.
     * @return Parsed OrderEvent object.
     */
    OrderEvent parseMessage(const std::string& message);
};