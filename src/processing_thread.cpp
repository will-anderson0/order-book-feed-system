#include <iostream>
#include "../include/market_data_parser.h"
#include "../include/thread_safe_queue.h"
#include "../include/order_book.h"

extern std::atomic<bool> running;
extern ThreadSafeQueue<OrderEvent> event_queue;
extern OrderBook book;
extern std::mutex book_mutex;

void processingThread()
{
  while (running || !event_queue.empty())
  {
    OrderEvent event;
    if (event_queue.try_pop(event))
    {
      std::lock_guard<std::mutex> lock(book_mutex);

      // Convert OrderEvent to an Order
      Order order = {
          event.order_id,
          event.price,
          event.quantity,
          event.side};

      if (event.type == EventType::ADD)
      {
        book.addOrder(order);
      }
      else if (event.type == EventType::MODIFY)
      {
        book.modifyOrder(order.order_id, order);
      }
      else if (event.type == EventType::CANCEL)
      {
        book.cancelOrder(order.order_id);
      }
    }
  }
}
