#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <cstdlib>
#include <windows.h>

std::string key_log_file_name;
std::string interval_log_file_name;
std::vector<int> key_record;
std::vector<int> key_interval;
bool first_interval = true;
auto start = std::chrono::high_resolution_clock::now();
std::chrono::duration<double> interval = std::chrono::high_resolution_clock::now() - start;

// Connects code to windows keyboard event
HHOOK keyboardHook;

void write_log(std::vector<int>& key_record, std::vector<int>& key_interval){
    std::ofstream file1(key_log_file_name);
    std::ofstream file2(interval_log_file_name);

    if(!file1.is_open()){
        std::cerr << "Error: failed to write log." << std::endl;
        exit(1);
    }

    if(!file2.is_open()){
        std::cerr << "Error: failed to write log." << std::endl;
        exit(1);
    }

    for(int key_code : key_record){
            file1 << key_code << "\n";
    }

    for(int interval : key_interval){
        file2 << interval << "\n";
    }

    file1.close();
    file2.close();
}

std::string key_handle(int key_code){
    std::string key_str;
    
    switch (key_code){
        case VK_BACK: key_str = "[Backspace]"; break;
        case VK_TAB: key_str = "[Tab]"; break;
        case VK_RETURN: key_str = "[Enter]"; break;
        case VK_SHIFT: key_str = "[Shift]"; break;
        case VK_CONTROL: key_str = "[Ctrl]"; break;
        case VK_MENU: key_str = "[Alt]"; break;
        case VK_ESCAPE: key_str = "[Esc]"; break;
        case VK_SPACE: key_str = "[Space]"; break;
        case VK_LEFT: key_str = "[Left Arrow]"; break;
        case VK_RIGHT: key_str = "[Right Arrow]"; break;
        case VK_UP: key_str = "[Up Arrow]"; break;
        case VK_DOWN: key_str = "[Down Arrow]"; break;
        case VK_DELETE: key_str = "[Delete]"; break;
        case 91: key_str = "[Windows]"; break;
        case 164: key_str = "[Alt]"; break;
        default: key_str = static_cast<char>(key_code);
    }

    return key_str;
}

// Callback function runs whenever keyboard event occurs
// nCode: if it can be processed    wParam: stroke type    lParam: keypress data
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam){
    if (nCode >= 0 && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)){
        // Extracts key data
        KBDLLHOOKSTRUCT *raw = (KBDLLHOOKSTRUCT*)lParam;

        // Convert raw key binary to char
        int key_code = raw->vkCode;
        std::string key_str = key_handle(key_code);
        interval = std::chrono::high_resolution_clock::now() - start;
        start = std::chrono::high_resolution_clock::now();

        // interval is int in ms
        if(first_interval){
            first_interval = false;
        }else{
            key_interval.push_back(interval.count()*1000);
        }

        // Stop when 'q' is pressed
        if (key_str == "Q"){
            UnhookWindowsHookEx(keyboardHook);
            write_log(key_record, key_interval);
            std::cout << "Done" << std::endl;
            exit(0);
        }

        key_record.push_back(key_code);
        std::cout << "Key: " << std::setw(8)<< std::left << key_code;
        std::cout << "Str: " << key_str << std::endl;
    }

    // Must call next hook to ensure keyboard functions after the capture
    return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

int main(int argc, char** argv){
    // parse args
    if(argc != 2){
        std::cerr << "Error: Please provide a name for the log file." << std::endl;
        exit(1);
    }

    key_log_file_name = argv[1];
    interval_log_file_name = argv[1];
    key_log_file_name = "log/" + key_log_file_name + ".txt";
    interval_log_file_name = "log/" + interval_log_file_name + "_interval.txt";

    // Installs low-level keyboard hook that listens for keyboard inputs globally
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);

    if (!keyboardHook){
        std::cerr << "Error: keyboard not found." << std::endl;
        exit(1);
    }

    std::cout << "All following keypresses will be recorded." << std::endl;
    std::cout << "To stop recording, type 'q' here directly." << std::endl;

    start = std::chrono::high_resolution_clock::now();

    // Retrieve system events, process it normally, which calls KeyboardProc as defined
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
