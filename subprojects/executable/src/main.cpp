#include "async.hpp"
#include "processor.hpp"

#include <chrono>
#include <iostream>
#include <thread>

int main(int argc, char* argv[])
{
    // same blocks (block =3)
    std::string data1 =
        "cmd1\ncmd2\n"
        "{\ncmd3\ncmd4\n}\n" // block1
        "{\ncmd5\ncmd6\n{\ncmd7\ncmd8\n}\ncmd9\n}"
        "\n{\ncmd10\ncmd11"
        "\n";

    // same blocks (block =3)
    std::string data2 =
        "cmd1x\ncmd2x\n"
        "{\ncmd3x\ncmd4x\n}\n" // block1
        "{\ncmd5x\ncmd6x\n{\ncmd7x\ncmd8x\n}\ncmd9x\n}"
        "\n{\ncmd10x\ncmd11x"
        "\n";
    // 2x3 block + 1
    std::string data3 = "cmd31\ncmd32\ncmd33\ncmd34\ncmd35\ncmd36\ncmd37\n";
    // create first connection
    auto* handler = async::connect(3);
    // receive call on first
    async::receive(handler, data1.c_str(), data1.size());
    std::thread thr = std::thread([&]() {
        // create second connection
        auto* handler2 = async::connect(2);
        // to make sure process last
        // ::this_thread::sleep_for(std::chrono::milliseconds(50));
        // receive on second
        async::receive(handler2, data3.c_str(), data3.size());
        async::disconnect(handler2);
    });
    // receive call on first
    async::receive(handler, data2.c_str(), data2.size());
    // join second thread
    thr.join();
    async::disconnect(handler);
    return EXIT_SUCCESS;
}
