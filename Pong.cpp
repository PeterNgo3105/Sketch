#include "stdafx.h"
#include <conio.h>
#include <algorithm>
#include <sstream>

#include "cvui.h"
#include "CBase4618.h"
#include "CControl.h"
#include "Pong.h"

#define CANVAS_NAME "Pong game"

enum { DIGITAL = 0, ANALOG, SERVO };
int A_type = ANALOG;
int D_type = DIGITAL;
int D_channel = 33;
int Reset_channel = 32;
int dirY_channel = 26;
int digital_val = 1;
int reset_val = 1;
int dirY_val;
int line_thickness = 8;
int bar_half_length = 40;
int FPS_SP = 34;
int paddle_thickness = 20;
int collision, collision2;
int angle_level = 0;
int pos1_temp;
int pos2_temp;
bool ons_inital = true;
int collision_state = 0;
float speed;

cv::Point Close_position;
cv::Point Reset_position;
cv::Point gui_position;



int clamp(int value, int min, int max);

CPong::CPong(cv::Size Canva_size, int comport) {
    control.init_com(comport);
    _Canvas = cv::Mat::zeros(Canva_size, CV_8UC3);
}

void CPong::run() {
    cvui::init(CANVAS_NAME);
    _pos1 = _Canvas.size().width / 2;
    _pos2 = _Canvas.size().height / 2;
    _botY_mid_paddle_point = _Canvas.size().height / 2;
    
    std::thread thread2(&CPong::GPIO_thread,this);
    std::thread thread3(&CPong::update_thread,this);
    thread2.detach();
    thread3.detach();
    while (_running) {
        
        {
            auto end_time = std::chrono::steady_clock::now() + std::chrono::duration<double, std::milli>(1000/FPS_SP);
           
            auto calc_start = std::chrono::steady_clock::now();
            draw();  
           
            std::this_thread::sleep_until(end_time);
            auto calc_end = std::chrono::steady_clock::now();
            std::chrono::duration<double> calc_elapsed = calc_end - calc_start;
          
            double FPS = 1 / calc_elapsed.count();
            _FPS = std::round(FPS * 10000.0) / 10000.0;
            std::cout << "\nElapsed Time: " << calc_elapsed.count() << " FPS " << _FPS << "\n";
        }  

    }
}
void CPong::update() {
    auto end_time = std::chrono::system_clock::now() + std::chrono::milliseconds(1000 / FPS_SP);
    auto calc_start = std::chrono::steady_clock::now();
    // Code under test
    speed = _speed / FPS_SP;
    _pointY_mid_position = (((_Canvas.size().height) * (100 - _pointY_dir_percent)) / 100);
   
    int closestX = clamp(_ball.x, _player_baddle.x, _player_baddle.x + _player_baddle.width);
    int closestY = clamp(_ball.y, _player_baddle.y, _player_baddle.y + _player_baddle.height);
    int bot_closestX = clamp(_ball.x-_ball_size, _bot_baddle.x, _bot_baddle.x + _bot_baddle.width);
    int bot_closestY = clamp(_ball.y, _bot_baddle.y, _bot_baddle.y + _player_baddle.height);
    int player_paddle_length = bar_half_length * 2;
    int wall_check = clamp(_ball.y, 0, _Canvas.size().height);
    int wall_top_collision = wall_check * wall_check; 
    int wall_bottom_collision = (_Canvas.rows - wall_check) * (_Canvas.rows - wall_check);
    int computer_win_point = 1;
    int player_win_point = 1;

    if (ons_inital) {
        pos1_temp = _Canvas.size().width / 2;
        pos2_temp = _Canvas.size().height / 2;
    }
    int pointX_speed = sqrt((speed)* (speed));
    int pointY_speed = sqrt((speed) * (speed));
    //distance between ball and baddle
    collision = (_ball.x - closestX) * (_ball.x - closestX) +(_ball.y - closestY) * (_ball.y - closestY);
    collision2 = (_ball.x - bot_closestX) * (_ball.x - bot_closestX) +(_ball.y - bot_closestY) * (_ball.y - bot_closestY);
    // _y_cursor inside canvas
    if (_pointY_mid_position - bar_half_length < 0) {
        _pointY_mid_position = bar_half_length;
    }
    if (_pointY_mid_position + bar_half_length > _Canvas.rows) {
        _pointY_mid_position = _Canvas.rows - bar_half_length;
    }    
    if (_start_button) 
    {        
        if (collision_state == 1)
        {
            _pos1 -= pointX_speed;
            _pos2 += pointY_speed;
            _botY_mid_paddle_point += pointY_speed;
        }
        else if (collision_state == 2)
        {
            _pos1 -= pointX_speed;
            _pos2 -= pointY_speed;
            _botY_mid_paddle_point -= pointY_speed;
        }
        else if (collision_state == 3)
        {
            _pos1 += pointX_speed;
            _pos2 += pointY_speed;
            _botY_mid_paddle_point += pointY_speed;
        }
        else if (collision_state == 4)
        {
            _pos1 += pointX_speed;
            _pos2 -= pointY_speed;
            _botY_mid_paddle_point -= pointY_speed;
        }
        else if (collision_state == 5)
        {
            _pos1 -= pointX_speed;
            _pos2 += pointY_speed;
            _botY_mid_paddle_point += pointY_speed;
        }
        else if (collision_state == 6)
        {
            _pos1 += pointX_speed;
            _pos2 += pointY_speed;
            _botY_mid_paddle_point += pointY_speed;
        }
        else if (collision_state == 7)
        {
            _pos1 -= pointX_speed;
            _pos2 -= pointY_speed;
            _botY_mid_paddle_point -= pointY_speed;
        }
        else if (collision_state == 8)
        {
            _pos1 += pointX_speed;
            _pos2 -= pointY_speed;
            _botY_mid_paddle_point -= pointY_speed;
        }
        else if (collision_state == 0)
        {
            _pos1 += pointX_speed;
            _pos2 += pointY_speed;
            _botY_mid_paddle_point += pointY_speed;
        }
        if ((collision < (_ball_size * _ball_size)))
        {
            if (_pos1 > pos1_temp && _pos2 > pos2_temp)
            {
                _pos1 -= pointX_speed;
                _pos2 += pointY_speed;
                _botY_mid_paddle_point += pointY_speed;
                collision_state = 1;
            }
            else if (_pos1 > pos1_temp && _pos2 < pos2_temp)
            {
                _pos1 -= pointX_speed;
                _pos2 -= pointY_speed;
                _botY_mid_paddle_point -= pointY_speed;
                collision_state = 2;
            }  
        }
        else if (collision2 < (_ball_size * _ball_size))
        {
            if (_pos1 < pos1_temp && _pos2 > pos2_temp)
            {
                _pos1 += pointX_speed;
                _pos2 += pointY_speed;
                _botY_mid_paddle_point += pointY_speed;
                collision_state = 3;
            }
            else if (_pos1 < pos1_temp && _pos2 < pos2_temp)
            {
                _pos1 += pointX_speed;
                _pos2 -= pointY_speed;
                _botY_mid_paddle_point -= pointY_speed;
                collision_state = 4;
            }
        }
        else if (wall_top_collision < (_ball_size * _ball_size))
        {
            if (_pos1 < pos1_temp && _pos2 < pos2_temp)
            {
                _pos1 -= pointY_speed;
                _pos2 += pointY_speed;
                _botY_mid_paddle_point += pointY_speed;
                collision_state = 5;
            }
            else if (_pos1 > pos1_temp && _pos2 < pos2_temp)
            {
                _pos1 += pointX_speed;
                _pos2 += pointY_speed;
                _botY_mid_paddle_point += pointY_speed;
                collision_state = 6;
            }
        }
        else if (wall_bottom_collision < (_ball_size * _ball_size))
        {          
            if (_pos1 < pos1_temp && _pos2 > pos2_temp)
            {
                _pos1 -= pointX_speed;
                _pos2 -= pointY_speed;
                _botY_mid_paddle_point -= pointY_speed;
                collision_state = 7;
            }
            else if (_pos1 > pos1_temp && _pos2 > pos2_temp)
            {
                if (collision_state == 0)
                {
                    _pos1 = _pos1 + (1*pointX_speed);
                    _pos2 =_pos2- (1* pointY_speed);
                    _botY_mid_paddle_point -= pointY_speed;
                    collision_state = 8;
                }
                else
                {
                    _pos1 += pointX_speed;
                    _pos2 -= pointY_speed;
                    _botY_mid_paddle_point -= pointY_speed;
                    collision_state = 8;
                }
            }   
        }  
        if (_ball.x > _Canvas.cols + 3)
        {
            _computer_score++;
            _pos1 = _Canvas.size().width / 2;
            _pos2 = _Canvas.size().height / 2;
            _botY_mid_paddle_point = _Canvas.size().height / 2;
            _start_button = false;
        }
        else if (_ball.x < -3)
        {   
            _player_score++;
            _pos1 = _Canvas.size().width / 2;
            _pos2 = _Canvas.size().height / 2;
            _botY_mid_paddle_point = _Canvas.size().height / 2;
            _start_button = false;
        }   
        ons_inital = false;
        pos1_temp = _pos1;
        pos2_temp = _pos2;
        auto calc_end = std::chrono::steady_clock::now();
        float calc_elapsed = std::chrono::duration<float>(calc_end - calc_start).count();
        

    }
    // Sleep if time remaining
    std::this_thread::sleep_until(end_time);
}
void CPong::draw() {
    // GUI Menu
    //auto end_time = std::chrono::steady_clock::now() + std::chrono::duration<double, std::milli>(1000 / FPS_SP);
    
    gui_position = cv::Point(10, 10);
    cvui::window(_Canvas, gui_position.x, gui_position.y, 220, 170, "Pong {FPS                  }");
    gui_position = cv::Point(100, 15);
    cvui::text(_Canvas, gui_position.x, gui_position.y, std::to_string(_FPS));
    gui_position = cv::Point(15, 35);
    cvui::text(_Canvas, gui_position.x, gui_position.y, "Player:");
    gui_position = cv::Point(65, 35);
    cvui::text(_Canvas, gui_position.x, gui_position.y, std::to_string(_player_score));
    gui_position = cv::Point(105, 35);
    cvui::text(_Canvas, gui_position.x, gui_position.y, "Computer:");
    gui_position = cv::Point(200, 35);
    cvui::text(_Canvas, gui_position.x, gui_position.y, std::to_string(_computer_score));
    gui_position = cv::Point(80, 65);
    cvui::text(_Canvas, gui_position.x, gui_position.y, "Radius");
    gui_position = cv::Point(10, 70);
    cvui::trackbar(_Canvas, gui_position.x, gui_position.y, 190, &_ball_size, 5, 100);
    gui_position = cv::Point(80, 120);
    cvui::text(_Canvas, gui_position.x, gui_position.y, "Speed");
    gui_position = cv::Point(10, 125);
    cvui::trackbar(_Canvas, gui_position.x, gui_position.y, 190, &_speed, 100, 400);
    Close_position = cv::Point(30, 180);
    Reset_position = cv::Point(130, 180);

    if (cvui::button(_Canvas, Reset_position.x, Reset_position.y, 60, 40, "reset")) {
        _reset = true;
        cvui::update();
    }
    if (cvui::button(_Canvas, Close_position.x, Close_position.y, 60, 40, "Exit")) {
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

    _bot_baddle = cv::Rect(0, _botY_mid_paddle_point - bar_half_length, 4, 80);
    _player_baddle = cv::Rect(_Canvas.size().width, _pointY_mid_position - bar_half_length, 4, 80);
    _ball = cv::Point(_pos1, _pos2);
    cv::Point point1 = cv::Point(_Canvas.size().width / 2, 0);
    cv::Point point2 = cv::Point(_Canvas.size().width / 2, _Canvas.size().height);
    cv::line(_Canvas, point1, point2, cv::Scalar(0, 0, 255),2, cv::LINE_AA);
    cv::circle(_Canvas, _ball, 4, cv::Scalar(200, 200, 200), _ball_size, cv::LINE_AA); 
    cv::rectangle(_Canvas, _player_baddle, cv::Scalar(255, 255, 255), paddle_thickness, 8, 0);
    cv::rectangle(_Canvas, _bot_baddle, cv::Scalar(255, 255, 255), paddle_thickness, 8, 0);
    if (_computer_score >= 5 || _player_score >= 5)
    {

        gui_position = cv::Point(400, 250);
        cvui::window(_Canvas, gui_position.x, gui_position.y, 220, 170, "Game Over");
        gui_position = cv::Point(450, 300);
        cvui::text(_Canvas, gui_position.x, gui_position.y, "Game Over");

    }
    cv::imshow(CANVAS_NAME, _Canvas);

    if (_reset) {
        _Canvas.setTo(cv::Scalar(0, 0, 0));
        _pos1 = _Canvas.size().width / 2;
        _pos2 = _Canvas.size().height / 2;
        _botY_mid_paddle_point = _Canvas.size().height / 2;
        //_level_up = 0;
        _computer_score = 0;
        _player_score = 0;
        _ball_size = 30;
        _speed = 400;
        _start_button = false;
        _reset = false;
    }
    _Canvas.setTo(cv::Scalar(0, 0, 0));
    cvui::update();
    cv::waitKey(1);
}

void CPong::GPIO() {
    auto end_time = std::chrono::system_clock::now() + std::chrono::milliseconds(1000 / FPS_SP);
    control.get_data(D_type, Reset_channel, reset_val);
    
    if(reset_val==0){
        _reset = true;
    }
    control.get_data(D_type, D_channel, digital_val);
    if (_computer_score < 5 && _player_score < 5)
    {
        if (control.get_button(digital_val)) {
            _start_button = true;
        }
    }
  
    control.get_data(A_type, dirY_channel, dirY_val);
    _pointY_dir_percent = static_cast<int>(control.get_analog(dirY_val));
    // Sleep if time remaining
    std::this_thread::sleep_until(end_time);
    }

CPong::CPong() {

}
void CPong::update_thread(CPong* ptr) {
    while (ptr->_thread_exit == false)
    {

        ptr->update();

    }
    std::cout << "update exit\n";
}
void CPong::draw_thread(CPong* ptr) {
    while (ptr->_thread_exit == false)
    {
        ptr->draw();
    }
    std::cout << "draw exit\n";
}
void CPong::GPIO_thread(CPong* ptr) {
    while (ptr->_thread_exit == false)
    {
        ptr->GPIO();
    }
    std::cout << "GPIO exit\n";
}



///////////////////////////
/// checking if the ball collize to the paddle
///////////////////////////
int clamp(int value, int min, int max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}
