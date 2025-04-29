#include "order_book.h"
#include <iostream>

void OrderBook::addOrder(const Order &order)
{
  if (order.side == SideType::B)
  {
    bids[order.price].push_back(order);
  }
  else if (order.side == SideType::S)
  {
    asks[order.price].push_back(order);
  }
  else
  {
    std::cerr << "Invalid order side: " << std::endl;
    return;
  }

  order_id_map[order.order_id] = {order.side, order.price};
}

void OrderBook::modifyOrder(const std::string &order_id, const Order &new_order)
{
  auto it = order_id_map.find(order_id);
  if (it == order_id_map.end())
  {
    std::cerr << "Modify failed: Order ID " << order_id << " not found.\n";
    return;
  }

  SideType old_side = it->second.first;
  double old_price = it->second.second;
  auto &book_side = (old_side == SideType::B) ? bids : asks;

  auto price_it = book_side.find(old_price);
  if (price_it != book_side.end())
  {
    auto &order_list = price_it->second;
    for (auto order_it = order_list.begin(); order_it != order_list.end(); ++order_it)
    {
      if (order_it->order_id == order_id)
      {
        order_list.erase(order_it);
        if (order_list.empty())
        {
          book_side.erase(price_it);
        }
        break;
      }
    }
  }

  // Insert the new modified order
  addOrder(new_order);
}

void OrderBook::cancelOrder(const std::string &order_id)
{
  auto it = order_id_map.find(order_id);
  if (it == order_id_map.end())
  {
    std::cerr << "Cancel failed: Order ID " << order_id << " not found.\n";
    return;
  }

  SideType side = it->second.first;
  double price = it->second.second;
  auto &book_side = (side == SideType::B) ? bids : asks;

  auto price_it = book_side.find(price);
  if (price_it != book_side.end())
  {
    auto &order_list = price_it->second;
    for (auto order_it = order_list.begin(); order_it != order_list.end(); ++order_it)
    {
      if (order_it->order_id == order_id)
      {
        order_list.erase(order_it);
        if (order_list.empty())
        {
          book_side.erase(price_it);
        }
        break;
      }
    }
  }

  order_id_map.erase(it);
}

void OrderBook::printOrderBook() const
{
  std::cout << "Order Book:" << std::endl;

  std::cout << "Bids:" << std::endl;
  for (const auto &pair : bids)
  {
    for (const auto &order : pair.second)
    {
      std::cout << "Order ID: " << order.order_id
                << ", Price: " << order.price
                << ", Quantity: " << order.quantity
                << std::endl;
    }
  }

  std::cout << "Asks:" << std::endl;
  for (const auto &pair : asks)
  {
    for (const auto &order : pair.second)
    {
      std::cout << "Order ID: " << order.order_id
                << ", Price: " << order.price
                << ", Quantity: " << order.quantity
                << std::endl;
    }
  }
}

const std::map<double, std::deque<Order>> &OrderBook::getBids() const
{
  return bids;
}

const std::map<double, std::deque<Order>> &OrderBook::getAsks() const
{
  return asks;
}
