#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include "types.h"
#include "market_data_parser.h"

std::vector<std::string> splitByComma(const std::string &str)
{
  std::vector<std::string> result;
  std::stringstream ss(str);
  std::string item;

  while (std::getline(ss, item, ','))
  {
    result.push_back(item);
  }

  return result;
}

OrderEvent MarketDataParser::parseMessage(const std::string &message)
{

  // Split the message by commas
  std::vector<std::string> parts = splitByComma(message);

  // Check if the message has the correct number of parts
  if (parts.size() != 5)
  {
    std::cerr << "Invalid message format: " << message << std::endl;
    return OrderEvent{
        EventType::ADD,
        SideType::B,
        0.0,
        0,
        ""};
  }

  // Parse the event type
  EventType eventType;
  if (parts[0] == "ADD")
  {
    eventType = EventType::ADD;
  }
  else if (parts[0] == "MODIFY")
  {
    eventType = EventType::MODIFY;
  }
  else if (parts[0] == "CANCEL")
  {
    eventType = EventType::CANCEL;
  }

  // Parse the side type
  SideType sideType;
  if (parts[1] == "B")
  {
    sideType = SideType::B;
  }
  else if (parts[1] == "S")
  {
    sideType = SideType::S;
  }

  return OrderEvent{
      eventType,
      sideType,
      std::stod(parts[2]),
      std::stoi(parts[3]),
      parts[4]};
};