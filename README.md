# Market Data Feed Handler + Real-Time Order Book

## Overview

Ultra-low latency market data feed handler written in C++, maintaining a real-time in-memory order book and exposing a WebSocket API for local access. A Python dashboard is included for live visualization of market depth.

## Features

- TCP-based feed ingestion from a simulated market data stream
- Multithreaded, real-time in-memory order book
- WebSocket server exposing live snapshots in JSON format
- Python Dash frontend for market visualization
- Unit-tested architecture

## Project Structure

| Folder       | Description                                  |
| ------------ | -------------------------------------------- |
| `/src`       | Core C++ source files                        |
| `/include`   | Header files for shared types/interfaces     |
| `/tests`     | Unit tests for order book and parser         |
| `/scripts`   | Python feed simulator that sends fake orders |
| `/dashboard` | Python Dash app for live book visualization  |

## Getting Started

### Step 1: Start the market data feed simulator

This Python script creates a TCP server and sends random `ADD`, `MODIFY`, and `CANCEL` messages.

```bash
cd scripts
python3 feed_simulator.py
```

### Step 2: Build and run the C++ engine

```bash
cd ..
cmake -S . -B build
cmake --build build
./build/market_data_feed
```

### Step 3: Launch the live Python dashboard

```bash
cd dashboard
source ../.venv/bin/activate
python live_order_book_dash.py
```
