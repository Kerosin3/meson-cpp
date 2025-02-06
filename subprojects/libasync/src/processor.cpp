#include "processor.hpp"
namespace Proc
{
void Proc::CmdProcessor::initialize()
{
    *thr = std::thread([&]() {
        std::string TmpString;
        auto isOpBrace = [](char symbol) -> bool { return symbol == '{'; };
        auto isClBrace = [](char symbol) -> bool { return symbol == '}'; };
        auto exec_write = [&]() {
            std::lock_guard guard(data->qmtx);
            data->blockname = pull.front().second;
            for (const auto& elem : pull)
            {
                data->dqueue.push(elem);
                buf_print.push(elem.first);
                data->processed++;
            }
            consPrinter.print();
            pull.clear();
            data->readed = true;
            cvx.notify_one();
        };
        auto clean_n = [&]() {
            m_NCloseBracets = 0;
            m_NOpenBracets = 0;
        };
        while (!data->disconnet)
        {
            std::unique_lock d_lock(cmd_data_mtx);
            cv_data.wait(d_lock,
                         [this] { return input_aquired && !input_processed; });
            if (data->disconnet)
            {
                break;
            }
            while (std::getline(datasource, TmpString, '\n'))
            {
                if (TmpString.empty())
                    continue;
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
                        if (m_NCloseBracets == m_NOpenBracets && !pull.empty())
                        {
                            exec_write();
                        }
                        clean_n();
                    }
                    else
                    {
                        if (m_NCloseBracets == m_NOpenBracets && !pull.empty())
                        {
                            exec_write();
                        }
                        clean_n();
                    }
                }
                else
                {
                    pull.push_back(current_cmd);
                    if ((!(pull.size() % blocksize) && m_NOpenBracets == 0))
                    {
                        std::lock_guard guard(data->qmtx);
                        data->blockname = pull.front().second;
                        for (const auto& elem : pull)
                        {
                            buf_print.push(elem.first);
                            data->dqueue.push(elem);
                        }
                        pull.clear();
                        consPrinter.print();
                        data->readed = true;
                        cvx.notify_one();
                    }
                }

                std::unique_lock lk(data->qmtx);
                cvx.wait(lk, [this] { return !data->readed; });
            }
            if (!pull.empty())
            {
                data->blockname = pull.front().second;

                for (const auto& elem : pull)
                {
                    data->dqueue.push(elem);
                    buf_print.push(elem.first);
                    data->processed++;
                }
                consPrinter.print();
                data->readed = true;
                cvx.notify_one();
                clean_n();
                pull.clear();
            }
            input_processed = true;
            input_aquired = false;
            cv_data.notify_all();
        }
        consPrinter.print();
        data->processing_done = true;
        cvx.notify_all();
    });
}
// processor hub
void Proc::ProcessorHub::receive_input(std::string& sdata)
{
    if (!initialized)
    {
        readerStart();
        printersStart();
    }
    initialized = true;
    datasource = std::istringstream{sdata};
    m_cmdProcessor.input_aquired = true;
    m_cmdProcessor.cv_data.notify_all();
    std::unique_lock lock(m_cmdProcessor.cmd_data_mtx);
    m_cmdProcessor.cv_data.wait(lock, [this] {
        return m_cmdProcessor.input_processed;
    });
    m_cmdProcessor.input_processed = false;
    m_cmdProcessor.cv_data.notify_all();
}

ProcessorHub::~ProcessorHub()
{
    m_cmdProcessor.input_processed = false;
    m_cmdProcessor.input_aquired = true;
    data->disconnet = true;
    filePrinter1.cvx.notify_all();
    filePrinter2.cvx.notify_all();
    m_cmdProcessor.cv_data.notify_all();

    if (p_thr1->joinable())
        p_thr1->join();
    if (p_thr2->joinable())
        p_thr2->join();
    if (r_thr->joinable())
        r_thr->join();
}
void Proc::ProcessorHub::finish()
{
    data->disconnet = true;
}
} // namespace Proc
