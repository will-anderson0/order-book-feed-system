cmake_minimum_required(VERSION 3.10)
project(OrderBookFeedSystem)

set(CMAKE_CXX_STANDARD 17)

# === Include headers ===
include_directories(include)
include_directories(/opt/homebrew/include)

# === Main app executable ===
add_executable(market_data_feed
    src/main.cpp
    src/market_data_parser.cpp
    src/order_book.cpp
    src/network_thread.cpp
    src/processing_thread.cpp
    src/ticker_thread.cpp
)

# === Link libraries ===
target_link_libraries(market_data_feed
    ${Boost_LIBRARIES}
    pthread
)

# === Test executables ===

add_executable(test_market_data_parser
    tests/test_market_data_parser.cpp
    src/market_data_parser.cpp
)

add_executable(test_order_book
    tests/test_order_book.cpp
    src/order_book.cpp
)

enable_testing()
add_test(NAME test_market_data_parser COMMAND test_market_data_parser)
add_test(NAME test_order_book COMMAND test_order_book)
