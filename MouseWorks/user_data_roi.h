#pragma once

#include <opencv2/imgproc.hpp>

class User_data_roi {
public:
    bool b_down;
    cv::Point p_move, p_down, p_up;
    cv::Mat img;
    uint32_t w, h;
    cv::Point top_left;

    User_data_roi(bool _b_down = false,
        cv::Point _p_move = cv::Point(), cv::Point _p_down = cv::Point(), cv::Point _p_up = cv::Point(),
        cv::Mat _img = cv::Mat(),
        uint32_t _w = 0, uint32_t _h = 0,
        cv::Point _top_left = cv::Point());

    ~User_data_roi() = default;
};