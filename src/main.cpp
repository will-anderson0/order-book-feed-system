#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <mutex>

#include "../include/order_book.h"
#include "../include/thread_safe_queue.h"
#include "../include/market_data_parser.h"

// Declare shared resources
std::atomic<bool> running(true);
std::atomic<size_t> message_count(0);
ThreadSafeQueue<OrderEvent> event_queue;
OrderBook book;
std::mutex book_mutex;

// Function declarations
void networkThread();
void processingThread();
void tickerThread();

int main()
{
    std::cout << "Starting multithreaded order book...\n";

    std::thread network(networkThread);
    std::thread processor(processingThread);
    std::thread ticker(tickerThread);

    std::this_thread::sleep_for(std::chrono::seconds(10)); // Run system for 5 seconds

    running = false; // Signal shutdown

    network.join();
    processor.join();
    ticker.join();

    std::cout << "\nFinal Order Book State:\n";
    {
        std::lock_guard<std::mutex> lock(book_mutex);
        book.printOrderBook();
    }

    std::cout << "\nMessages received: " << message_count.load() << std::endl;
    std::cout << "Approximate throughput: "
              << message_count.load() / 5.0 << " messages/second" << std::endl;

    std::cout << "Shutdown complete.\n";
    return 0;
}
