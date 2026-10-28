#pragma once

#include <string>

struct Config {
    std::string device_name;
    int interval_ms;
};

// 配置文件缺失或格式错误时抛出 std::runtime_error。
Config load_config(const std::string& path);
