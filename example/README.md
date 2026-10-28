# 第一周示例工程

这是一个 C++17 多文件项目。`monitor` 读取配置并定时打印模拟设备状态；`debug_demo` 是独立的 GDB 逻辑错误练习。

## 构建和运行

在 WSL 的 Linux shell 中进入本目录。下列路径是 Windows 目录下的示例；如果使用按[环境讲义](../01-环境与Linux基础.md)创建的 Linux 主目录副本，改为 `cd ~/week01-example`。复制到其他位置后，本 README 中指向上级讲义的相对链接可能不再有效，可回到原资料目录查看。

```bash
cd '/mnt/d/py-code/linuxc++/learn_01/example'
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/monitor config.txt
```

预期每秒输出一行设备名、递增序号和 `status=ok`，例如：

```text
Starting orange-pi-demo; press Ctrl+C to stop.
device=orange-pi-demo sequence=1 status=ok
device=orange-pi-demo sequence=2 status=ok
```

按 Ctrl+C 后程序设置退出标志并在睡眠返回后结束，打印 `Stopping cleanly.`。等待时长受睡眠和调度影响。直接运行不带参数会显示用法并以状态码 2 退出。

## 修改配置与检查错误

`config.txt` 中的 `device_name` 和 `interval_ms` 各写一次；周期为 200～5000 毫秒的十进制整数。空行和以 `#` 开头的行被忽略，未知或重复键会报错。

```bash
./build/monitor does-not-exist.txt
echo $?  # 应为非零
```

如需试验非法配置，请复制 `config.txt` 到自己的练习文件，修改后传入该文件，避免丢失原始样例。文件相对路径以当前工作目录为准。运行时读取配置，修改后需要重新启动程序，但不需要重新编译。

## 调试练习

`./build/debug_demo` 故意输出 30，正确结果应为 40。跟随 [GDB 讲义](../05-GDB调试.md) 调试并在自己的副本中修复。

## 文件说明

- `include/config.hpp`：配置结构和函数声明。
- `src/config.cpp`：配置文件读取、校验和错误处理。
- `src/main.cpp`：参数处理、信号处理、输出循环。
- `src/debug_demo.cpp`：独立的调试示例。
- `CMakeLists.txt`：定义两个可执行目标。
