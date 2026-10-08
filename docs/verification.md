# 实验 1：最终设计与验证

## 环境与结果

- Windows 11，本机 Qt 6.10.3 MinGW 64 位，GCC 13.1.0。
- 主工程使用 qmake `.pro`，已实际编译链接成功。
- Qt Test 引擎输出：43 passed、0 failed；界面输出：20 passed、0 failed。
- 两个测试类各包含初始化和清理 2 项，实际功能验证总计 59 项。
- 界面测试既在 offscreen 模式运行，也在本机 Windows 平台插件上运行；后者验证真实字体和尺寸。
- qmake 运行版使用 windeployqt 配置运行库，已启动并确认窗口响应。

## 界面与布局

`mainwindow.ui` 使用 QVBoxLayout 放置标题、表达式 QLabel、只读 QLineEdit、按键区与快捷键提示。
按键区为 5 行 4 列的 QGridLayout，缩放时控件随布局改变尺寸；输入框字体按可用宽度缩小。
样式表保存在 `.ui` 的窗口 styleSheet 属性中，区分数字、工具、操作符及等号按钮；
当前待执行操作符高亮，错误显示使用红色文字。
界面已在本机 Qt Creator 的 Designer 视图中打开检查。

## 关键代码

| 实验点 | 代码位置 | 行为 |
| --- | --- | --- |
| 信号与槽 | MainWindow 构造函数 | 所有 QPushButton 的 clicked 连接到统一处理入口 |
| 重复小数点 | CalculatorEngine::inputDecimal | 检查当前字符串是否已经包含小数点 |
| 状态管理 | CalculatorEngine::State | FirstOperand / OperatorPending / SecondOperand / Result / Error |
| 第二操作数起始 | CalculatorEngine::prepareForEntry | 清当前输入，不丢失第一个操作数及操作符 |
| 连续操作符 | CalculatorEngine::inputOperator | 等待状态替换操作符；第二操作数输入完毕后先计算 |
| 除零与溢出 | CalculatorEngine::calculate | 运算前检查除数，运算后检查有限数值 |
| 结果后继续输入 | CalculatorEngine::prepareForEntry | 数字重新开始；操作符继续当前结果 |
| 键盘输入 | MainWindow::keyPressEvent / eventFilter | 键盘映射后与鼠标共用 handleCommand |
| 显示与字体 | MainWindow::updateDisplay / fitDisplayFont | 更新表达式、结果、错误颜色和字体大小 |

## 已验证的代表性输入

| 输入 | 结果 / 现象 |
| --- | --- |
| 鼠标 `1.5 + 2 =` | 3.5；显示 `1.5 + 2 =` |
| 键盘 `12.5 + 7.25 =` | 19.75 |
| 小键盘 `2.5 * 4`、小键盘 Enter | 10 |
| `1..2 + 3.4 =` | 4.6，重复小数点忽略 |
| `8 + * 2 =` | 16，最后一个操作符生效 |
| `2 + 3 * 4 =` | 20，按标准计算器输入顺序计算 |
| `8 / 0 =` | 不能除以零，红色错误文字 |
| 除零后输入 `2 + 3 =` | 5，自动恢复新计算 |
| `2 + 3 =` 后输入 `7` | 7，开始新计算 |
| `2 + 3 = * 4 =` | 20，使用结果继续计算 |
| `12 + 34`、退格、`5 =` | 47，修改第二操作数 |
| `5 + 9`、CE、`2 =` | 7，保留第一个操作数和加号 |
| `12 + 3`、C、`4 * 2 =` | 8，清除所有状态 |
| 鼠标输入 `1`，键盘 `2.5+`，鼠标 `3`，键盘 `.4`、退格、`5`、Enter | 16，表达式 `12.5 + 3.5 =` |
| 结果后的退格 | 保留结果，不能误删 |
| 15 位数字且窗口缩至最小 | 数字完整显示，无文字截断 |
| 连续大数乘法导致数值溢出 | 结果超出范围；C 后恢复 0 |

实际修复前后对照见 `development.md`，包含 6 项真实失败现象及修复方法。

## 本机验证证据

按存储要求，截图与测试日志保存在 `F:\Codex\huancun`，不作为生成缓存上传 GitHub。

- `F:\Codex\huancun\qt-count-evidence\preview.png`：最终原生 Windows 界面，鼠标计算 1.5 + 2 = 3.5。
- `F:\Codex\huancun\qt-count-evidence\divide-by-zero.png`：除零及错误颜色。
- `F:\Codex\huancun\qt-count-evidence\long-number.png`：最小窗口的长数字显示。
- `F:\Codex\huancun\qt-count-task\stage4-before.txt`：修复前 6 项失败。
- `F:\Codex\huancun\qt-count-task\stage4-after.txt`：修复后的引擎测试。
- `F:\Codex\huancun\qt-count-task\final-engine.txt`：最终引擎测试。
- `F:\Codex\huancun\qt-count-task\final-native-ui.txt`：最终原生 Windows 界面及交互测试。

可设置 `CALCULATOR_PREVIEW_PATH` 和 `CALCULATOR_QA_DIR`，运行界面测试重新生成截图；
截图需使用 Windows 平台插件，offscreen 模式在本机不能正确加载字体。

## Git 提交

仓库：<https://github.com/Hui-size/qt_count_experiment>。5 次提交分别实现：

1. 工程与 Designer 格式界面。
2. 数字、小数点、退格和清除的统一输入处理。
3. 四则运算、连续运算和表达式状态。
4. 实际边界失败修复及 `.pro` 工程入口。
5. 键盘共用逻辑、样式优化、交互测试及说明完善。

使用 `git log --oneline --reverse` 查看实际提交历史。
