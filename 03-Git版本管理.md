# 03 Git 版本管理

## 1. 基本概念

Git 将项目的历史记录为一系列提交（commit）。日常主要涉及三个区域：工作区是当前文件；暂存区是准备放入下一次提交的内容；仓库保存已经提交的快照。

```text
修改文件 → git add → 暂存区 → git commit → 提交历史
```

`git status` 看状态；`git diff` 看工作区与暂存区的差异；`git diff --cached` 看暂存区与当前提交的差异。Git 不会自动保存所有修改，提交前要检查暂存内容。

## 2. 在独立目录练习

**不要在现有工程或含已有 `.git` 的目录里随意执行 `git init`**。下面的操作在 WSL 普通用户的 Linux shell 下新建一个独立练习目录；如果 `mkdir` 提示目录已存在，请停止并检查，或选择一个新的目录名，再继续后面的命令：

```bash
mkdir ~/week01-git-practice
cd ~/week01-git-practice
git init
git config user.name '你的名字'
git config user.email '你的邮箱'
printf '# Git practice\n' > README.md
git status
git add README.md
git diff --cached
git commit -m 'Add practice README'
```

这里的 `git config` 不带 `--global`，只配置当前练习仓库。邮箱写自己的实际或允许公开的地址；不要提交密钥和个人敏感信息。

## 3. 完成第二次提交

```bash
printf '\nFirst change.\n' >> README.md
git status
git diff
git add README.md
git diff --cached
git commit -m 'Document first change'
git log --oneline --decorate -5
```

观察每一步文件所处的状态。注意：`git add` 记录的是执行时的文件内容；之后再编辑，要重新 `git add` 才会包含新改动。

## 4. 忽略构建产物

项目通常不提交构建目录。在练习仓库创建 `.gitignore`：

```gitignore
build/
*.o
```

忽略规则主要作用于未跟踪文件；已经被跟踪的文件不会因为新写规则自动取消跟踪。示例工程已提供自己的 `.gitignore`。

## 5. 安全地撤销暂存

误把文件加入暂存区时，可先用 `git status` 查看提示。对已有提交的仓库，可以执行 `git restore --staged README.md`，这会取消该文件暂存并保留工作区改动。恢复工作区内容会覆盖改动，先理解命令及确认目标再操作。

## 6. 练习与自检

1. 在独立练习仓库完成两次提交，并用 `git log --oneline` 检查。
2. 修改文件、暂存、再次修改，分别观察 `git diff` 和 `git diff --cached`。
3. 解释 `git add` 和 `git commit` 分别做了什么；说出为什么不应该提交 `build/`。

**答案提示**：`git add` 把当前内容放入暂存区，`git commit` 根据暂存内容创建提交。构建产物通常可由源码重新生成，且可能与系统和构建配置相关。

参考：[Pro Git：记录每次更新](https://git-scm.com/book/zh/v2/Git-基础-记录每次更新到仓库)。
