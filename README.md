# 带键盘事件的计算器

Qt 应用程序开发 · 实验 1（2026 秋）。

## 在你电脑上的 Qt Creator 中编译运行

双击根目录的 `qt_count_experiment.pro`，用 Qt Creator 打开。
选择 **Desktop Qt 6.10.3 MinGW 64-bit** Kit，构建目录填写 `F:\Codex\huancun\qt-count-creator`，
然后点击“配置项目”。按 `Ctrl+B` 编译，点击左下角绿色运行按钮或按 `Ctrl+R` 运行。

界面保存在根目录的 `mainwindow.ui`，可在 Qt Creator / Qt Designer 中打开、预览和编辑。
显示区域使用只读 QLineEdit 和表达式 QLabel；整体使用 QVBoxLayout，20 个按键使用 5×4 QGridLayout。
控件属性、文字、尺寸及布局均保存于 `.ui`，由 CMake AUTOUIC 生成界面代码。

使用本机安装的 Qt 6.10.3 MinGW 64 位、CMake 3.21+ 和 C++17 编译器。

```powershell
.\scripts\build.ps1 -Test
```

`.pro` 是交互使用的主要入口；`CMakeLists.txt` 同时用于自动化回归测试。
目前已完成界面、统一输入处理、四则运算和表达式显示。
连续运算按输入顺序执行，连续操作符替换为最后一次输入；缺少第二操作数时等号不触发计算。
已完成除零、结果后继续输入、退格状态保护、CE 和错误恢复等边界修复。
键盘功能在第 5 阶段完善。实际问题与修复过程见 `docs/development.md`。
