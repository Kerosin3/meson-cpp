#pragma once
#include <atomic>
#include <chrono>
#include <format>
#include <queue>
#include <semaphore>
#include <string>
#include <iostream>

// struct to pass data to writers
struct CmdQueqe
{
    using data_t = std::pair<std::string, std::string>;
    std::mutex qmtx;
    std::queue<data_t> dqueue;
    std::string blockname;
    bool readed = false;
    std::atomic_bool disconnet = false;
    int processed = 0;
    bool processing_done = false;
    std::counting_semaphore<1> start{0};
    CmdQueqe() = default;
    CmdQueqe(CmdQueqe&& cmd) noexcept {};
};

class Commands
{
  public:
    Commands() = delete;
    ~Commands() = default;
    explicit Commands(std::string cmd) :
        m_Cmd{std::move(cmd)}, m_Timestamp{std::chrono::system_clock::to_time_t(
                                   std::chrono::system_clock::now())}
    {}
    [[nodiscard]] std::string getCmd() const
    {
        return std::format("{}", m_Cmd);
    }
    [[nodiscard]] std::string getTimestamp()
    {
        // add atomic int to serial in order to make each timestamp unique
        // std::cout << "call\n";
        return std::format("{}{:0>3}", m_Timestamp,
                           serial.fetch_add(1, std::memory_order_seq_cst));
    }
    std::pair<std::string, std::string> getWhole()
    {
        return {getCmd(), getTimestamp()};
    }

  private:
    std::string m_Cmd;
    time_t m_Timestamp;
    static inline std::atomic_size_t serial{0};
};
