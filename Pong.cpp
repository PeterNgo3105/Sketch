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
int collision, collision2;
int angle_level = 0;
int pos1_temp;
int pos2_temp;
bool ons_inital = true;
int collision_state = 0;



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
        cvui::window(_Canvas, gui_position.x, gui_position.y, 220, 220, "Pong {FPS}");
        gui_position = cv::Point(100, 15);
        cvui::text(_Canvas, gui_position.x, gui_position.y, "value");
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
        cvui::trackbar(_Canvas, gui_position.x, gui_position.y, 190, &ball_size, 5, 120);
        gui_position = cv::Point(80, 120);
        cvui::text(_Canvas, gui_position.x, gui_position.y, "Speed");
        gui_position = cv::Point(10, 125);
        cvui::trackbar(_Canvas, gui_position.x, gui_position.y, 190, &speed, 5, 120);
        //cvui::update();
        Close_position = cv::Point(30, 180);
        Reset_position = cv::Point(130, 180);
        if (cvui::button(_Canvas, Reset_position.x, Reset_position.y, 60, 40, "reset")) {
            _reset = true;
            //cvui::update();
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
            //cvui::update();
        }
        else
        {

            auto calc_start = std::chrono::steady_clock::now();
            GPIO();
            update();
            draw();
            cv::waitKey(1);
            
            auto calc_end = std::chrono::steady_clock::now();
            auto calc_elapsed = (std::chrono::duration_cast<std::chrono::milliseconds>(calc_end - calc_start));
            std::cout << "\nElapsed Time: " << calc_elapsed.count();
        }
        cvui::update();
    }
}
void CPong::update() {
    auto calc_start = std::chrono::steady_clock::now();
    // Code under test
    
    _pointY_mid_position = (((_Canvas.size().height) * (100 - _pointY_dir_percent)) / 100);
   
    int closestX = clamp(_ball.x, _player_baddle.x, _player_baddle.x + _player_baddle.width);
    int closestY = clamp(_ball.y, _player_baddle.y, _player_baddle.y + _player_baddle.height);
    int bot_closestX = clamp(_ball.x-ball_size, _bot_baddle.x, _bot_baddle.x + _bot_baddle.width);
    int bot_closestY = clamp(_ball.y, _bot_baddle.y, _bot_baddle.y + _player_baddle.height);
    int player_paddle_length = bar_half_length * 2;
    int wall_check = clamp(_ball.y, 0, _Canvas.size().height);
    int wall_top_collision = wall_check * wall_check; 
    int wall_bottom_collision = (_Canvas.rows - wall_check) * (_Canvas.rows - wall_check);
    int val = 1;
    int val1 = 1;
    if (_ball.x > _Canvas.cols) 
    {
        val = 0;
        
    }   
    else if (_ball.x < 0)
    {
        val1 = 0;
   
    }
    if (control.get_button(val))
    {
        _computer_score++;
        _reset = true;
    }
    if (control.get_button(val1))
    {
        _player_score++;
        _reset = true;
    }


        

    if (ons_inital) {
        pos1_temp = _Canvas.size().width / 2;
        pos2_temp = _Canvas.size().height / 2;
    }
    int pointX_speed = sqrt((speed/2)* (speed/ 2));
    int pointY_speed = sqrt((speed / 2) * (speed / 2));

   // Collision(angle_level, collision, collision2);
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

        if ((collision < (ball_size * ball_size)))
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
        else if (collision2 < (ball_size * ball_size))
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
        else if (wall_top_collision < (ball_size * ball_size))
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
        else if (wall_bottom_collision < (ball_size * ball_size))
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
        ons_inital = false;
        pos1_temp = _pos1;
        pos2_temp = _pos2;
        auto calc_end = std::chrono::steady_clock::now();
        float calc_elapsed = std::chrono::duration<float>(calc_end - calc_start).count();
        //calculate midpoint of the paddle
        std::cout << "time " << calc_elapsed << "\n";
         speed= speed + speed * calc_elapsed*1000;
        std::cout << "speed " << speed << "\n";

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
        _pos1 = _Canvas.size().width / 2;
        _pos2 = _Canvas.size().height / 2;
        _botY_mid_paddle_point = _Canvas.size().height / 2;
        _level_up = 0;
        _start_button = false;
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