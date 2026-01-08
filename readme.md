# Order Matching Engine (C++)

A single-threaded, in-memory order matching engine implemented in C++ for high-performance trading simulations.

## Features

- **Order Types**: Supports BUY and SELL orders, including LIMIT and MARKET orders.
- **Matching Logic**: Implements price-time priority with partial fills and FIFO matching at each price level.
  - BUY orders match against the lowest available SELL price.
  - SELL orders match against the highest available BUY price.
  - LIMIT orders halt when the price exceeds the specified limit.
  - MARKET orders execute until fully filled or the order book is exhausted.
- **Priority**: Orders at the same price are processed in FIFO order.

## Performance

- Single-threaded, in-memory core optimized for speed.
- Average latency: ~6–7 µs per order.
- Scales linearly, tested with up to 50,000 orders.

## Benchmark Results
- Average Latency 500 orders: 2.3 ms.
- Average Latency 1,000 orders: 4.4 ms.
- Average Latency 10,000 orders: 59 ms.
- Average Latency 50,000 orders: 344 ms.


## Input and Integration

- Orders are ingested via a local API endpoint using httplib Library.
- Test order flows can be generated using the included Python script.

## Contributing

Feel free to submit issues or pull requests for improvements to the matching logic.
