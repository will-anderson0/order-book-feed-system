#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <iostream>
#include <string>
#include <mutex>
#include "../include/order_book.h"

typedef websocketpp::server<websocketpp::config::asio> server;

extern OrderBook book;
extern std::mutex book_mutex;

void on_message(server *s, websocketpp::connection_hdl hdl, server::message_ptr msg)
{
  std::string payload = msg->get_payload();
  std::cout << "Received: " << payload << std::endl;

  if (payload == "snapshot")
  {
    std::string snapshot;

    {
      std::lock_guard<std::mutex> lock(book_mutex);

      snapshot += "Bids:\n";
      for (const auto &[price, orders] : book.getBids())
      {
        for (const auto &order : orders)
        {
          snapshot += "OrderID: " + order.order_id + ", Price: " + std::to_string(order.price) + ", Qty: " + std::to_string(order.quantity) + "\n";
        }
      }

      snapshot += "Asks:\n";
      for (const auto &[price, orders] : book.getAsks())
      {
        for (const auto &order : orders)
        {
          snapshot += "OrderID: " + order.order_id + ", Price: " + std::to_string(order.price) + ", Qty: " + std::to_string(order.quantity) + "\n";
        }
      }
    }

    s->send(hdl, snapshot, websocketpp::frame::opcode::text);
  }
  else
  {
    s->send(hdl, "Unknown command", websocketpp::frame::opcode::text);
  }
}

int main()
{
  server ws_server;

  try
  {
    ws_server.set_message_handler(std::bind(&on_message, &ws_server, std::placeholders::_1, std::placeholders::_2));
    ws_server.init_asio();
    ws_server.listen(9001);
    ws_server.start_accept();
    std::cout << "WebSocket server listening on port 9001" << std::endl;
    ws_server.run();
  }
  catch (const std::exception &e)
  {
    std::cout << "Exception: " << e.what() << std::endl;
  }
}
