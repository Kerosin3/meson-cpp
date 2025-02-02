#pragma once
#include "printers.hpp"

#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <iostream>
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
    CmdQueqe& data;
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

    CmdProcessor(CmdQueqe& qdata, std::condition_variable& cv,
                 std::unique_ptr<std::thread>& thx, std::istringstream& dsource,
                 size_t bs) :
        data(qdata), cvx(cv), thr(thx), datasource(dsource), blocksize(bs),
        consPrinter(c_thr, data, buf_print)
    {
        consPrinter.run();
        c_thr.detach();
    }

    void initialize()
    {
        *thr = std::thread([&]() {
            std::string TmpString;
            auto isOpBrace = [](char symbol) -> bool { return symbol == '{'; };
            auto isClBrace = [](char symbol) -> bool { return symbol == '}'; };
            auto exec_write = [&]() {
                std::lock_guard guard(data.qmtx);
                data.blockname = pull.front().second;
                for (const auto& elem : pull)
                {
                    data.dqueue.push(elem);
                    buf_print.push(elem.first);
                    data.processed++;
                }
                consPrinter.print();
                pull.clear();
                data.readed = true;
                cvx.notify_one();
            };
            auto clean_n = [&]() {
                m_NCloseBracets = 0;
                m_NOpenBracets = 0;
            };
            // std::istringstream {input_data};
            while (!data.disconnet)
            {
                std::cout << "start endless\n";
                std::unique_lock d_lock(cmd_data_mtx);
                cv_data.wait(d_lock, [this] {
                    return input_aquired && !input_processed;
                });
                std::cout << "analyze\n";
                if (data.disconnet)
                {
                    std::cout << "BREAK!\n";
                    break;
                }
                // if (!datasource.good())
                // break;
                while (std::getline(datasource, TmpString, '\n'))
                {
                    if (TmpString.empty())
                        continue;
                    std::cout << "data -> " << TmpString << "\n";
                    Commands cmd{TmpString};
                    auto current_cmd = cmd.getWhole();
                    // open bracet
                    if (isOpBrace(TmpString.at(0)))
                    {
                        if (m_NOpenBracets == 0 && !pull.empty())
                        {
                            exec_write();
                        }
                        m_NOpenBracets++;
                    }
                    else if (isClBrace(TmpString.at(0)))
                    {
                        if (m_NOpenBracets > 0)
                        {
                            m_NCloseBracets++;
                            if (m_NCloseBracets == m_NOpenBracets &&
                                !pull.empty())
                            {
                                exec_write();
                            }
                            clean_n();
                        }
                        else
                        {
                            if (m_NCloseBracets == m_NOpenBracets &&
                                !pull.empty())
                            {
                                exec_write();
                            }
                            std::cout << "----\n";
                            clean_n();
                        }
                    }
                    else
                    {
                        pull.push_back(current_cmd);
                        if ((!(pull.size() % blocksize) && m_NOpenBracets == 0))
                        {
                            std::lock_guard guard(data.qmtx);
                            // get first command timestamp as filename
                            data.blockname = pull.front().second;
                            for (const auto& elem : pull)
                            {
                                buf_print.push(elem.first);
                                data.dqueue.push(elem);
                            }
                            pull.clear();
                            consPrinter.print();
                            data.readed = true;
                            data.processed++;
                            cvx.notify_one();
                        }
                    }

                    std::unique_lock lk(data.qmtx);
                    cvx.wait(lk, [this] { return !data.readed; });
                }
                // exec_write();

                std::lock_guard guard(data.qmtx);
                data.blockname = pull.front().second;
                for (const auto& elem : pull)
                {
                    data.dqueue.push(elem);
                    buf_print.push(elem.first);
                    data.processed++;
                }
                consPrinter.print();
                data.readed = true;
                cvx.notify_one();
                clean_n();
                pull.clear();
                input_processed = true;
                input_aquired = false;
                std::cout << "input PROCESSED\n";
                cv_data.notify_all();
                std::cout << "cycle endless\n";
            }
            consPrinter.print();
            data.processing_done = true;
            cvx.notify_all();
            std::cout << "io cycle out\n";
        });
        // detach thread
        // thr->detach();
    }

    ~CmdProcessor()
    {
        std::cout << "ioreader died\n";
    }
};

class ProcessorHub
{
    CmdQueqe data;
    const int PRINTER1_SERIAL = 1;
    const int PRINTER2_SERIAL = 2;
    size_t m_blocksize{};
    std::condition_variable cvx;
    std::istringstream datasource;
    std::unique_ptr<std::thread> r_thr;
    CmdProcessor m_processos;
    std::unique_ptr<std::thread> p_thr1;
    FilePrinter filePrinter1;
    std::unique_ptr<std::thread> p_thr2;
    FilePrinter filePrinter2;
    //   std::unique_ptr<std::thread> c_thr;
    //   ConsolePrinter consolePrinter;

    // protect writes in case..
    Spinlock writerSlock{};
    Spinlock sSlock{};

  public:
    explicit ProcessorHub(size_t blocksize) :
        m_blocksize(blocksize),
        r_thr(std::make_unique<std::thread>(std::thread{})),
        m_processos{data, cvx, r_thr, datasource, blocksize},
        p_thr1(std::make_unique<std::thread>(std::thread{})),
        filePrinter1{data,      cvx,        p_thr1, PRINTER1_SERIAL,
                     blocksize, writerSlock},
        p_thr2(std::make_unique<std::thread>(std::thread{})),
        filePrinter2{data, cvx, p_thr2, PRINTER2_SERIAL, blocksize, writerSlock}
    {
        // start reader
        m_processos.initialize();
        printersStart();
    }
    void readerStart()
    {
        m_processos.initialize();
    }
    void printersStart()
    {
        filePrinter1.initialize();
        filePrinter2.initialize();
    }
    void finish()
    {
        std::cout << "finishing\n";
        data.disconnet = true;
        // uniqe lock
        // wait
        // data.processing_done = true;
    }
    void receive_input(std::string& sdata)
    {
        // std::lock_guard lock(m_processos.cmd_data_mtx);
        // }
        // std::cout << "input data:\n" << sdata << "\n";
        std::cout << "AQUIRED DATA\n";
        // std::lock_guard lock(m_processos.cmd_data_mtx);
        datasource = std::istringstream{sdata};
        m_processos.input_aquired = true;
        m_processos.cv_data.notify_all();
        std::unique_lock lock(m_processos.cmd_data_mtx);
        std::cout << "Waiting\n";
        m_processos.cv_data.wait(lock, [this] {
            return m_processos.input_processed;
        });

        std::cout << "!!!recv done!\n";
        m_processos.input_processed = false;
        m_processos.cv_data.notify_all();
    }
    ~ProcessorHub()
    {
        m_processos.input_processed = false;
        m_processos.input_aquired = true;
        data.processing_done = true;
        filePrinter1.cvx.notify_all();
        filePrinter2.cvx.notify_all();
        m_processos.cv_data.notify_all();
        std::cout << "Hub dies!\n";
        p_thr1->join();
        p_thr2->join();
        r_thr->join();
    }

  private:
    void process()
    {
        if (r_thr->joinable())
        {
            r_thr->join();
        }
        else
        {
            throw std::runtime_error("error joining reader");
        }

        if (p_thr1->joinable())
        {
            p_thr1->join();
        }
        else
        {
            throw std::runtime_error("error joining reader");
        }

        if (p_thr2->joinable())
        {
            p_thr2->join();
        }
        else
        {
            throw std::runtime_error("error joining reader");
        }
    }
};

} // namespace Proc
