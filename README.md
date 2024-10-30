# 程序说明
## 程序功能

本程序用于读取配置文件并根据配置文件中的指令模拟键盘事件。支持以下几种类型的指令：

- CLICK：模拟按下或释放某个键。
- SLEEP：暂停指定的时间。
- LOCK：设置或取消锁定键（如大写锁定、滚动锁定、数字锁定）。
- COMBINATION：模拟组合键。
 
## 操作手册

1. 配置文件格式
   配置文件是一个文本文件，每行包含一个指令。指令的格式如下：
   - CLICK <键名> <状态>：模拟按下或释放某个键。
     * <键名>：键的名称，例如 A, Shift, Ctrl 等。
     * <状态>：键的状态，可以是 DOWN 或 UP。
   - SLEEP <时间>：暂停指定的时间。
     * <时间>：暂停的时间（毫秒）。
   - LOCK <锁名> <状态>：设置或取消锁定键。
     * <锁名>：锁的名称，可以是 CapsLock, ScrollLock, NumLock。
     * <状态>：锁的状态，可以是 ON 或 OFF。
   - COMBINATION <键名1> <键名2> ... <键名N>：模拟组合键。

2. 运行程序
   程序可以通过命令行运行，支持以下两种方式指定配置文件路径：
   通过命令行参数：

```shell
 program.exe path/to/config.txt
```

其中 path/to/config.txt 是配置文件的路径。
默认配置文件： 如果没有通过命令行参数指定配置文件路径，程序将默认使用当前目录下的 config.txt 文件。

3. 示例配置文件

```text
CLICK A DOWN
SLEEP 1000
CLICK A UP
LOCK CapsLock ON
SLEEP 500
LOCK CapsLock OFF
```
