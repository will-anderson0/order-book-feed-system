cmake_minimum_required(VERSION 3.10)
project(OrderBookFeedSystem)

cmake_policy(SET CMP0167 NEW)

set(CMAKE_CXX_STANDARD 17)

# === Boost manually set for macOS/Homebrew ===
set(Boost_INCLUDE_DIR /opt/homebrew/include)
set(Boost_LIBRARY_DIR /opt/homebrew/lib)

find_package(Boost REQUIRED COMPONENTS system thread)

include_directories(include)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

# === Executable ===
add_executable(market_data_feed
    src/main.cpp
    src/market_data_parser.cpp
    src/order_book.cpp
    src/network_thread.cpp
    src/processing_thread.cpp
    src/ticker_thread.cpp
    src/websocket_server.cpp
)

# === Link Boost and pthread ===
target_link_libraries(market_data_feed
    ${Boost_LIBRARIES}
    pthread
)

# === Tests (optional) ===
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
