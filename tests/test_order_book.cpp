#include <iostream>
#include <cassert>
#include "../include/order_book.h"

// Test 1: Add a new B order to the book
void testAddBuyOrder()
{
  OrderBook book;
  Order buyOrder1 = {"B1", 100.5, 5, SideType::B};
  book.addOrder(buyOrder1);

  assert(book.getBids().at(100.5).size() == 1);
  assert(book.getBids().at(100.5).front().order_id == "B1");

  std::cout << "Test 1 passed: Add Buy Order\n";
}

// Test 2: Add a new S order to the book
void testAddSellOrder()
{
  OrderBook book;
  Order sellOrder1 = {"S1", 101.0, 10, SideType::S};
  book.addOrder(sellOrder1);

  assert(book.getAsks().at(101.0).size() == 1);
  assert(book.getAsks().at(101.0).front().order_id == "S1");

  std::cout << "Test 2 passed: Add Sell Order\n";
}

// Test 3: Modify an existing B order

void testModifyBuyOrder()
{
  OrderBook book;
  Order buyOrder1 = {"B1", 100.5, 5, SideType::B};
  book.addOrder(buyOrder1);

  auto bids = book.getBids();
  auto it = bids.find(100.5);
  assert(it != bids.end());
  assert(it->second.front().quantity == 5);

  Order modifiedBuyOrder = {"B1", 100.5, 15, SideType::B};
  book.modifyOrder("B1", modifiedBuyOrder);

  bids = book.getBids(); // Reload after modify
  it = bids.find(100.5);
  assert(it != bids.end());
  assert(it->second.front().quantity == 15);

  std::cout << "Test 3 passed: Modify Buy Order\n";
}

// Test 4: Modify an existing S order
void testModifySellOrder()
{
  OrderBook book;
  Order sellOrder1 = {"S1", 101.0, 10, SideType::S};
  book.addOrder(sellOrder1);

  assert(book.getAsks().at(101.0).front().quantity == 10);

  Order modifiedSellOrder = {"S1", 101.0, 2, SideType::S};
  book.modifyOrder("S1", modifiedSellOrder);

  assert(book.getAsks().at(101.0).front().quantity == 2);

  std::cout << "Test 4 passed: Modify Sell Order\n";
}

// Test 5: Cancel a B order
void testCancelBuyOrder()
{
  OrderBook book;
  Order buyOrder1 = {"B1", 100.5, 5, SideType::B};
  book.addOrder(buyOrder1);

  book.cancelOrder("B1");

  auto bids = book.getBids();
  auto it = bids.find(100.5);
  assert(it == bids.end()); // 100.5 price level should not exist anymore

  std::cout << "Test 5 passed: Cancel Buy Order\n";
}

// Test 6: Cancel a S order
void testCancelSellOrder()
{
  OrderBook book;
  Order sellOrder1 = {"S1", 101.0, 10, SideType::S};
  book.addOrder(sellOrder1);

  book.cancelOrder("S1");

  auto bids = book.getBids();
  auto it = bids.find(101.0);
  assert(it == bids.end()); // 101.0 price level should not exist anymore

  std::cout << "Test 5 passed: Cancel Buy Order\n";
}

// Test 7: Print the order book (manual check)
void testPrintOrderBook()
{
  OrderBook book;
  Order buyOrder1 = {"B1", 100.5, 5, SideType::B};
  Order sellOrder1 = {"S1", 101.0, 10, SideType::S};
  book.addOrder(buyOrder1);
  book.addOrder(sellOrder1);

  std::cout << "\nTest 7 output: Order Book Print\n";
  book.printOrderBook();
  std::cout << "Test 7 passed: Print Order Book\n";
}

// Test 8: Handle adding an invalid side
void testInvalidSide()
{
  OrderBook book;
  Order invalidOrder = {"X1", 99.9, 10, static_cast<SideType>('X')};
  book.addOrder(invalidOrder);

  // Should not add to bids or asks
  assert(book.getBids().empty());
  assert(book.getAsks().empty());

  std::cout << "Test 8 passed: Invalid Side Handling\n";
}

// Test 9: Handle modifying a non-existing order
void testModifyInvalidOrder()
{
  OrderBook book;
  Order dummyOrder = {"D1", 100.0, 5, SideType::B};
  book.modifyOrder("nonexistent_id", dummyOrder);

  // Should not crash, nothing to assert unless you want to check size stays zero
  assert(book.getBids().empty());
  assert(book.getAsks().empty());

  std::cout << "Test 9 passed: Modify Invalid Order\n";
}

// Test 10: Handle cancelling a non-existing order
void testCancelInvalidOrder()
{
  OrderBook book;
  book.cancelOrder("nonexistent_id");

  // Should not crash
  assert(book.getBids().empty());
  assert(book.getAsks().empty());

  std::cout << "Test 10 passed: Cancel Invalid Order\n";
}

int main()
{
  testAddBuyOrder();
  testAddSellOrder();
  testModifyBuyOrder();
  testModifySellOrder();
  testCancelBuyOrder();
  testCancelSellOrder();
  testPrintOrderBook();
  testInvalidSide();
  testModifyInvalidOrder();
  testCancelInvalidOrder();

  std::cout << "\nAll tests passed successfully!\n";
  return 0;
}
