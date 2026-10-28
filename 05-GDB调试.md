# 05 GDB 调试：从现象到原因

## 1. 准备

先在 WSL 的 Linux shell 中构建。下面使用 Windows 目录下的原始示例；如果使用 Linux 主目录副本，将第一行改为 `cd ~/week01-example`：

```bash
cd '/mnt/d/py-code/linuxc++/learn_01/example'
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
gdb ./build/debug_demo
```

下面 `(gdb)` 表示 GDB 提示符，不是需要输入的文字。

## 2. 示例中的逻辑错误

`debug_demo` 根据设备数量和总采样数计算平均值。本题调用时总数为 120，数量为 3，但代码把分母多加了 1，因此会打印 30 而不是 40。这个错误是故意保留的，只用于调试练习。

```text
(gdb) break average_per_device
(gdb) run
(gdb) info args
(gdb) list
(gdb) next
(gdb) next
(gdb) print denominator
(gdb) backtrace
(gdb) continue
(gdb) quit
```

断点停止在函数的某行附近；`next` 执行当前行并停在下一行，因此应在初始化语句执行后查看 `denominator`。`info args` 看传入参数，`print` 看表达式，`backtrace`（或 `bt`）看当前函数及调用者。由于编译器、GDB 版本和源代码行号可能不同，以实际停止位置为准。

再尝试 `step` 进入函数，`next` 执行下一行而通常不进入函数，`continue` 运行到下一个断点或程序结束。`info breakpoints` 查看断点，`delete 1` 删除编号 1 的断点。

## 3. 修改并验证

在你自己的练习副本中，将 `debug_demo.cpp` 中 `device_count + 1` 改为 `device_count`，重新构建并运行，检查输出为 40。保存故障记录：预期、实际、复现命令、断点、观察到的参数与变量、原因、修改和复测结果。

## 4. 常用命令速查

| 命令 | 功能 |
| --- | --- |
| `break 函数名` / `break 文件名:行号` | 设置断点 |
| `run` | 启动程序；可在后面给参数 |
| `next` / `step` | 单步执行，通常分别表示不进入 / 进入被调函数 |
| `print 表达式` | 查看变量或表达式 |
| `info locals` / `info args` | 查看局部变量 / 参数 |
| `backtrace` | 查看调用栈 |
| `frame 1` | 选择调用栈中的指定帧 |
| `continue` / `quit` | 继续运行 / 退出 GDB |

若代码被优化、二进制不含调试信息、源代码与二进制不一致，变量或行号可能难以观察。先确认 Debug 配置并重新构建。

参考：[GDB 官方手册](https://sourceware.org/gdb/current/onlinedocs/gdb/)。
