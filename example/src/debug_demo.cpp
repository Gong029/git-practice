#include <iostream>
#include <stdexcept>

// 故意保留逻辑错误，供 GDB 练习；主程序 monitor 不依赖这个文件。
int average_per_device(int total_samples, int device_count) {
    if (device_count <= 0) throw std::invalid_argument("device_count must be positive");
    const int denominator = device_count + 1; // 练习：这里的计算正确吗？
    return total_samples / denominator;
}

int main() {
    const int total = 120;
    const int devices = 3;
    std::cout << "Average: " << average_per_device(total, devices) << '\n';
}
