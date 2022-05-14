//
// Created by foxinio on 5/14/22.
//

#include <sender.hpp>
#include <init.hpp>

int main(int argc, char* argv[]) {
    auto i = init(argc, argv);
    auto worker = sender{i.sockfd,
                         i.addr,
                         std::move(i.out_file),
                         i.no_bits};
    return worker.run();
}
