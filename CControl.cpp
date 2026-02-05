
#include "cvui.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <regex>
#include <algorithm>
#include "stdafx.h"
#include "serial.h"
#include "server.h"
#include "CControl.h"


void CControl::init_com(int comport) {
	std::string port = "COM" + std::to_string(comport);
	_com.open(port);
}
bool CControl::get_data(int type, int channel, int& result) {
    
    if (_com.is_open()) {
        // TX and RX strings
        std::string tx_str = ("G " + std::to_string(type) + " " + std::to_string(channel) + "\n");
        std::string rx_str;
        std::string value;
        // temporary storage
        char buff[2];
        // Send TX string
        _com.write(tx_str.c_str(), tx_str.length());
        Sleep(10); // servo need this timer
        rx_str = "";
        // start timeout count
        double start_time = cv::getTickCount();
        buff[0] = 0;
        
        while (buff[0] != '\n')// && cv::getTickCount() - start_time < 1000)
        {
            if (_com.read(buff, 1) > 0)
            {
                rx_str = rx_str + buff[0];
            }
        }
        if (buff[0] != '\n')// && cv::getTickCount() - start_time >= 1000)
        {
            return false;
        }
        size_t pos = rx_str.find_last_of(' ');

        std::string lastWord;
            lastWord = rx_str.substr(pos + 1);
        result = std::stoi(lastWord);
        return true;
    }
    else {
        return false;
    }            
}
bool CControl::set_data(int type, int channel, int val) {
	
    if (_com.is_open()) {
        // Sending command to comport
        std::string tx_str = ("S " + std::to_string(type) + " " + std::to_string(channel) + " " + std::to_string(val) + "\n");
        std::string rx_str;      
        char buff[2];
        // commanding micro controller
        _com.write(tx_str.c_str(), tx_str.length());
        Sleep(10); // 100ms is needed for servo
        rx_str = "";
        // start timeout count
        double start_time = cv::getTickCount();
        
        buff[0] = 0;     
        while (buff[0] != '\n' && (GetTickCount() - start_time) < 1)
        {
            if (_com.read(buff, 1) > 0)
            {
                rx_str = rx_str + buff[0];
            }
        }
        if (buff[0] != '\n' && (GetTickCount() - start_time) >= 1)
        {
            return false;
        }
        return true;
    }
    else
        return false;  
}
float CControl::get_analog(float value) {
    float val = (value / 4096) * 100;//4096
    return val;
}
bool CControl::get_button(bool button) {
    using clock = std::chrono::steady_clock;
    static bool last_button = true;
    static auto last_time = clock::now() - std::chrono::seconds(1);
    auto now = clock::now();
    if (button && !last_button) {
        if (now - last_time >= std::chrono::seconds(1)) {
            last_time = now;
            last_button = button;
            return true;
        }
    }
    last_button = button;
    return false;
}
CControl::CControl() {

}
CControl:: ~CControl() {

}

