#include "stdafx.h"
#include "CSpaceInvaderGame.h"
#include "CShip.h"
#include "cvui.h"
#include <conio.h>

#define CANVAS_NAME "Space Invader game"

cv::Point Space_Close_position;
cv::Point Space_Reset_position;
cv::Point position;
cv::Point pos_temp = cv::Point(0,0);
cv::Point2f vel;
CShip S;
CShipMissile SM;
CInvader e;
CMissile m;
bool fire;
int Space_FPS_SP = 34;
int ship_size = 60;
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
int moving;
int moving_temp = 0;
int score = 0;
int raw;
int Button_count = 0;
const int center = 2048;
const int deadZone = 100;
float ship_horizontal = 0;
CSpaceInvaderGame::CSpaceInvaderGame(cv::Size Canva_size, int comport) {
    control.init_com(comport);
    _Canvas = cv::Mat::zeros(Canva_size, CV_8UC3);
}
void CSpaceInvaderGame::init() {
    CInvader e;
    
    
    enemies.clear();
    for (int pos = 0; pos < 30; pos++)
    {
        if (pos <10)
        {         
            position = cv::Point((30 + (pos%10) * 100), 100);
            e.set_pos(position);
        }
        else if (pos >= 10 && pos < 20)
        {
            position = cv::Point((30 + (pos%10) * 100), 150);
            e.set_pos(position);
        }
        else if (pos >= 20 && pos < 30)
        {
            position = cv::Point((30 + (pos % 10) * 100), 200);
            e.set_pos(position);
        }
        enemies.push_back(e);
    }
}
////////////////////////////////////////////
/////Run function
////////////////////////////////////////////
void CSpaceInvaderGame::run() {
    cvui::init(CANVAS_NAME);
    init();
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
    std::string scoreText = std::to_string(score);
    Space_Close_position = cv::Point(10, 10);
    if (S.get_lives() > 0)
    {       
        Space_Reset_position = cv::Point(70, 10);
        cv::putText(_Canvas, "Score", cv::Point(150, 30), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 255, 255), 2);
        cv::putText(_Canvas, scoreText, cv::Point(250, 30), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 255, 0), 2);
        cv::putText(_Canvas, "Live", cv::Point(700, 30), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 255, 255), 2);
        cv::putText(_Canvas, std::to_string(S.get_lives()), cv::Point(800, 30), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 255, 255), 2);
        cv::putText(_Canvas, "Missile", cv::Point(350, 30), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 255, 255), 2);
        //cv::putText(_Canvas, std::to_string(Missile.size()), cv::Point(400, 30), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 255, 255), 2);
        cv::putText(_Canvas, std::to_string(ShipMissile.size()), cv::Point(480, 30), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 255, 255), 2);
        if (cvui::button(_Canvas, Space_Reset_position.x, Space_Reset_position.y, 60, 40, "reset")) {
            _reset = true;
            cvui::update();
        }
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
  
    for (int m_count = 0; m_count < Missile.size(); m_count++)
        Missile[m_count].draw(_Canvas);

    for (int e_count = 0; e_count < enemies.size(); e_count++)
        enemies[e_count].draw(_Canvas);

   
    position = S.get_pos();
    if (_ship_position_percentage < 0.4 && position.x >=0)
    {
        vel.x = -5;
        vel.y = 0;
        S.move(vel);
    }
    else if (_ship_position_percentage > 0.6 && (position.x + ship_size) <= _Canvas.cols)
    {
        vel.x = 5;
        vel.y = 0;
        S.move(vel);
    }
    else
    {
        vel.x = 0;
        vel.y = 0;
        S.move(vel);
    }
   
        S.draw(_Canvas);
    if (_button) {
        position = S.get_pos();
        position.x += + ship_size/2;

         SM.set_pos(position);
         ShipMissile.push_back(SM);

         _button = false;
    }
    for (int sm_count = 0; sm_count < ShipMissile.size(); sm_count++)
    {
        vel.x = 0;
        vel.y = -5;
        ShipMissile[sm_count].move(vel);
        ShipMissile[sm_count].draw(_Canvas);
    }
    if (_reset) {
        _Canvas.setTo(cv::Scalar(0, 0, 0));
        init();
        Missile.clear();
        ShipMissile.clear();
        S.set_lives(3);
        score = 0;
        _reset = false;
    }
    if (S.get_lives()<=0)
    {
        Space_Reset_position = cv::Point(480, 380);
        cv::putText(_Canvas, "your Score:", cv::Point(350, 320), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 255, 255), 2);
        cv::putText(_Canvas, scoreText, cv::Point(600, 320), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 255, 0), 2);
        cv::putText(_Canvas, "GAME OVER", cv::Point(400, 280), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 255), 2);
        if (cvui::button(_Canvas, Space_Reset_position.x, Space_Reset_position.y, 60, 40, "Play Again")) {
            _reset = true;
            cvui::update();
        }
    }
    if (enemies.size() == 0)
    {
        Space_Reset_position = cv::Point(480, 380);
        cv::putText(_Canvas, "your Score:", cv::Point(350, 320), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 255, 255), 2);
        cv::putText(_Canvas, scoreText, cv::Point(600, 320), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 255, 0), 2);
        cv::putText(_Canvas, "YOU WIN", cv::Point(400, 280), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 255), 2);
        if (cvui::button(_Canvas, Space_Reset_position.x, Space_Reset_position.y, 60, 40, "Play Again")) {
            _reset = true;
            cvui::update();
        }
    }
    
    cv::imshow(CANVAS_NAME, _Canvas);
    _Canvas.setTo(cv::Scalar(0, 0, 0));
    cv::waitKey(1);
}
////////////////////////////////////////////
/////   GPIO Function
////////////////////////////////////////////
void CSpaceInvaderGame::GPIO() {
    
    control.get_data(analog, Horizontal_dir_channel, Horizontal_val);
    //raw = static_cast<int>(control.get_analog(Horizontal_val));
    _ship_position_percentage = static_cast<float>(control.get_analog(Horizontal_val))/100;
    //auto end_time = std::chrono::system_clock::now() + std::chrono::milliseconds(1000 / Space_FPS_SP);
    control.get_data(R_Digital, R_channel, R_val);

    if (R_val == 0) {
        _reset = true;
    }
    control.get_data(Shoot_Digital, Digital_channel, digi_val);
    if (control.get_button(digi_val)) {
        _button = true;
        Button_count++;
        std::cout << "Missiles launded: " << Button_count << "\n";

    }

    // Sleep if time remaining
    //std::this_thread::sleep_until(end_time);
}
////////////////////////////////////////////
/////   Update Function
////////////////////////////////////////////
void CSpaceInvaderGame::update() 
{
    for (int in_Index = 0; in_Index < enemies.size(); in_Index++) {
        position = enemies[in_Index].get_pos();
        if ((position.x - pos_temp.x) > 0) {
            if ((position.x + 60) >= _Canvas.cols) {
                moving = 1;             //moving down
                break;
            }
            else
            {
                moving = 0;             //moving forward
            }
        }
        else
        {
            if (position.x <= 0) {
                moving = 1;             //moving down
                break;
            }
            else
            {
                moving = 2;             //moving backward
            }
        }
    }
    if (moving == 0)
    {
        vel.x = 1;
        vel.y = 0;
        for (auto& e : enemies) {
            e.move(vel);
        }
    }
    if (moving == 2)
    {
        vel.x = -1;
        vel.y = 0;
        for (auto& e : enemies) {
            e.move(vel);
        }
    }
    if (moving == 1)
    {
        vel.x = 0;
        vel.y = 5;
        for (auto& e : enemies) {
            e.move(vel);
        }
        if (moving_temp == 0)
            position.x = _Canvas.size().width + 5;
        else if (moving_temp == 2)
            position.x = -5;
    }
    pos_temp = position;
    moving_temp = moving;
    _point_horizontal_mid = (((_Canvas.size().width) * _ship_position_percentage) / 80);
    //std::cout << " position " << _point_horizontal_mid << "\n";
    // _x_cursor inside canvas
    
    if (loop == 3 && !enemies.empty()) {
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
    // Remove missiles that leave the screen
    Missile.erase
    (
        std::remove_if(Missile.begin(), Missile.end(),
            [&](CMissile& m)
            { 
                position = m.get_pos();
                return position.y > _Canvas.rows; 
            }),
        Missile.end()
    );
    // Remove player missiles that leace screen
    ShipMissile.erase
    (
        std::remove_if(ShipMissile.begin(), ShipMissile.end(),
            [&](CShipMissile& sm)
            {
                position = sm.get_pos();
                return position.y < 0;
            }),
        ShipMissile.end()
    );
    // checking collision between Invader missiles and player ship
    if (S.get_lives() > 0) 
    {
        for (auto& m : Missile)
        {
            if (m.collide(S))
            {
                S.hit();
                m.hit();
                
            }
        }
    }
    // Remove Invader Missiles if it hit player ship
    Missile.erase
    (
        std::remove_if(Missile.begin(), Missile.end(),
            [&](CMissile& m)
            {

                return m.get_lives() <= 0;
            }),
        Missile.end()
    );
    // Checking collision between Ship missiles and Invader
    for (auto& sm : ShipMissile)
    {
        for (auto& e : enemies)
        {
            if (sm.collide(e))
            {
                e.hit();
                sm.hit();
                score += 10;
            }
        }
    }
    // Remove Invader 
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](CInvader& e)
            {
                return e.get_lives() <= 0;
            }),
        enemies.end()
    );
    // Remove PLayer ship missiles
    ShipMissile.erase(
        std::remove_if(ShipMissile.begin(), ShipMissile.end(),
            [](CShipMissile& sm)
            {
                return sm.get_lives() <= 0;
            }),
        ShipMissile.end()
    );
    for (auto& e : enemies)
    {
        if (e.collide(S))
        {
            S.hit();
            e.hit();

        }
    }
    // Remove Invader 
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](CInvader& e)
            {
                return e.get_lives() <= 0;
            }),
        enemies.end()
    );


 
        
}