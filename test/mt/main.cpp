//
// Created by foxinio on 5/14/22.
//

#include <sender.hpp>
#include <init.hpp>

#include <logger/fwd.hpp>

int main() {
//    log::logger::traits.set_level(log::level::debug);
    const char* args[] = {
            "./transport",
            "127.0.0.1",
            "40001",
            "../../../output",
            "1000024",
            nullptr
    };
    auto i = init(5, (char**)args);
    auto worker = sender{i.sockfd,
                         i.addr,
                         std::move(i.out_file),
                         i.no_bits};
    return worker.run();
}
