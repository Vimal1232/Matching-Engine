# Order Matching Engine (C++)

A single-threaded, in-memory order matching engine implemented in C++ to study performance characteristics of exchange-style order books.

## Order Types

- **BUY / SELL**: Support for buy and sell orders.
- **LIMIT and MARKET orders**: Limit orders execute at a specified price or better; market orders execute at the best available price.


## Matching Rules

- **BUY orders** match against the lowest available SELL price.
- **SELL orders** match against the highest available BUY price.
- **LIMIT orders** stop when the limit price is breached.
- **MARKET orders** execute until fully filled or the order book is exhausted.

## Design Notes

- Price levels are stored using integer-based prices to avoid floating-point precision issues.
- Orders are Maintained in HashMap of Price Buckets.
- Best bid/ask selection uses priority-based data structures.
- Single-threaded by design to keep behavior deterministic and easy to reason about.

## Input & Integration

Orders are ingested via a local API endpoint using the httplib library. The engine expects a JSON array of orders from the server.

Example order format:

```json
[
  {
    "id": "order1",
    "price": 100.0,
    "qty": 10.0,
    "side": "BUY",
    "tif": "GTC",
    "timestamp": 1640995200,
    "type": "LIMIT"
  }
]
```

## API

The engine fetches orders from a local HTTP server. To integrate:

1. Start a local server serving JSON order data.
2. Run the engine to process the orders.

## Performance

- **Single-threaded, in-memory matching core**
- Benchmarks run on a local machine (matching only, no persistence)
- Latency measured as batch execution time

### Benchmark Results

| Order Count | Avg Batch Latency |
| ----------- | ----------------- |
| 500         | ~2.3 ms           |
| 1,000       | ~4.4 ms           |
| 10,000      | ~59 ms            |
| 50,000      | ~344 ms           |

This corresponds to an average ~5–7 µs per order in the tested range. Observed performance grows proportionally with order volume within the tested limits.

## Disclaimer

This project is not production-ready and does not aim to replicate the complexity or throughput of real-world trading venues. It is intended as a learning and experimentation project focused on correctness, data structures, and performance trade-offs.
