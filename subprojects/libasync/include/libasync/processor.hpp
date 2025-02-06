#pragma once
#include "printers.hpp"

#include <condition_variable>
#include <cstddef>
#include <istream>
#include <memory>
#include <mutex>
#include <semaphore>
#include <sstream>
#include <string>
#include <thread>

namespace Proc
{

struct CmdProcessor
{
    std::mutex cmd_data_mtx;
    std::condition_variable cv_data;
    bool input_aquired{false};
    bool input_processed{false};
    std::shared_ptr<CmdQueqe> data;
    std::condition_variable& cvx;
    std::unique_ptr<std::thread>& thr;
    std::istringstream& datasource;
    std::vector<CmdQueqe::data_t> pull;
    std::thread c_thr;
    size_t blocksize{};
    ConsolePrinter consPrinter;
    size_t m_NOpenBracets{0};
    size_t m_NCloseBracets{0};
    std::queue<std::string> buf_print;

    CmdProcessor(std::shared_ptr<CmdQueqe> qdata, std::condition_variable& cv,
                 std::unique_ptr<std::thread>& thx, std::istringstream& dsource,
                 size_t bs) :
        data(qdata), cvx(cv), thr(thx), datasource(dsource), blocksize(bs),
        consPrinter(c_thr, data, buf_print)
    {
        consPrinter.run();
    }

    void initialize();
};

class ProcessorHub
{
    std::shared_ptr<CmdQueqe> data;
    const int PRINTER1_SERIAL = 1;
    const int PRINTER2_SERIAL = 2;
    size_t m_blocksize{};
    std::condition_variable cvx;
    std::istringstream datasource;
    std::unique_ptr<std::thread> r_thr;
    CmdProcessor m_cmdProcessor;
    std::unique_ptr<std::thread> p_thr1;
    FilePrinter filePrinter1;
    std::unique_ptr<std::thread> p_thr2;
    FilePrinter filePrinter2;
    Spinlock writerSlock{};
    Spinlock sSlock{};
    bool initialized{false};

  public:
    explicit ProcessorHub(size_t blocksize) :
        data{std::make_shared<CmdQueqe>(CmdQueqe{})}, m_blocksize(blocksize),
        r_thr(std::make_unique<std::thread>(std::thread{})),
        m_cmdProcessor{data, cvx, r_thr, datasource, blocksize},
        p_thr1(std::make_unique<std::thread>(std::thread{})),
        filePrinter1{data,      cvx,        p_thr1, PRINTER1_SERIAL,
                     blocksize, writerSlock},
        p_thr2(std::make_unique<std::thread>(std::thread{})),
        filePrinter2{data, cvx, p_thr2, PRINTER2_SERIAL, blocksize, writerSlock}
    {}
    ProcessorHub& operator=(ProcessorHub&&) = delete;
    ProcessorHub& operator=(ProcessorHub) = delete;
    ProcessorHub(ProcessorHub&) = delete;
    ProcessorHub(ProcessorHub&&) = delete;
    void readerStart()
    {
        m_cmdProcessor.initialize();
    }
    void printersStart()
    {
        filePrinter1.initialize();
        filePrinter2.initialize();
    }
    void finish();
    void receive_input(std::string& sdata);
    ~ProcessorHub();
};

} // namespace Proc
