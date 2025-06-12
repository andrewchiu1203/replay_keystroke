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
