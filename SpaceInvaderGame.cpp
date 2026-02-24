#include "stdafx.h"
#include "CSpaceInvaderGame.h"

#include "cvui.h"
#include <conio.h>

#define CANVAS_NAME "Space Invader game"

cv::Point Space_Close_position;
cv::Point Space_Reset_position;


int Space_FPS_SP = 34;
int ship_size = 20;
int R_Digital = 0;
int analog = 1;
int R_channel = 32;
int R_val = 1;
int Horizontal_dir_channel = 2;
int Horizontal_val;


CSpaceInvaderGame::CSpaceInvaderGame(cv::Size Canva_size, int comport) {
    control.init_com(comport);
    _Canvas = cv::Mat::zeros(Canva_size, CV_8UC3);
}
////////////////////////////////////////////
/////Run function
////////////////////////////////////////////
void CSpaceInvaderGame::run() {
    cvui::init(CANVAS_NAME);
    
    while (_running) {

        {
            auto end_time = std::chrono::steady_clock::now() + std::chrono::duration<double, std::milli>(1000 / Space_FPS_SP);

            auto calc_start = std::chrono::steady_clock::now();
            draw();
            update();
            GPIO();
            std::this_thread::sleep_until(end_time);
            auto calc_end = std::chrono::steady_clock::now();
            std::chrono::duration<double> calc_elapsed = calc_end - calc_start;

            double FPS = 1 / calc_elapsed.count();
            _FPS = std::round(FPS * 10000.0) / 10000.0;
            //std::cout << "\nElapsed Time: " << calc_elapsed.count() << " FPS " << _FPS << "\n";
        }

    }
}
////////////////////////////////////////////
/////   Draw Function
////////////////////////////////////////////
void CSpaceInvaderGame::draw() {
    // GUI Menu
    //auto end_time = std::chrono::steady_clock::now() + std::chrono::duration<double, std::milli>(1000 / FPS_SP);
    int score = 0; // example score
    std::string scoreText =  std::to_string(score);
    cv::putText(_Canvas, "Score", cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 255, 255), 2);
    cv::putText(_Canvas, scoreText, cv::Point(120, 30), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 255, 0), 2);
    cv::putText(_Canvas, "Live", cv::Point(700, 30), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 255, 255), 2);
    //cv::putText(_Canvas, "Live", cv::Point(_Canvas.size().width-10, 300), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 255, 255), 2);
    Space_Close_position = cv::Point(30, 180);
    Space_Reset_position = cv::Point(130, 180);

    if (cvui::button(_Canvas, Space_Reset_position.x, Space_Reset_position.y, 60, 40, "reset")) {
        _reset = true;
        cvui::update();
    }
    if (cvui::button(_Canvas, Space_Close_position.x, Space_Close_position.y, 60, 40, "Exit")) {
        _running = false;
        _thread_exit = true;
        cvui::update();
    }
    if (_kbhit()) {           // check if key pressed (non-blocking)
        char ch = _getch();   // read key
        if (ch == 113 || ch == 81) {       // ESC key ASCII = 27
            while (_kbhit()) _getch();
            _running = false;
            _thread_exit = true;
        }
    }
    _player_Ship = cv::Rect(_point_horizontal_mid - ship_size, _Canvas.size().height -5, ship_size*2, 5);
    cv::rectangle(_Canvas, _player_Ship, cv::Scalar(255, 255, 255), 4, 8, 0);
    _gun = cv::Rect(_point_horizontal_mid - 2, _Canvas.size().height - 7, 4, 4);
    cv::rectangle(_Canvas, _gun, cv::Scalar(255, 255, 255), 4, 8, 0);

    
    cv::imshow(CANVAS_NAME, _Canvas);
    _Canvas.setTo(cv::Scalar(0, 0, 0));
    cv::waitKey(1);
}
////////////////////////////////////////////
/////   GPIO Function
////////////////////////////////////////////
void CSpaceInvaderGame::GPIO() {
    auto end_time = std::chrono::system_clock::now() + std::chrono::milliseconds(1000 / Space_FPS_SP);
    control.get_data(R_Digital, R_channel, R_val);

    if (R_val == 0) {
        _reset = true;
    }
    /*control.get_data(D_type, D_channel, digital_val);
    if (_computer_score < 5 && _player_score < 5)
    {
        if (control.get_button(digital_val)) {
            _start_button = true;
        }
    }*/

    control.get_data(analog, Horizontal_dir_channel, Horizontal_val);
    _ship_position_percentage = static_cast<int>(control.get_analog(Horizontal_val));
    // Sleep if time remaining
    std::this_thread::sleep_until(end_time);
}
////////////////////////////////////////////
/////   Update Function
////////////////////////////////////////////
void CSpaceInvaderGame::update() 
{
    _point_horizontal_mid = (((_Canvas.size().width) * _ship_position_percentage) / 80);
    std::cout << " position " << _point_horizontal_mid << "\n";
    // _x_cursor inside canvas
    if (_point_horizontal_mid - ship_size < 0) {
        _point_horizontal_mid  = 2*ship_size;
    }
    if (_point_horizontal_mid + ship_size > _Canvas.size().width) {
        _point_horizontal_mid = _Canvas.cols - ship_size;
    }
    
}