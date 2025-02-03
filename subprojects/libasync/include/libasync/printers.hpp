#pragma once
#include "commands.hpp"
#include "misc.hpp"

#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <format>
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <ostream>
#include <queue>
#include <string>
#include <thread>

class ConsolePrinter
{
    std::thread& thr;
    std::shared_ptr<CmdQueqe> data;
    std::binary_semaphore may_print{false};
    std::queue<std::string>& str_data;

  public:
    explicit ConsolePrinter(std::thread& thx, std::shared_ptr<CmdQueqe> cdata,
                            std::queue<std::string>& dat) :
        thr(thx), data(cdata), str_data(dat)
    {}
    void print()
    {
        may_print.release();
    }
    void lock()
    {
        may_print.acquire();
    }
    void run()
    {
        thr = std::thread([&]() {
            while (true)
            {
                may_print.acquire();
                if (!data || data->disconnet || str_data.empty())
                {
                    break;
                }
                std::cout << "block:\n";
                while (!str_data.empty())
                {
                    std::cout << str_data.front() << " ";
                    str_data.pop();
                }
                std::cout << std::endl;
            }
            std::cout << "console cycle out\n";
        });
    }
    ~ConsolePrinter()
    {
        std::cout << "CP DIES" << "\n";
    }
};

struct iPrinter
{
    std::shared_ptr<CmdQueqe> data;
    std::condition_variable& cvx;
    std::unique_ptr<std::thread>& thr;
    int m_serial;
    size_t blocksize;
    Spinlock& s_lock;

    iPrinter(std::shared_ptr<CmdQueqe> qdata, std::condition_variable& cv,
             std::unique_ptr<std::thread>& thx, int serial, size_t bs,
             Spinlock& sl) :
        data(qdata), cvx(cv), thr(thx), m_serial(serial), blocksize(bs),
        s_lock(sl)
    {
        std::cout << "printer creation\n";
    }
    std::string printer_name{RandomString::generate(5)};

    virtual void initialize() = 0;
    virtual ~iPrinter() = default;
};

struct FilePrinter : iPrinter
{
    FilePrinter(std::shared_ptr<CmdQueqe> qdata, std::condition_variable& cv,
                std::unique_ptr<std::thread>& thx, int serial, size_t bs,
                Spinlock& sl) : iPrinter(qdata, cv, thx, serial, bs, sl)
    {}

    virtual void initialize() override
    {
        *thr = std::thread([&]() {
            while (!data->disconnet)
            {
                {
                    std::unique_lock a_lock(data->qmtx);
                    cvx.wait(a_lock, [this] {
                        return data->readed || data->processing_done ||
                               data->processed || data->disconnet;
                    });
                    // s_lock.lock();
                    if (data->dqueue.empty())
                        break;
                    auto& [elem, timestampx] = data->dqueue.front();
                    timestampx += "_" + printer_name;
                    auto& fname = data->blockname;
                    std::cout << "written block " << elem << "\n";

                    std::ofstream myfile(fname, std::ios::out | std::ios::app);
                    auto data_to_write = std::format(
                        "writer >>{}<<, CMD: [{}], timestamp: {} fname: {}",
                        m_serial, elem, timestampx, fname);
                    myfile << data_to_write << std::endl;
                    // other thread may use hon
                    data->dqueue.pop();
                    if (data->processing_done)
                        break;
                    // s_lock.unlock();
                    data->processed--;
                    data->readed = false;
                    a_lock.unlock();
                    cvx.notify_all();
                }
            }
            std::cout << "file writer cycle out\n";
        });
    }
    ~FilePrinter() override
    {
        if (thr->joinable())
            thr->join();
        std::cout << "writed dies\n";
    }
};
