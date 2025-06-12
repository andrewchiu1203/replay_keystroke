# replay_keystroke
Automate software install on windows by recording keystrokes and replay it.

<p>
    #include <iostream>
    #include <fstream>
    #include <vector>
    #include <windows.h>
    
    std::string log_file_name = "key_record.txt";
    std::vector<int> key_record;
    
    // Connects code to windows keyboard event
    HHOOK keyboardHook;
    
    void write_log(std::vector<int>& data){
        std::ofstream file(log_file_name);
    
        if(file.is_open()){
            for(int key_code : data){
                file << key_code << "\n";
            }
        }else{
            std::cerr << "Error: failed to write key log." << std::endl;
            exit(1);
        }
    
        file.close();
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
    
            // Stop when 'q' is pressed
            if (key_str == "Q"){
                UnhookWindowsHookEx(keyboardHook);
                write_log(key_record);
                exit(0);
            }
    
            key_record.push_back(key_code);
            std::cout << key_code << ":\t" << key_str << std::endl;
        }
    
        // Must call next hook to ensure keyboard functions after the capture
        return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
    }
    
    int main(){
        // Installs low-level keyboard hook that listens for keyboard inputs globally
        keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
    
        if (!keyboardHook){
            std::cerr << "Error: keyboard not found." << std::endl;
            exit(1);
        }
    
        std::cout << "All following keypresses will be recorded." << std::endl;
        std::cout << "To stop recording, type 'q' here directly." << std::endl;
    
        MSG msg;
    
        // Retrieve system events, process it normally, which calls KeyboardProc as defined
        while (GetMessage(&msg, NULL, 0, 0)){
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    
        return 0;
    }
</p>

<p>
    #include <iostream>
    #include <fstream>
    #include <vector>
    #include <windows.h>
    
    std::string log_file_name = "key_record.txt";
    
    std::vector<int> read_log(){
        std::ifstream file(log_file_name);
        std::vector<int> key_record;
        int key_code;
    
        if(!file){
            std::cerr << "Error: failed to open key log." << std::endl;
            exit(1);
        }
    
        while(file >> key_code){
            key_record.push_back(key_code);
        }
    
        return key_record;
    }
    
    void simulate_keys(const std::vector<int>& key_record) {
        for (int key_code : key_record) {
            INPUT input = {0};
            input.type = INPUT_KEYBOARD;
            input.ki.wVk = key_code;
    
            // Key down
            input.ki.dwFlags = 0;
            SendInput(1, &input, sizeof(INPUT));
    
            // Short delay
            Sleep(250);
    
            // Key up
            input.ki.dwFlags = KEYEVENTF_KEYUP;
            SendInput(1, &input, sizeof(INPUT));
    
            // Short delay
            Sleep(250);
        }
    }
    
    
    int main(){
        std::vector<int> key_record;
        key_record = read_log();
    
        for(int key_code : key_record){
            std::cout << key_code << std::endl;
        }
    
        std::cout << "Simulating keystrokes..." << std::endl;
        Sleep(2000);
    
        simulate_keys(key_record);
    
        std::cout << "Done" << std::endl;
    
        return 0;
    }
</p>
