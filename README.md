# 带键盘事件的计算器

Qt 应用程序开发 · 实验 1（2026 秋）。

## 第 1 阶段：界面与工程

界面保存在 `src/calculatorwindow.ui`，可在 Qt Creator / Qt Designer 中打开、预览和编辑。
显示区域使用只读 QLineEdit 和表达式 QLabel；整体使用 QVBoxLayout，20 个按键使用 5×4 QGridLayout。
控件属性、文字、尺寸及布局均保存于 `.ui`，由 CMake AUTOUIC 生成界面代码。

使用本机安装的 Qt 6.10.3 MinGW 64 位、CMake 3.21+ 和 C++17 编译器。

```powershell
.\scripts\build.ps1 -Test
```

也可以在 Qt Creator 中打开 `CMakeLists.txt`，选择匹配的 Qt 6 MinGW Kit 构建。
目前已完成界面、统一输入处理、四则运算和表达式显示。
连续运算按输入顺序执行，连续操作符替换为最后一次输入；缺少第二操作数时等号不触发计算。
边界情况修复和键盘功能在后续真实提交中完善。
