#include "stdafx.h"
#include "CSpaceInvaderGame.h"
#include "CShip.h"
#include "cvui.h"
#include <conio.h>

#define CANVAS_NAME "Space Invader game"

cv::Point Space_Close_position;
cv::Point Space_Reset_position;
cv::Point position;

bool fire;
int Space_FPS_SP = 34;
int ship_size = 20;
int R_Digital = 0;
int analog = 1;
int Shoot_Digital = 0, Digital_channel = 33, digi_val = 1;
int R_channel = 32;
int R_val = 1;
int Horizontal_dir_channel = 2;
int Horizontal_val;
int enemies_moving_Forward = 10;
int enemies_moving_Backward = 22;
int initial = 0;
int loop = 0;

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
    CInvader e;
    CMissile m;
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

    if (initial == 0) 
    {
        enemies.clear();
        for (int pos = 0; pos < 10; pos++)
        {
            position = cv::Point((30 + pos * 100), 100);
            e.set_pos(position);
            enemies.push_back(e);
        }
        initial = 1;
    }
   
    if (enemies_moving_Forward <= 20) 
    {
        for (auto& e : enemies) {
            e.move(1);
        }
        /*for (auto& e : enemies) {
            position = e.get_pos();
            position.x += 2;
            e.set_pos(position);
        }*/
   
        enemies_moving_Forward++;
    }
    else if (enemies_moving_Forward > 20 && enemies_moving_Backward ==22 && enemies_moving_Forward != 22) {
        enemies_moving_Backward = 0;
        enemies_moving_Forward = 22;
    }

    if (enemies_moving_Backward <= 20)
    {  
        for (auto& e : enemies) {
            e.move(-1);
        }
        /*for (auto& e : enemies) {
            position = e.get_pos();
            position.x -= 2;
            e.set_pos(position);
        }  */
        enemies_moving_Backward++;
    }
    else if(enemies_moving_Backward > 20 && enemies_moving_Forward == 22 && enemies_moving_Backward!=22)
    {
        enemies_moving_Forward = 0;
        enemies_moving_Backward = 22;       
    }
   
     if (loop ==10 && !enemies.empty()) {
        // Random enemy shoots
        int idx = rand() % enemies.size();
        position = enemies[idx].get_pos();
        position.x = position.x + 20;
        m.set_pos(position);
        Missile.push_back(m);
        loop = 0;   
    }
     for (auto& m : Missile) {
         position = m.get_pos();
         position.y += 5;
         m.set_pos(position);
     }
     loop++;
   
    for (int m_count = 0; m_count < Missile.size(); m_count++)
        Missile[m_count].draw(_Canvas);

    for (int e_count = 0; e_count < enemies.size(); e_count++)
        enemies[e_count].draw(_Canvas);
    CShip s;
    cv::Point S_pos = cv::Point(_point_horizontal_mid - ship_size, _Canvas.size().height - 5);
        s.set_pos(S_pos);
        s.draw(_Canvas);
    
    cv::imshow(CANVAS_NAME, _Canvas);
    _Canvas.setTo(cv::Scalar(0, 0, 0));
    cv::waitKey(1);
}
////////////////////////////////////////////
/////   GPIO Function
////////////////////////////////////////////
void CSpaceInvaderGame::GPIO() {
    //auto end_time = std::chrono::system_clock::now() + std::chrono::milliseconds(1000 / Space_FPS_SP);
    control.get_data(R_Digital, R_channel, R_val);

    if (R_val == 0) {
        _reset = true;
    }
    control.get_data(Shoot_Digital, Digital_channel, digi_val);
    
        if (control.get_button(digi_val)) {
            fire = true;
        }
        else
        {
            fire = false;
        }

    control.get_data(analog, Horizontal_dir_channel, Horizontal_val);
    _ship_position_percentage = static_cast<int>(control.get_analog(Horizontal_val));
    // Sleep if time remaining
    //std::this_thread::sleep_until(end_time);
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
    // Remove missiles that leave the screen
    Missile.erase(
        std::remove_if(Missile.begin(), Missile.end(),
            [&](CMissile& m)
            { position = m.get_pos();
    return position.y > _Canvas.rows; }),
        Missile.end()
    );
}