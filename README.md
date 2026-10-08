# 带键盘事件的计算器

Qt 应用程序开发 · 实验 1（2026 秋）。使用你电脑上安装的 **Qt 6.10.3 MinGW 64-bit**。

## 在 Qt Creator 中打开、编译、运行

1. 双击根目录的 **qt_count_experiment.pro**，或在 Qt Creator 中选择“文件 → 打开文件或项目”。
2. 首次打开时选择 **Desktop Qt 6.10.3 MinGW 64-bit** Kit。
3. 将构建目录设置为 `F:\Codex\huancun\qt-count-creator`，点击“配置项目”。
4. 按 **Ctrl+B** 编译，再按 **Ctrl+R** 或点击左下角绿色三角形运行。
5. 双击 `mainwindow.ui` 进入 Qt Creator 的设计界面，查看和编辑控件属性、网格布局及样式表。

源码、头文件和 `.ui` 都在工程根目录；不需要另建工程或手工复制代码。
`.pro` 是日常使用的入口；CMake 文件用于自动化测试。

## 功能与约定

- 四则运算、小数、正负号、退格、C（全部清除）和 CE（清当前操作数）。
- 显示正在输入的操作数、操作符、完整双操作数表达式及结果。
- 每个操作数只能有一个小数点；连续操作符替换为最后一个。
- 连续运算按输入顺序计算，例如 `2 + 3 × 4 = 20`。
- 结果后输入数字或小数点开启新计算，输入操作符继续使用结果。
- 除零显示“不能除以零”；数字、小数点、C 或 CE 可恢复，错误状态下操作符和退格忽略。
- 退格仅修改正在输入的操作数；等待第二操作数和显示结果时退格忽略。
- 未输入第二操作数时等号忽略；重复等号保留结果。
- `double` 计算，显示保留最多 15 位有效数字；每个操作数最多输入 15 位数字。
- 超出有限数值范围显示“结果超出范围”。这是一款标准计算器，不提供表达式优先级或任意精度计算。

| 键盘 | 对应功能 |
| --- | --- |
| 主键盘 / 小键盘 0–9 | 数字 |
| `.` / `,` | 小数点 |
| `+`、`-`、`*`、`/` | 加、减、乘、除 |
| Enter / 小键盘 Enter / `=` | 等号 |
| Backspace | 退格 |
| Esc / C | 全部清除 |
| Delete | 清除当前操作数（CE） |

鼠标 clicked 信号、窗口键盘事件和子控件事件过滤器都调用 `handleCommand → CalculatorEngine::dispatch`，
输入混用时保持同一份状态。Ctrl/Alt/Meta 组合键不参与计算输入。

## 主要文件

| 文件 | 用途 |
| --- | --- |
| `qt_count_experiment.pro` | Qt Creator / qmake 工程入口 |
| `main.cpp` | QApplication 和窗口启动 |
| `mainwindow.h/.cpp` | 信号槽、键盘事件、显示更新及字体适配 |
| `mainwindow.ui` | Designer 界面、控件属性、布局和样式表 |
| `calculatorengine.h/.cpp` | 五种状态及统一计算逻辑 |
| `tests/` | Qt Test 输入序列与鼠标、键盘交互回归 |
| `docs/development.md` | 迭代过程、实际失败现象及修复 |
| `docs/verification.md` | 最终测试结果、关键代码定位及截图位置 |
| `docs/ai-assistance.md` | 一个实际 AI 辅助开发过程 |

## 命令行构建与测试（可选）

日常直接使用 Qt Creator 即可；以下脚本方便重复验证，工具均来自本机 Qt 安装目录。

```powershell
.\scripts\build.ps1 -Test
.\scripts\build.ps1 -Test -Deploy
```

脚本默认使用 `F:\QT\qt\6.10.3\mingw_64` 和 `F:\QT\qt\Tools\mingw1310_64`。
临时目录、qmake 构建目录和 CMake 测试构建目录均位于 `F:\Codex\huancun`。
`-Deploy` 在工程的 `dist` 目录生成可直接启动的 `qt_count_experiment.exe` 及其 Qt / MinGW 运行库。
`dist`、生成文件和个人 Qt Creator 配置均不提交到 GitHub。

实际功能测试共 **59 项全部通过**（Qt Test 输出为引擎 43、界面 20，包含各自初始化与清理共 4 项）。
本机 Windows 原生界面测试也通过，已检查中文字体、除零显示和最小窗口中的 15 位数字。

实验视频由你自行录制，本项目不代录视频。
