#ifndef Order_H
#define Order_H
#include <external/httplib.h>
#include <sys/_types/_int64_t.h>

#include <optional>
#include <queue>
#include <vector>

#include "external/json.hpp"

struct Order {
  std::string id;
  std::optional<int64_t> price;
  int64_t Qty;
  std::string side;
  std::string tif;
  int64_t timestamp;
  std::string type;
};

class Compare {
 public:
  bool operator()(const Order& a, const Order& b) const {
    if (a.timestamp > b.timestamp) {
      return true;
    } else if (a.timestamp == b.timestamp) {
      if (a.price > b.price) {
        return true;
      } else {
        return false;
      }
    }

    return false;
  }
};

std::priority_queue<Order, std::vector<Order>, Compare> Order_Book();

#endif