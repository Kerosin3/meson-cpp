#include "async.hpp"
#include "processor.hpp"

#include <chrono>
#include <iostream>
#include <thread>

int main(int argc, char* argv[])
{
    // async::func1();
    // async::func2();
    // auto handler = async::connect(3);
    // auto handler2 = async::connect(3);
    // std::string data =
    // "cmd1\ncmd2\n{\ncmd3\ncmd4\n}\n{\ncmd5\ncmd6\n{\ncmd7\ncmd8\n}\ncmd9\n}"
    // "\n{\ncmd10\ncmd11\n";
    // async::receive(handler, data.c_str(), data.size());
    // async::receiveCin(handler);

    //-----------------
    std::string data =
        "cmd1\ncmd2\n"
        "{\ncmd3\ncmd4\n}\n" // block1
        "{\ncmd5\ncmd6\n{\ncmd7\ncmd8\n}\ncmd9\n}"
        "\n{\ncmd10\ncmd11"
        "\n";

    std::string datax =
        "cmd1x\ncmd2x\n"
        "{\ncmd3x\ncmd4x\n}\n" // block1
        "{\ncmd5x\ncmd6x\n{\ncmd7x\ncmd8x\n}\ncmd9x\n}"
        "\n{\ncmd10x\ncmd11x"
        "\n";
    std::string data2 =
        "cmd21\ncmd22\n"
        "{\ncmd23\ncmd24\n}\n" // block1
        "{\ncmd25\ncmd26\n{\ncmd27\ncmd28\n}\ncmd29\n}"
        "\n{\ncmd30\ncmd31"
        "\n";
    std::string data3 = "cmd21\ncmd22\n";
    constexpr size_t BLOCKSIZE = 3;
    auto* handler = async::connect(3);
    auto* handler2 = async::connect(4);
    async::receive(handler, data.c_str(), data.size());
    async::receive(handler, data2.c_str(), data2.size());
    async::receive(handler2, datax.c_str(), datax.size());
    async::disconnect(handler);
    async::disconnect(handler2);
    return EXIT_SUCCESS;
}
