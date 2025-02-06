#pragma once
#include <algorithm>
#include <atomic>
#include <random>

namespace RandomString {

static constexpr const char *DEFAULT_CHARSET =
    "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
static std::default_random_engine random{static_cast<unsigned>(time(0))};
static std::mt19937 random_generator(random());

inline std::string generate(size_t length, const std::string &charset = "") {
  std::string str = charset.empty() ? std::string(DEFAULT_CHARSET) : charset;
  while (length > str.length())
    str += str;
  std::shuffle(str.begin(), str.end(), random_generator);
  return str.substr(0, length);
}

} // namespace RandomString

class Spinlock {
  std::atomic_bool locked{false};

public:
  void lock() {
    while (locked.exchange(true))
      ;
  }
  void unlock() { locked.store(false); }
};
