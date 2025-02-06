#pragma once
#include "commands.hpp"
#include "misc.hpp"

#include <condition_variable>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
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
    void run();
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
    {}
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

    void initialize() override;
};
