#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <sstream>
#include <iostream>

#include "../include/market_data_parser.h"
#include "../include/thread_safe_queue.h"
#include "../include/order_book.h"

extern std::atomic<bool> running;
extern ThreadSafeQueue<OrderEvent> event_queue;
extern std::atomic<size_t> message_count;

void networkThread()
{
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0)
  {
    perror("Socket creation error");
    return;
  }

  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(9000);

  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
  {
    perror("Invalid address / Address not supported");
    return;
  }

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
  {
    perror("Connection Failed");
    return;
  }

  std::cout << "Connected to Python feed simulator.\n";

  char buffer[1024] = {0};
  std::string leftover;
  MarketDataParser parser;

  while (running)
  {
    int valread = read(sock, buffer, sizeof(buffer) - 1);
    if (valread > 0)
    {
      buffer[valread] = '\0';
      std::string data = leftover + std::string(buffer);

      std::istringstream stream(data);
      std::string line;
      leftover.clear();

      while (std::getline(stream, line))
      {
        if (line.empty())
          continue;

        if (stream.eof() && data.back() != '\n')
        {
          leftover = line;
          break;
        }

        OrderEvent event = parser.parseMessage(line);
        event_queue.push(event);
        message_count++;
      }
    }
    else if (valread == 0)
    {
      // Connection closed
      break;
    }
    else
    {
      perror("Read error");
      break;
    }
  }

  close(sock);
}
