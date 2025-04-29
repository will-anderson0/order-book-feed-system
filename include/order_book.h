#pragma once
#include <string>
#include <deque>
#include <map>
#include "types.h"

class OrderBook
{

public:
  /**
   * @brief Adds an order to the order book.
   *
   * @param order The order to be added.
   */
  void addOrder(const Order &order);

  /**
   * @brief Modifies an existing order in the order book.
   *
   * @param order_id The ID of the order to be modified.
   * @param new_order The new order details.
   */
  void modifyOrder(const std::string &order_id, const Order &new_order);

  /**
   * @brief Cancels an existing order in the order book.
   *
   * @param order_id The ID of the order to be canceled.
   */
  void cancelOrder(const std::string &order_id);

  /**
   * @brief Prints the current state of the order book.
   */
  void printOrderBook() const;

  /**
   * @brief Retrieves the current bids in the order book.
   *
   * @return A map of bids, where the key is the price and the value is a deque of orders.
   */
  const std::map<double, std::deque<Order>> &getBids() const;

  /**
   * @brief Retrieves the current asks in the order book.
   *
   * @return A map of asks, where the key is the price and the value is a deque of orders.
   */
  const std::map<double, std::deque<Order>> &getAsks() const;

private:
  std::map<double, std::deque<Order>> bids;
  std::map<double, std::deque<Order>> asks;
  std::unordered_map<std::string, std::pair<SideType, double>> order_id_map;
};