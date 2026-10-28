# 04 多文件项目与 CMake

## 1. 为什么需要构建工具

小程序可以手动输入 `g++ main.cpp config.cpp ...`；源文件增多、需要调试配置和依赖时，构建系统能记录构建规则并只重建必要内容。CMake 生成构建系统，由 `cmake --build` 调用对应的构建工具。

示例结构：

```text
example/
├── CMakeLists.txt
├── config.txt
├── include/config.hpp
└── src/
    ├── config.cpp
    ├── main.cpp
    └── debug_demo.cpp
```

`config.hpp` 声明配置结构和函数，`config.cpp` 提供实现，`main.cpp` 负责程序入口。`debug_demo.cpp` 是单独的调试程序，不能与另一个 `main` 合并为同一个目标。

## 2. 阅读示例配置

```cmake
cmake_minimum_required(VERSION 3.16)
project(week01_monitor LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_executable(monitor src/main.cpp src/config.cpp)
target_include_directories(monitor PRIVATE include)
add_executable(debug_demo src/debug_demo.cpp)
```

`project` 声明项目语言；`add_executable` 指定目标及源文件；`target_include_directories` 设置该目标的头文件搜索路径。`PRIVATE` 表示该设置用于此目标本身。`CMAKE_CXX_STANDARD_REQUIRED` 表示要求指定语言标准，而不是静默降级。

## 3. 配置、构建与运行

在 WSL 的 Linux shell 中运行。下面使用 Windows 目录下的原始示例；如果已按环境文档复制到 Linux 主目录，则将第一行改为 `cd ~/week01-example`：

```bash
cd '/mnt/d/py-code/linuxc++/learn_01/example'
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/monitor config.txt
```

`-S .` 指源码目录，`-B build` 指独立的构建目录。常见 Linux 单配置生成器通过 `CMAKE_BUILD_TYPE=Debug` 启用调试构建；多配置生成器的配置方式不同。输出路径以实际生成器为准，上面示例适用于常见 Linux Makefile/Ninja 构建。

不要把 `build/` 当源码；删除自己生成的构建目录后可以重新配置和构建，但删除前先确认路径和其中没有需要保留的文件。配置中用到的相对路径以**运行程序时当前目录**为准，未必是可执行文件所在目录。

## 4. 常见错误

- `cmake: command not found`：先安装 CMake 或检查 PATH。
- `config.hpp: No such file`：检查目标头文件路径和 include 写法。
- `undefined reference`：检查声明是否有对应定义、源文件是否加入正确目标。
- 找不到 `config.txt`：查看 `pwd`，确认启动目录或传入配置文件绝对路径。
- GDB 看不到源代码：确认使用带调试信息的 Debug 构建，且调试的是重新构建后的程序。

## 5. 练习

1. 增加一个新函数，分别写在头文件和源文件中，在主程序调用并构建。
2. 有意暂时从 `add_executable` 移除 `src/config.cpp`，观察链接错误，再恢复并重建。
3. 修改配置文件周期并运行，说明是程序运行时读取配置，还是 CMake 编译时读取。

**提示**：示例在启动时读取配置；修改配置后重新运行程序即可，不必重新编译。

参考：[CMake 官方教程](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)。
