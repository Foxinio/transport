//
// Created by foxinio on 5/14/22.
//

#include <sender.hpp>
#include <init.hpp>

#include <logger/fwd.hpp>

int main(int argc, char* argv[]) {
    log::logger::traits.set_level(log::level::debug);
    auto i = init(argc, argv);
    auto worker = sender{i.sockfd,
                         i.addr,
                         std::move(i.out_file),
                         i.no_bits};
    return worker.run();
}
