#pragma once

#include <string>

/**
 * @brief Types of events that can occur in the order book.
 */
enum class EventType
{
  ADD,    ///< Add a new order to the book
  MODIFY, ///< Modify an existing order's size or price
  CANCEL  ///< Cancel an existing order
};

/**
 * @brief Side of the order: Buy (Bid) or Sell (Ask).
 */
enum class SideType
{
  B, ///< Bid (Buy)
  S  ///< Ask (Sell)
};

/**
 * @brief Represents a single event (add/modify/cancel) in the market data feed.
 */
struct OrderEvent
{
  EventType type;       ///< Type of event (ADD, MODIFY, CANCEL)
  SideType side;        ///< Side of the book (B = Bid, S = Ask)
  double price;         ///< Price of the order
  int quantity;         ///< Quantity of the order
  std::string order_id; ///< Unique identifier for the order
};

/**
 * @brief Represents a single order in the order book.
 */
struct Order
{
  std::string order_id; ///< Unique identifier for the order
  double price;         ///< Price of the order
  int quantity;         ///< Quantity of the order
  SideType side;        ///< Side of the book (B = Bid, S = Ask)
};