#include "stdafx.h"
#include <conio.h>
#include <algorithm>

#include "cvui.h"
#include "CSketch.h"
#include "CBase4618.h"
#include "CControl.h"


#define CANVAS_NAME "Etch-A-Sketch game"

enum { DIGITAL = 0, ANALOG, SERVO };
int type = ANALOG;
int d_type = DIGITAL;
int d_channel = 32;
int reset_channel = 33;
int x_channel = 2;
int y_channel = 26;
int r_channel = 39;
int b_channel = 37;
int g_channel = 38;
int d_val = 0;
int x_val;
int y_val;
int val = 1;
int radian = 2;
int thickness = 2;
int x_offset = 110;
int y_offset = 50;
float speed = 5.0f;
int dir_x, dir_y;


CSketch::CSketch(cv::Size Canva_size, int comport) {   
    control.init_com(comport);
    _Canva = cv::Mat::zeros(Canva_size, CV_8UC3);
}
void CSketch::run() {    
    bool running = true;
    cvui::init(CANVAS_NAME);
    cv::Point Close_position;
    cv::Point Reset_position;
    
    while (running) {
        Close_position = cv::Point(10, 10);
        Reset_position = cv::Point(80, 10);
        if (cvui::button(_Canva, Reset_position.x, Reset_position.y, 60, 40, "reset")) {
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
        else if (cvui::button(_Canva, Close_position.x, Close_position.y, 60, 40, "Exit")) {
            running = false;
            cvui::update();
        }
        else
         {
            update(); 
            draw();   
            cv::waitKey(1);
        }

    }
}
void CSketch::update(){
    control.get_data(d_type, reset_channel, d_val);
    if (control.get_button(d_val)) {
        _reset = true;
    }
  
    
    control.get_data(type, x_channel, x_val);
    control.get_data(type, y_channel, y_val);

    const int DEAD_ZONE = 2;  // You can adjust this threshold value
    int x_analog = static_cast<int>(control.get_analog(x_val));
    int y_analog = static_cast<int>(control.get_analog(y_val));

    // If joystick is within dead zone, set it to 0 (neutral position)
    if (abs(x_analog - 50) < DEAD_ZONE) x_analog = 50;
    if (abs(y_analog - 50) < DEAD_ZONE) y_analog = 50;


    dir_x = ((_Canva.size().width / 3) * (x_analog)) / 100;  // Normalized to -50 to +50 range
    dir_y = ((_Canva.size().height / 3) * (100 - y_analog)) / 100;
    //_x_cursor = dir_x;
    //_y_cursor = dir_y;

    _x_cursor = dir_x + ((_Canva.size().width - 50) * static_cast<int>(control.get_analog(x_val))) / 90;
    _y_cursor = dir_y + ((_Canva.size().height - 100) * (90 - static_cast<int>(control.get_analog(y_val)))) / 90;
   
    //_x_cursor = ((_Canva.size().width)* static_cast<int>(control.get_analog(x_val)))/ 90;
    //_y_cursor = ((_Canva.size().height) * (80 - static_cast<int>(control.get_analog(y_val))))/ 80;

    control.get_data(d_type, d_channel, d_val);
    if (control.get_button(d_val)) {
        if (_color <= 2)
        {
            _color++;
        }
        else
            _color = 0;
    }
    if (_color == 0)
    {
        control.set_data(d_type, g_channel, !val);
        control.set_data(d_type, r_channel, val);
    }
    else if (_color == 1)
    {   
        control.set_data(d_type, r_channel, !val);
        control.set_data(d_type, b_channel, val);
    }
        
    else
    {
        control.set_data(d_type, b_channel, !val);
        control.set_data(d_type, g_channel, val);
    }      
}
void CSketch::draw() {
    
  
    // _x_cursor inside canvas
    if (_x_cursor - x_offset < radian) {
        _x_cursor = radian + x_offset;
    }
    if (_x_cursor - x_offset > _Canva.cols - radian) {
        _x_cursor = _Canva.cols - radian + x_offset;
    }
    // _y_cursor inside canvas
    if (_y_cursor - y_offset < radian) {
        _y_cursor = radian + y_offset;
    }
    if (_y_cursor - y_offset > _Canva.rows - radian) {
        _y_cursor = _Canva.rows - radian + y_offset;
    }
    cv::Point center = cv::Point(_x_cursor - x_offset, _y_cursor - y_offset);
    if (_color == 0) 
        cv::line(_Canva,center, center, cv::Scalar(0, 0, 255), thickness, cv::LINE_AA);
    else if (_color ==1)
        cv::circle(_Canva, center, radian, cv::Scalar(255, 0, 0), thickness, cv::LINE_AA);
    else
        cv::circle(_Canva, center, radian, cv::Scalar(0, 255, 0), thickness, cv::LINE_AA);
    
    cv::imshow(CANVAS_NAME, _Canva);
    if (_reset) {
        _Canva.setTo(cv::Scalar(0, 0, 0));
        _reset = false;
    }
}

CSketch::CSketch() {

}
CSketch ::~CSketch() {

}