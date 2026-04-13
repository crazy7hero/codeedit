# codeedit
Qt官方代码编辑器示例的深度解读 | 行号显示 | 当前行高亮 | 完整注释
# Qt 代码编辑器 - 官方示例深度解读


## 📖 项目简介

这是一个基于 **Qt 官方 Code Editor Example** 的深度解读项目。通过详细的中文注释和原理分析，帮助你理解：

- ✅ 如何在 QPlainTextEdit 中添加行号显示
- ✅ 如何实现行号与文本的同步滚动
- ✅ 如何高亮显示当前光标所在行
- ✅ Qt 信号槽在复杂交互中的应用
- ✅ 自定义控件的性能优化技巧

## 🎯 项目来源

本项目代码来自 **Qt 官方示例**，笔者负责：
- 添加详细的中文注释
- 解释核心代码的原理
- 分析官方设计思路
- 补充使用示例

> 📌 官方文档：[Qt Code Editor Example](https://doc.qt.io/qt-6/qtwidgets-widgets-codeeditor-example.html)

## ✨ 功能特性

| 功能 | 说明 |
|------|------|
| 行号显示 | 左侧自动显示行号，随滚动同步 |
| 当前行高亮 | 光标所在行显示浅黄色背景 |
| 文件操作 | 支持打开/保存文本文件 |
| 状态栏 | 显示当前光标位置（行:列）|
| 性能优化 | 只绘制可见区域，支持大文件 |

## 📸 效果截图
展示效果，notepad++效果差不多
<img width="1557" height="637" alt="Snipaste_2026-04-13_12-17-47" src="https://github.com/user-attachments/assets/c24cd23b-318b-4c8b-8217-fe6a444ba8fe" />


## 🚀 快速开始

### 环境要求

- Qt 5.15 或更高版本
- CMake 3.16+ 或 qmake

### 编译运行
Qt_5_15_2_MinGW_64_bit

# 使用 qmake 构建
qmake
make

# 或使用 CMake 构建
mkdir build && cd build
cmake ..
make

# 运行
./CodeEditor

