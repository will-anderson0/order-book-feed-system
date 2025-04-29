#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <iostream>
#include <string>
#include <mutex>
#include <set>
#include "../include/order_book.h"

typedef websocketpp::server<websocketpp::config::asio> server;

extern OrderBook book;
extern std::mutex book_mutex;
extern std::atomic<bool> running;

server ws_server;
std::set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> clients;

// This helper sends snapshots to all connected clients
void send_snapshot()
{
  std::string snapshot;

  {
    std::lock_guard<std::mutex> lock(book_mutex);

    snapshot += "{ \"bids\": [";
    bool first = true;
    for (auto it = book.getBids().rbegin(); it != book.getBids().rend(); ++it)
    {
      for (const auto &order : it->second)
      {
        if (!first)
          snapshot += ",";
        snapshot += "{ \"price\":" + std::to_string(order.price) +
                    ", \"quantity\":" + std::to_string(order.quantity) +
                    ", \"id\":\"" + order.order_id + "\" }";
        first = false;
      }
    }
    snapshot += "], \"asks\": [";
    first = true;
    for (const auto &[price, orders] : book.getAsks())
    {
      for (const auto &order : orders)
      {
        if (!first)
          snapshot += ",";
        snapshot += "{ \"price\":" + std::to_string(order.price) +
                    ", \"quantity\":" + std::to_string(order.quantity) +
                    ", \"id\":\"" + order.order_id + "\" }";
        first = false;
      }
    }
    snapshot += "] }";
  }

  for (const auto &hdl : clients)
  {
    try
    {
      ws_server.send(hdl, snapshot, websocketpp::frame::opcode::text);
    }
    catch (const websocketpp::exception &e)
    {
      std::cout << "Send failed: " << e.what() << std::endl;
    }
  }
}

void on_open(websocketpp::connection_hdl hdl)
{
  std::cout << "Client connected\n";
  clients.insert(hdl);
}

void on_close(websocketpp::connection_hdl hdl)
{
  std::cout << "Client disconnected\n";
  clients.erase(hdl);
}

void on_message(server *s, websocketpp::connection_hdl hdl, server::message_ptr msg)
{
  if (msg->get_payload() == "snapshot")
  {
    send_snapshot();
  }
  else
  {
    s->send(hdl, "Unknown command", websocketpp::frame::opcode::text);
  }
}

void websocketThread()
{
  ws_server.set_open_handler(&on_open);
  ws_server.set_close_handler(&on_close);
  ws_server.set_message_handler(std::bind(&on_message, &ws_server, std::placeholders::_1, std::placeholders::_2));

  ws_server.init_asio();
  ws_server.listen(9001);
  ws_server.start_accept();

  std::cout << "WebSocket server listening on port 9001\n";
  ws_server.run();
}
