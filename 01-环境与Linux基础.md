# 01 环境与 Linux 基础

## 1. 目标

准备可编译 C++17 的 Linux 环境，理解源码到可执行文件的基本过程，并查看程序的进程和资源。本周不依赖开发板，也不要求交叉编译。

你有两个环境：本地 WSL Ubuntu-20.04 和课题组 Ubuntu 22.04 服务器。**本周优先用本地 WSL**，方便访问当前资料、独立练习和安装工具；服务器留作学习远程登录及核对 Linux 行为的补充环境。二者与未来 ARM64 开发板并不完全相同，后续仍需要上板实践。

## 2. WSL：确认版本和用户

在 **Windows PowerShell** 执行：

```powershell
wsl -l -v
wsl -d Ubuntu-20.04
```

第一条显示发行版和 WSL 版本；你的截图只能确认 Ubuntu-20.04 能启动，不能确定是 WSL 1 还是 WSL 2。第二条进入 Linux shell，以下说明除特别标记外都在 **WSL 的 Linux shell** 执行。

你的提示符 `root@localhost:...#` 表示当前以 root 运行，不是出错。日常编辑和练习推荐普通用户：它能降低误改系统或把练习文件创建为 root 所有的概率。先在 WSL 执行：

```bash
whoami
id
getent passwd 1000
ls /home
```

`getent passwd 1000` 只用于查找 UID 为 1000 的常见账户；查不到并不代表不存在其他普通账户。如果你已知道自己的普通用户名，退出当前 WSL shell（执行 `exit`），在 **PowerShell** 用以下命令进入，将 `你的Linux用户名` 替换为真实用户名：

```powershell
wsl -d Ubuntu-20.04 -u g
```

进入后用 `whoami` 和 `pwd` 确认；如果当前目录仍是 `/mnt/c/.../Desktop`，可以用 `cd ~` 进入该用户的 Linux 主目录。

**如果确实没有可用的普通账户**，可以在本地 WSL 的 root shell 创建一个专门练习账户（示例名 `learner`；先运行 `getent passwd learner` 检查是否已存在，存在则换名或使用已有账户）：

```bash
adduser learner
usermod -aG sudo learner
```

`adduser` 会交互式设置 Linux 密码；额外资料可按提示留空。`usermod` 把该用户加入管理员组，以便日后通过 `sudo` 安装工具。创建后退出 WSL，在 PowerShell 执行 `wsl -d Ubuntu-20.04 -u learner`，然后用 `whoami` 确认。已有账户也要按实际情况确认是否有 sudo 权限；不必为练习修改 WSL 默认登录账户。上面操作只针对**你的本地 WSL**，不适用于共享服务器。

Ubuntu 20.04 标准支持已于 2025 年 5 月结束；建议之后考虑新安装受支持的 Ubuntu LTS，或按自身情况选择扩展安全维护。不要直接卸载、重置或升级现有发行版：先确认已有数据和备份方案。本周可以先在现有 WSL 中学习。[Ubuntu 20.04 支持状态](https://ubuntu.com/20-04)

## 3. 检查和安装工具

先在 **WSL 普通用户的 Linux shell** 检查已有工具：

```bash
g++ --version
cmake --version
git --version
gdb --version
uname -a
```

若工具缺失，且你决定在**本地 WSL** 安装，可执行：

```bash
sudo apt update
sudo apt install build-essential cmake git gdb procps iproute2
sudo apt install build-essential cmake git gdb procps iproute2
```

`sudo` 通常请求当前 Linux 普通用户的密码；root shell 本身不需要加 `sudo`。`apt update` 刷新包列表，`apt install` 会安装所列软件和所需依赖，不会自动执行全系统升级。`build-essential` 提供常用编译工具；`procps` 提供 `ps`、`top`；`iproute2` 提供 `ss`。`lsof` 是可选工具，可另用 `sudo apt install lsof` 安装。

共享服务器上先检查现有工具，缺少工具时联系管理员；不要因为服务器登录提示有更新，就自行运行 `sudo apt update`、安装或升级。

## 4. 访问当前资料和准备练习副本

当前资料在 Windows 路径 `D:\py-code\linuxc++\learn_01`。在 WSL 的 Linux shell 先验证挂载：

```bash
ls /mnt/d
cd '/mnt/d/py-code/linuxc++/learn_01'
pwd
ls
```

标准自动挂载下，Windows 的 `D:` 一般对应 `/mnt/d`；如果目录不存在，先检查 WSL 挂载设置和 Windows 路径，不要盲目创建同名目录。可以直接在上述目录学习和构建，Windows 与 WSL 看到的是同一份文件。

也可以把示例复制到普通用户的 Linux 主目录，避免频繁跨文件系统构建；[微软文档](https://learn.microsoft.com/en-us/windows/wsl/filesystems)推荐 Linux 工具使用 Linux 文件系统以获得更好的性能。以下命令在 `learn_01` 目录执行；若目标已存在，它会停止并保留现有内容：

```bash
if [ ! -e "$HOME/week01-example" ]; then
    cp -a example "$HOME/week01-example"
else
    printf '%s\n' '目标已存在，请检查后手动选择新目录。'
fi
cd "$HOME/week01-example"
pwd
```

两处是**独立副本**：以后在其中一处修改不会自动同步到另一处。以下讲义的 `/path/to/learn_01/example` 可用原目录 `/mnt/d/py-code/linuxc++/learn_01/example` 替换；如果使用副本，则改为 `~/week01-example`。继续之前确认当前用户不是 root，以免在 `/root` 下创建练习文件。

## 5. 课题组服务器如何使用

服务器已显示成功登录，用户为 `gongziming`，当前目录是该用户主目录。它适合练习 SSH、查看进程和在个人目录运行小程序；登录提示中的更新数量可能过时，系统更新由管理员处理。不要在共享目录、大规模占用资源或更改系统服务；如需复制代码到服务器，请用课题组允许的传输方式，并先确认服务器地址和自己的目录。

`-bash: /home/dell/NetWork/start.sh: Permission denied` 表示登录 shell 尝试执行这个路径时被拒绝；**不能仅凭提示确定原因**，也不能断定是文件自身权限问题。既然已出现命令提示符，可先继续学习。若课题组需要排查，可将完整提示反馈给管理员；不要自行修改 `/home/dell` 中的脚本或权限。

## 6. 常用命令与路径

```bash
pwd                    # 当前目录
ls -la                 # 文件及隐藏文件
cd '/mnt/d/py-code/linuxc++/learn_01'  # 挂载可用时进入资料目录
mkdir -p ~/cpp-practice
cp example/config.txt ~/cpp-practice/
cat ~/cpp-practice/config.txt
```

`/` 开头是绝对路径；相对路径以当前目录为起点。`~` 表示当前 Linux 用户的主目录。命令参数区分大小写；遇到 `No such file or directory` 先检查当前目录和路径。

## 7. 从源码到可执行文件

典型流程：预处理（展开头文件和宏）→ 编译（生成汇编等中间结果）→ 汇编（生成目标文件）→ 链接（组合目标文件和库）。多文件工程中，头文件通常提供声明，`.cpp` 提供定义；链接错误可能来自缺少定义或没有加入对应源文件。

```bash
mkdir -p ~/cpp-practice
cd ~/cpp-practice
cat > hello.cpp <<'EOF'
#include <iostream>
int main() {
    std::cout << "hello Linux C++\n";
}
EOF
g++ -std=c++17 -Wall -Wextra -g hello.cpp -o hello
./hello
```

`-std=c++17` 选语言版本，`-Wall -Wextra` 开启常用警告，`-g` 生成调试信息，`-o` 指定输出文件。编译错误先看**第一条**并结合行号定位；警告也需要理解。CMake 会在后续讲义中代管多文件编译。

## 8. 进程与观察

先按 [示例工程说明](example/README.md) 构建示例。在一个终端启动：

```bash
cd '/mnt/d/py-code/linuxc++/learn_01/example'
./build/monitor config.txt
```

在第二个终端观察，再回到第一个终端按 Ctrl+C：

```bash
ps -ef | grep '[m]onitor'
top
ss -lnt
```

`ps` 给出当前进程快照，通常可看到 PID；`top` 动态显示进程和资源，按 `q` 退出；`ss -lnt` 列出监听中的 TCP socket。第一周示例**没有网络服务**，`ss` 不显示它是正常的。方括号写法用于避免 `grep` 命中自己的命令行；具体进程显示依系统而异。

如果你是在 `~/week01-example` 副本中构建的，请将上述 `cd` 换成 `cd ~/week01-example`。

## 9. 动手练习与自检

1. 写出自己的 Linux 发行版、编译器、CMake、GDB 版本。
2. 修改 `hello.cpp`，重新编译并运行；故意写错一个分号，读懂编译器报错后修复。
3. 找到示例程序的 PID，观察其退出前后的 `ps` 输出。
4. 回答：头文件、目标文件、可执行文件分别做什么？编译错误和链接错误有什么区别？

**提示**：编译错误通常是某个源文件不符合语言或声明规则；链接错误发生在组合目标文件和库时，例如调用了只声明、未提供定义的函数。

## 10. 延伸阅读

- [微软 WSL 安装说明](https://learn.microsoft.com/zh-cn/windows/wsl/install)
- [微软：在 WSL 中使用文件系统](https://learn.microsoft.com/en-us/windows/wsl/filesystems)
- [微软：WSL 命令参考](https://learn.microsoft.com/en-us/windows/wsl/basic-commands)
- [Ubuntu 20.04 支持状态](https://ubuntu.com/20-04)
- [GCC 编译选项手册](https://gcc.gnu.org/onlinedocs/gcc/Invoking-GCC.html)
- 本机可查看 `man ps`、`man top`、`man ss`；按 `q` 退出。
