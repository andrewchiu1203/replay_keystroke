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

void read_log(){
    std::ifstream file1(key_log_file_name);
    int key_code;
    std::ifstream file2(interval_log_file_name);
    int interval;

    if(!file1){
        std::cerr << "Error: failed to open log." << std::endl;
        exit(1);
    }

    while(file1 >> key_code){
        key_record.push_back(key_code);
    }

    if(!file2){
        std::cerr << "Error: failed to open log." << std::endl;
        exit(1);
    }

    while(file1 >> key_code){
        key_record.push_back(key_code);
    }

    while(file2 >> interval){
        key_interval.push_back(interval);
    }

    file1.close();
    file2.close();
}

void simulate_keys(){
    int key_code;
    int interval;

    for(int i = 0; i < key_record.size(); i++){
        key_code = key_record[i];
        interval = key_interval[i];

        INPUT input = {0};
        input.type = INPUT_KEYBOARD;
        input.ki.wVk = key_code;

        std::cout << "Key: " << std::left << std::setw(8)<< key_code;
        std::cout << "Pause (ms): " <<  interval << std::endl;

        // Key down
        input.ki.dwFlags = 0;
        SendInput(1, &input, sizeof(INPUT));

        // Short delay
        Sleep(100);

        // Key up
        input.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &input, sizeof(INPUT));

        // Actual delay
        Sleep(interval);
    }
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

    std::cout << "Replaying keystrokes...";   

    Sleep(1000);
    read_log();
    simulate_keys();
    
    std::cout << "Done" << std::endl;

    return 0;
}
