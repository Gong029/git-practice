#include "config.hpp"

#include <chrono>
#include <csignal>
#include <exception>
#include <iostream>
#include <thread>

namespace {
volatile std::sig_atomic_t stop_requested = 0;

void handle_signal(int) {
    stop_requested = 1;
}
} // namespace

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <config-file>\n";
        return 2;
    }
    try {
        const Config config = load_config(argv[1]);
        if (std::signal(SIGINT, handle_signal) == SIG_ERR ||
            std::signal(SIGTERM, handle_signal) == SIG_ERR) {
            std::cerr << "Error: unable to install signal handler\n";
            return 1;
        }
        std::cout << "Starting " << config.device_name << "; press Ctrl+C to stop.\n";
        unsigned long long sequence = 0;
        while (!stop_requested) {
            std::cout << "device=" << config.device_name << " sequence=" << ++sequence
                      << " status=ok" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(config.interval_ms));
        }
        std::cout << "Stopping cleanly.\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
}
