#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <list>

#include <filesystem>

// 定义命令信息结构体
struct CommandInfo {
    enum class CommandType { Click, Sleep, Lock, Combination };
    CommandType type;
    std::string keyName; // 按键名称
    bool keyState; // 按键状态 (UP/DOWN)
    int duration; // 睡眠时间或间隔时间
    std::string lockName; // 锁的名称
    bool lockState; // 锁的状态
    std::vector<std::string> combinationKeys; // 组合键列表

    // 统一的构造函数
    CommandInfo(CommandType t, const std::string& k = "", bool s = false, int d = 0, const std::string& ln = "", bool ls = false, const std::vector<std::string>& ck = {})
        : type(t), keyName(k), keyState(s), duration(d), lockName(ln), lockState(ls), combinationKeys(ck) {}

    // 判断是否为有效的命令
    bool isValid() const {
        switch (type) {
            case CommandType::Click:
                return !keyName.empty();
            case CommandType::Sleep:
                return duration > 0;
            case CommandType::Lock:
                return !lockName.empty();
            case CommandType::Combination:
                return !combinationKeys.empty();
            default:
                return false;
        }
    }

    // 重载 << 运算符以支持输出枚举类型
    friend std::ostream& operator<<(std::ostream& os, const CommandType& ct) {
        switch (ct) {
            case CommandType::Click:
                return os << "Click";
            case CommandType::Sleep:
                return os << "Sleep";
            case CommandType::Lock:
                return os << "Lock";
            case CommandType::Combination:
                return os << "Combination";
            default:
                return os << "Unknown";
        }
    }
};

// 去除字符串两端的空格
std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

// 获取虚拟键码
int getVirtualKeyCode(const std::string& keyStr) {
    if (keyStr == "LWin") return VK_LWIN;
    if (keyStr == "RWin") return VK_RWIN;
    if (keyStr == "LAlt") return VK_LMENU;
    if (keyStr == "RAlt") return VK_RMENU;
    if (keyStr == "LShift") return VK_LSHIFT;
    if (keyStr == "RShift") return VK_RSHIFT;
    if (keyStr == "LCtrl") return VK_LCONTROL;
    if (keyStr == "RCtrl") return VK_RCONTROL;
    if (keyStr == "Space") return VK_SPACE;
    if (keyStr == "Enter") return VK_RETURN;
    if (keyStr == "Backspace") return VK_BACK;
    if (keyStr == "Tab") return VK_TAB;
    if (keyStr == "Esc") return VK_ESCAPE;
    if (keyStr == "F1") return VK_F1;
    if (keyStr == "F2") return VK_F2;
    if (keyStr == "F3") return VK_F3;
    if (keyStr == "F4") return VK_F4;
    if (keyStr == "F5") return VK_F5;
    if (keyStr == "F6") return VK_F6;
    if (keyStr == "F7") return VK_F7;
    if (keyStr == "F8") return VK_F8;
    if (keyStr == "F9") return VK_F9;
    if (keyStr == "F10") return VK_F10;
    if (keyStr == "F11") return VK_F11;
    if (keyStr == "F12") return VK_F12;
    if (keyStr == "NumLock") return VK_NUMLOCK;
    if (keyStr == "ScrollLock") return VK_SCROLL;
    if (keyStr == "CapsLock") return VK_CAPITAL;
    if (keyStr == "Insert") return VK_INSERT;
    if (keyStr == "Delete") return VK_DELETE;
    if (keyStr == "Home") return VK_HOME;
    if (keyStr == "End") return VK_END;
    if (keyStr == "PageUp") return VK_PRIOR;
    if (keyStr == "PageDown") return VK_NEXT;
    if (keyStr == "Left") return VK_LEFT;
    if (keyStr == "Right") return VK_RIGHT;
    if (keyStr == "Up") return VK_UP;
    if (keyStr == "Down") return VK_DOWN;
    if (keyStr == "PrintScreen") return VK_SNAPSHOT;
    if (keyStr == "Pause") return VK_PAUSE;
    if (keyStr == "Menu") return VK_APPS;
    if (keyStr == "NumPad0") return VK_NUMPAD0;
    if (keyStr == "NumPad1") return VK_NUMPAD1;
    if (keyStr == "NumPad2") return VK_NUMPAD2;
    if (keyStr == "NumPad3") return VK_NUMPAD3;
    if (keyStr == "NumPad4") return VK_NUMPAD4;
    if (keyStr == "NumPad5") return VK_NUMPAD5;
    if (keyStr == "NumPad6") return VK_NUMPAD6;
    if (keyStr == "NumPad7") return VK_NUMPAD7;
    if (keyStr == "NumPad8") return VK_NUMPAD8;
    if (keyStr == "NumPad9") return VK_NUMPAD9;
    if (keyStr == "Multiply") return VK_MULTIPLY;
    if (keyStr == "Add") return VK_ADD;
    if (keyStr == "Separator") return VK_SEPARATOR;
    if (keyStr == "Subtract") return VK_SUBTRACT;
    if (keyStr == "Decimal") return VK_DECIMAL;
    if (keyStr == "Divide") return VK_DIVIDE;
    if (keyStr == "OemMinus") return VK_OEM_MINUS; // -
    if (keyStr == "OemPlus") return VK_OEM_PLUS; // +
    if (keyStr == "OemComma") return VK_OEM_COMMA; // ,
    if (keyStr == "OemPeriod") return VK_OEM_PERIOD; // .
    if (keyStr == "OemQuestion") return VK_OEM_2; // ?
    if (keyStr == "OemTilde") return VK_OEM_3; // `
    if (keyStr == "OemOpenBrackets") return VK_OEM_4; // [
    if (keyStr == "OemCloseBrackets") return VK_OEM_6; // ]
    if (keyStr == "OemPipe") return VK_OEM_5; // \

    if (keyStr == "OemSemicolon") return VK_OEM_1; // ;
    if (keyStr == "OemQuotes") return VK_OEM_7; // '
    if (keyStr == "OemBackslash") return VK_OEM_102; // \

    if (keyStr.length() == 1) return keyStr[0];
    return 0;
}

// 发送键盘事件
void sendKeyPress(int key, bool keyState) {
    INPUT input = {0};
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = key;

    if (keyState) {
        input.ki.dwFlags = 0; // 按下键
    } else {
        input.ki.dwFlags = KEYEVENTF_KEYUP; // 释放键
    }

    SendInput(1, &input, sizeof(INPUT));
}

// 设置锁状态
void setLockState(const std::string& lockName, bool state) {
    int lockKey = 0;
    if (lockName == "CapsLock") {
        lockKey = VK_CAPITAL;
    } else if (lockName == "ScrollLock") {
        lockKey = VK_SCROLL;
    } else if (lockName == "NumLock") {
        lockKey = VK_NUMLOCK;
    }

    if (lockKey != 0) {
        if (GetKeyState(lockKey) & 1) {
            if (!state) {
                keybd_event(lockKey, 0, 0, 0);  // 按下锁键
                keybd_event(lockKey, 0, KEYEVENTF_KEYUP, 0);  // 释放锁键
            }
        } else {
            if (state) {
                keybd_event(lockKey, 0, 0, 0);  // 按下锁键
                keybd_event(lockKey, 0, KEYEVENTF_KEYUP, 0);  // 释放锁键
            }
        }
    }
}

// 读取配置文件
std::list<CommandInfo> readConfigFile(const std::string& filename) {
    std::list<CommandInfo> commands;
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string eventType;
            iss >> eventType;

            if (eventType == "CLICK") {
                std::string keyName;
                std::string keyStateStr;
                iss >> keyName >> keyStateStr;
                bool keyState = (keyStateStr == "DOWN");
                commands.emplace_back(CommandInfo::CommandType::Click, keyName, keyState);
            } else if (eventType == "SLEEP") {
                int duration;
                iss >> duration;
                commands.emplace_back(CommandInfo::CommandType::Sleep, "", false, duration);
            } else if (eventType == "LOCK") {
                std::string lockName;
                std::string lockStateStr;
                iss >> lockName >> lockStateStr;
                bool lockState = (lockStateStr == "ON");
                commands.emplace_back(CommandInfo::CommandType::Lock, "", false, 0, lockName, lockState);
            } else if (eventType == "COMBINATION") {
                std::vector<std::string> combinationKeys;
                std::string key;
                while (iss >> key) {
                    combinationKeys.push_back(key);
                }
                commands.emplace_back(CommandInfo::CommandType::Combination, "", false, 0, "", false, combinationKeys);
            } else {
                std::cerr << "Unknown event type: " << eventType << " in line: " << line << std::endl;
            }
        }
        file.close();
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
    return commands;
}

int main(int argc, char* argv[]) {
    std::string configFilePath;

    if (argc > 1) {
        configFilePath = argv[1];
    } else {
        configFilePath = std::filesystem::current_path().string() + "\\config.txt";
    }
    // 读取配置文件
    std::list<CommandInfo> commands = readConfigFile(configFilePath);

    for (const auto& command : commands) {
        if (!command.isValid()) {
            std::cerr << "Invalid command: " << command.type << std::endl;
            continue;
        }

        switch (command.type) {
            case CommandInfo::CommandType::Click: {
                int key = getVirtualKeyCode(command.keyName);
                if (key != 0) {
                    sendKeyPress(key, command.keyState);
                    std::cout << "Pressed: " << command.keyName << " " << (command.keyState ? "DOWN" : "UP") << std::endl;
                } else {
                    std::cerr << "Unknown key: " << command.keyName << std::endl;
                }
                break;
            }
            case CommandInfo::CommandType::Sleep: {
                std::cout << "Sleeping for " << command.duration << " milliseconds" << std::endl;
                Sleep(command.duration);
                break;
            }
            case CommandInfo::CommandType::Lock: {
                setLockState(command.lockName, command.lockState);
                std::cout << command.lockName << " is now " << (command.lockState ? "ON" : "OFF") << std::endl;
                break;
            }
            case CommandInfo::CommandType::Combination: {
                for (const auto& key : command.combinationKeys) {
                    int keyCode = getVirtualKeyCode(key);
                    if (keyCode != 0) {
                        sendKeyPress(keyCode, true);
                        std::cout << "Pressed: " << key << " DOWN" << std::endl;
                    } else {
                        std::cerr << "Unknown key: " << key << std::endl;
                    }
                }
                for (const auto& key : command.combinationKeys) {
                    int keyCode = getVirtualKeyCode(key);
                    if (keyCode != 0) {
                        sendKeyPress(keyCode, false);
                        std::cout << "Pressed: " << key << " UP" << std::endl;
                    } else {
                        std::cerr << "Unknown key: " << key << std::endl;
                    }
                }
                break;
            }
        }
    }

    return 0;
}
