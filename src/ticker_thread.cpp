#include <iostream>
#include <chrono>
#include <thread>
#include "../include/order_book.h"

extern std::atomic<bool> running;
extern OrderBook book;
extern std::mutex book_mutex;

void tickerThread()
{
  while (running)
  {
    {
      std::lock_guard<std::mutex> lock(book_mutex);

      // std::cout << "\033[2J\033[1;1H"; // Clear screen

      std::cout << "=== Live Order Book Snapshot ===\n";

      std::cout << "Bids:\n";
      for (auto it = book.getBids().rbegin(); it != book.getBids().rend(); ++it)
      {
        for (const auto &order : it->second)
        {
          std::cout << "Price: " << order.price
                    << " | Size: " << order.quantity
                    << " | ID: " << order.order_id << "\n";
        }
      }

      std::cout << "Asks:\n";
      for (const auto &[price, orders] : book.getAsks())
      {
        for (const auto &order : orders)
        {
          std::cout << "Price: " << order.price
                    << " | Size: " << order.quantity
                    << " | ID: " << order.order_id << "\n";
        }
      }

      std::cout << "===============================\n";
    }

    std::this_thread::sleep_for(std::chrono::seconds(1)); // Print every second
  }
}
