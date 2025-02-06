#include "printers.hpp"

void ConsolePrinter::run()
{
    thr = std::thread([&]() {
        while (true)
        {
            may_print.acquire();
            if (!data || data->disconnet || str_data.empty())
            {
                break;
            }
            std::cout << "bulk:\n";
            while (!str_data.empty())
            {
                std::cout << str_data.front() << " ";
                str_data.pop();
            }
            std::cout << std::endl;
        }
    });
    thr.detach();
}

void FilePrinter::initialize()
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
                if (data->dqueue.empty())
                    continue;
                auto& [elem, timestampx] = data->dqueue.front();
                timestampx += "_" + printer_name;
                auto& fname = data->blockname;
				auto _fname = std::format("bulk{}.log",fname);
                std::ofstream myfile(_fname, std::ios::out | std::ios::app);
                auto data_to_write = std::format(
                    "writer >>{}<<, CMD: [{}], timestamp_writer: {}", m_serial,
                    elem, timestampx);
                myfile << data_to_write << std::endl;
                // other thread may use hon
                data->dqueue.pop();
                data->processed--;
                data->readed = false;
                a_lock.unlock();
                cvx.notify_all();
            }
        }
    });
}

