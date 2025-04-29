#include <cassert>
#include <iostream>
#include "../include/market_data_parser.h"

// Test 1: Normal ADD message
void testParseAddMessage()
{
    MarketDataParser parser;
    std::string message = "ADD,B,123.45,100,987654";
    OrderEvent event = parser.parseMessage(message);

    assert(event.type == EventType::ADD);
    assert(event.side == SideType::B);
    assert(event.price == 123.45);
    assert(event.quantity == 100);
    assert(event.order_id == "987654");

    std::cout << "Test 1 passed: Parse ADD Message\n";
}

// Test 2: Normal MODIFY message
void testParseModifyMessage()
{
    MarketDataParser parser;
    std::string message = "MODIFY,S,124.50,200,123456";
    OrderEvent event = parser.parseMessage(message);

    assert(event.type == EventType::MODIFY);
    assert(event.side == SideType::S);
    assert(event.price == 124.50);
    assert(event.quantity == 200);
    assert(event.order_id == "123456");

    std::cout << "Test 2 passed: Parse MODIFY Message\n";
}

// Test 3: Normal CANCEL message
void testParseCancelMessage()
{
    MarketDataParser parser;
    std::string message = "CANCEL,B,125.00,150,654321";
    OrderEvent event = parser.parseMessage(message);

    assert(event.type == EventType::CANCEL);
    assert(event.side == SideType::B);
    assert(event.price == 125.00);
    assert(event.quantity == 150);
    assert(event.order_id == "654321");

    std::cout << "Test 3 passed: Parse CANCEL Message\n";
}

int main()
{
    testParseAddMessage();
    testParseModifyMessage();
    testParseCancelMessage();

    std::cout << "\nAll tests passed successfully!\n";
    return 0;
}
