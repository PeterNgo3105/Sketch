#include "stdafx.h"
#include <conio.h>
#include <algorithm>

#include "cvui.h"
#include "CBase4618.h"
#include "CControl.h"
#include "Pong.h"

#define CANVAS_NAME "Pong game"

enum { DIGITAL = 0, ANALOG, SERVO };
int A_type = ANALOG;
int D_type = DIGITAL;
int D_channel = 32;
int Reset_channel = 33;
int dirY_channel = 26;
int digital_val;
int dirY_val;
int line_thickness = 8;
int bar_half_length = 40;
int ball_size = 30;
int speed = 30;
int paddle_thickness = 20;
int collision;

int clamp(int value, int min, int max);

CPong::CPong(cv::Size Canva_size, int comport) {
    control.init_com(comport);
    _Canvas = cv::Mat::zeros(Canva_size, CV_8UC3);
}

void CPong::run() {
    bool running = true;
    cvui::init(CANVAS_NAME);
    cv::Point Close_position;
    cv::Point Reset_position;
    cv::Point gui_position;
    bool do_canny = true;
    bool do_aruco = false;
    _pos1 = _Canvas.size().width / 2;
    _pos2 = _Canvas.size().height / 2;
    _botY_mid_paddle_point = _Canvas.size().height / 2;

    while (running) {
        // GUI Menu
        gui_position = cv::Point(10, 10);
        cvui::window(_Canvas, gui_position.x, gui_position.y, 220, 200, "Pong {FPS}");
        gui_position = cv::Point(100, 15);
        cvui::text(_Canvas, gui_position.x, gui_position.y, "value");
        gui_position = cv::Point(15, 35);
        cvui::text(_Canvas, gui_position.x, gui_position.y, "Player:");
        gui_position = cv::Point(105, 35);
        cvui::text(_Canvas, gui_position.x, gui_position.y, "Computer:");
        gui_position = cv::Point(80, 65);
        cvui::text(_Canvas, gui_position.x, gui_position.y, "Raius");
        gui_position = cv::Point(10, 70);
        cvui::trackbar(_Canvas, gui_position.x, gui_position.y, 190, &ball_size, 5, 120);
        gui_position = cv::Point(80, 120);
        cvui::text(_Canvas, gui_position.x, gui_position.y, "Speed");
        gui_position = cv::Point(10, 125);
        cvui::trackbar(_Canvas, gui_position.x, gui_position.y, 190, &speed, 5, 1000);
        //gui_position += cv::Point(5, 25);
        //cvui::checkbox(_Canvas, gui_position.x, gui_position.y, "Canny Filter", &do_canny);
        //gui_position += cv::Point(0, 25);
        //cvui::checkbox(_Canvas, gui_position.x, gui_position.y, "ArUco", &do_aruco);
        //gui_position += cv::Point(0, 25);
        //cvui::text(_Canvas, gui_position.x, gui_position.y, "Canny Threshold");
        
        //gui_position += cv::Point(0, 50);


        Close_position = cv::Point(30, 160);
        Reset_position = cv::Point(130, 160);
        if (cvui::button(_Canvas, Reset_position.x, Reset_position.y, 60, 40, "reset")) {
            _reset = true;
            cvui::update();
        }


        if (_kbhit()) {           // check if key pressed (non-blocking)
            char ch = _getch();   // read key
            if (ch == 113 || ch == 81) {       // ESC key ASCII = 27
                while (_kbhit()) _getch();
                running = false;
            }
        }
        else if (cvui::button(_Canvas, Close_position.x, Close_position.y, 60, 40, "Exit")) {
            running = false;
            cvui::update();
        }
        else
        {
            GPIO();
            update();
            draw();
            cv::waitKey(1);
        }

    }
}
void CPong::update() {
    //calculate midpoint of the paddle
    _pointY_mid_position = (((_Canvas.size().height) * (100 - _pointY_dir_percent)) / 100);
   
    int closestX = clamp(_ball.x, _player_baddle.x, _player_baddle.x + _player_baddle.width);
    int closestY = clamp(_ball.y, _player_baddle.y, _player_baddle.y + _player_baddle.height);

    //distance between ball and baddle
    collision = (_ball.x - closestX) * (_ball.x - closestX) + (_ball.y - closestY) * (_ball.y - closestY);


    // _y_cursor inside canvas
    if (_pointY_mid_position - bar_half_length < 0) {
        _pointY_mid_position = bar_half_length;
    }
    if (_pointY_mid_position + bar_half_length > _Canvas.rows) {
        _pointY_mid_position = _Canvas.rows - bar_half_length;
    } 
    if (_start_button) {
        if ((collision <= (ball_size * ball_size))) {
            _level_up += 1;
            _pos1 -= 3;
        }
        else if (_level_up == 0 ) {
            // Set update rate at 10 Hz (100 ms per loop)
            auto end_time = std::chrono::system_clock::now() + std::chrono::milliseconds(1000 / speed);
            // Do a bunch of processing
            _pos1 += 3;
            // Sleep if time remaining
            std::this_thread::sleep_until(end_time);
        }
        else if (_level_up == 1) {
            // Set update rate at 10 Hz (100 ms per loop)
            auto end_time = std::chrono::system_clock::now() + std::chrono::milliseconds(1000 / speed);
            // Do a bunch of processing
            _pos1 -= 3;
            // Sleep if time remaining
            std::this_thread::sleep_until(end_time);
        }       
    }
}
void CPong::draw() {
    _bot_baddle = cv::Rect(0, _botY_mid_paddle_point - bar_half_length, 4, 80);
    _player_baddle = cv::Rect(_Canvas.size().width, _pointY_mid_position - bar_half_length, 4, 80);
    _ball = cv::Point(_pos1, _pos2);
    cv::circle(_Canvas, _ball, 4, cv::Scalar(200, 200, 200), ball_size, cv::LINE_AA); 
        cv::rectangle(_Canvas, _player_baddle, cv::Scalar(255, 255, 255), paddle_thickness, 8, 0);
        cv::rectangle(_Canvas, _bot_baddle, cv::Scalar(255, 255, 255), paddle_thickness, 8, 0);
        cv::imshow(CANVAS_NAME, _Canvas);
    if (_reset) {
        _Canvas.setTo(cv::Scalar(0, 0, 0));
        _reset = false;
    }
    _Canvas.setTo(cv::Scalar(0, 0, 0));
}

void CPong::GPIO() {
    control.get_data(D_type, Reset_channel, digital_val);
    if (control.get_button(digital_val)) {
        _reset = true;
    }
    control.get_data(D_type, D_channel, digital_val);
    if (control.get_button(digital_val)) {
        _start_button = true;
    }
    control.get_data(A_type, dirY_channel, dirY_val);
    _pointY_dir_percent = static_cast<int>(control.get_analog(dirY_val));
    }

CPong::CPong() {

}

///////////////////////////
/// checking if the ball collize to the paddle
///////////////////////////
int clamp(int value, int min, int max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}