#include <iostream>
#include <string>
#include <serial_bridge_index.hpp>

int main() {
    std::string decoded = serial_bridge::decode_address(
        "{\"address\":\"REMOVED\",\"nettype_string\":\"MAINNET\"}"
    );

    std::cout << decoded << std::endl;
    return 0;
}
