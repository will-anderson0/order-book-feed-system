# Market Data Feed Handler + Real-Time Order Book

## Overview

Ultra-low latency market data feed handler in C++, maintaining a real-time in-memory order book and exposing a local API for visualization.

## Features

- TCP/WebSocket market data ingestion
- Sub-millisecond real-time order book updates
- Multithreaded event-driven architecture
- Local WebSocket API for external access
- Python dashboard for live market depth visualization

## Project Structure

- `/src` - Core C++ source files
- `/include` - Header files
- `/tests` - Unit tests
- `/dashboard` - Real-time Python dashboard
- `/scripts` - Automation scripts
- `/data` - Sample market feeds
- `/docs` - System design notes

## Build Instructions

```bash
mkdir build
cd build
cmake ..
make
./market_data_feed
```
