//
// Created by foxinio on 5/14/22.
//

#include <sender.hpp>
#include <init.hpp>

#include <logger/fwd.hpp>

#include <iostream>

int main(int argc, char* argv[]) {
    try {
        auto i = init(argc, argv);
        auto worker = sender{i.sockfd,
                             i.addr,
                             std::move(i.out_file),
                             i.no_bits};
        return worker.run();
    }
    catch(std::exception& e) {
        std::cerr << "exception caught: " << e.what() << "\n";
    }
}
