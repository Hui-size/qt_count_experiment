# 带键盘事件的计算器

Qt 应用程序开发 · 实验 1（2026 秋）。

## 第 1 阶段：界面与工程

界面保存在 `src/calculatorwindow.ui`，可在 Qt Creator / Qt Designer 中打开、预览和编辑。
显示区域使用只读 QLineEdit 和表达式 QLabel；整体使用 QVBoxLayout，20 个按键使用 5×4 QGridLayout。
控件属性、文字、尺寸及布局均保存于 `.ui`，由 CMake AUTOUIC 生成界面代码。

要求 Qt 6.9 或更高版本、CMake 3.21+、C++17 编译器；本机使用 Qt 6.10.3 MinGW 64 位。

```powershell
.\scripts\build.ps1 -Test
```

也可以在 Qt Creator 中打开 `CMakeLists.txt`，选择匹配的 Qt 6 MinGW Kit 构建。
目前已完成界面和统一输入处理，包括数字、小数点、退格、清除、CE、正负号以及重复小数点防护。
运算和键盘功能在后续真实提交中逐步实现。
