
#include <iostream>
#include <fstream>
#include <stdexcept>

void write_log() {
    std::ofstream log("sample.log");

    if (!log) {
        throw std::runtime_error("open log failed");
    }

    log << "started\n";
}

int main() {
    try {
        std::cout << "1. Before write_log\n";

        write_log();

        std::cout << "2. After write_log\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
