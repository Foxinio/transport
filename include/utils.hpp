// Szymon Jedras, i322920

#pragma once

#include <string>
#include <stdexcept>

namespace inet {

class not_valid_address_exception : public std::exception {
public:
    not_valid_address_exception();
    virtual const char * what() const noexcept override;
};

std::string get_addr(uint32_t addr);
std::string get_addr_with_mask(uint32_t addr, uint8_t mask);
uint32_t get_addr(const std::string& addr);
std::pair<uint32_t,uint8_t> get_addr_with_mask(const std::string& addr);

std::string get_addr_str(uint32_t addr);
std::string get_addr_with_mask_str(uint32_t addr, uint8_t mask);

}
