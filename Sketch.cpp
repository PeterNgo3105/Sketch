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
int thickness = 4;
int x_offset = 150;
int y_offset = 0;
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
            GPIO();
            update();
            draw();   
            cv::waitKey(1);
        }

    }
}
void CSketch::update(){
    
    if (control.get_button(_d_val)) {
        _reset = true;
    }
    _x_cursor = (((_Canva.size().width+100) * _x_percent) / 90);
    _y_cursor =  (((_Canva.size().height) * (90 - _y_percent)) / 90);
    // _x_cursor inside canvas
    if (_x_cursor -x_offset   < 0) {
        _x_cursor =  x_offset;
    }
    if (_x_cursor + x_offset > _Canva.cols) {
        _x_cursor =x_offset + _Canva.cols ;
    }
    // _y_cursor inside canvas
    if (_y_cursor -y_offset < 0) {
        _y_cursor = y_offset;
    }
    if (_y_cursor +y_offset > _Canva.rows) {
        _y_cursor = _Canva.rows ;
    }
    
       
}
void CSketch::draw() {

    
 
    
     _point1 = cv::Point(_x_cursor - x_offset, _y_cursor + y_offset);
    
    
    
    if (_color == 0)
        cv::line(_Canva, _point1, _point2, cv::Scalar(0, 0, 255), thickness, cv::LINE_AA);   
    else if (_color ==1)
        cv::line(_Canva, _point1, _point2, cv::Scalar(255, 0, 0), thickness, cv::LINE_AA);
    else
        cv::line(_Canva, _point1, _point2, cv::Scalar(0, 255, 0), thickness, cv::LINE_AA);
    //point2 = point1;
    _point2 = _point1;
    cv::imshow(CANVAS_NAME, _Canva);
    if (_reset) {
        _Canva.setTo(cv::Scalar(0, 0, 0));
        _reset = false;
    }
}
void CSketch::GPIO() {
    control.get_data(d_type, reset_channel, _d_val);

    control.get_data(type, x_channel, x_val);
    control.get_data(type, y_channel, y_val);

     _x_percent = static_cast<int>(control.get_analog(x_val));
     _y_percent = static_cast<int>(control.get_analog(y_val));

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

CSketch::CSketch() {

}
CSketch ::~CSketch() {

}