#pragma once

#include "user_data.h"
#include "user_data_roi.h"
#include "get_point.h"

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#define WINDOW_Y_SHIFT      (65)
#define WINDOW_X_SHIFT      (0)
#define SPEED               (1)                         //  Must be >= 1
#define RECT_COLOR          (Scalar(0, 0, 255))         //  B G R
#define RECT_THICKNESS      (1)

void callBackFuncROI(int event, int x, int y, int flags, void* u_data);

void callBackFuncSource(int event, int x, int y, int flags, void* u_data);
