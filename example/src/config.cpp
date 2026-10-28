#include "config.hpp"

#include <cctype>
#include <fstream>
#include <stdexcept>
#include <string>

namespace {
std::string trim(const std::string& text) {
    const auto first = text.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    const auto last = text.find_last_not_of(" \t\r\n");
    return text.substr(first, last - first + 1);
}

int parse_interval(const std::string& value) {
    if (value.empty()) throw std::runtime_error("interval_ms must be an integer");
    for (const unsigned char ch : value) {
        if (!std::isdigit(ch)) throw std::runtime_error("interval_ms must be an integer");
    }
    try {
        const int result = std::stoi(value);
        if (result < 200 || result > 5000) {
            throw std::runtime_error("interval_ms must be between 200 and 5000");
        }
        return result;
    } catch (const std::out_of_range&) {
        throw std::runtime_error("interval_ms is too large");
    }
}
} // namespace

Config load_config(const std::string& path) {
    std::ifstream input(path);
    if (!input) throw std::runtime_error("cannot open config: " + path);

    Config config{};
    bool has_name = false;
    bool has_interval = false;
    std::string line;
    unsigned line_number = 0;
    while (std::getline(input, line)) {
        ++line_number;
        const std::string text = trim(line);
        if (text.empty() || text.front() == '#') continue;

        const auto separator = text.find('=');
        if (separator == std::string::npos) {
            throw std::runtime_error("line " + std::to_string(line_number) + ": expected key=value");
        }
        const auto key = trim(text.substr(0, separator));
        const auto value = trim(text.substr(separator + 1));
        if (key == "device_name") {
            if (has_name) throw std::runtime_error("duplicate device_name");
            if (value.empty()) throw std::runtime_error("device_name must not be empty");
            config.device_name = value;
            has_name = true;
        } else if (key == "interval_ms") {
            if (has_interval) throw std::runtime_error("duplicate interval_ms");
            config.interval_ms = parse_interval(value);
            has_interval = true;
        } else {
            throw std::runtime_error("line " + std::to_string(line_number) + ": unknown key '" + key + "'");
        }
    }
    if (input.bad()) throw std::runtime_error("error reading config: " + path);
    if (!has_name || !has_interval) throw std::runtime_error("missing device_name or interval_ms");
    return config;
}
