#include "callbacks.h"

using namespace cv;

void callBackFuncROI(int event, int x, int y, int flags, void* u_data) {
    User_data_roi* data = (User_data_roi*)u_data;
    switch (event) {
    case EVENT_LBUTTONDOWN: {
        (*data).p_down = Point(x, y);
        (*data).b_down = true;
        break;
    }
    case EVENT_LBUTTONUP: {
        (*data).p_up = Point(x, y);
        (*data).b_down = false;
        break;
    }
    case EVENT_MOUSEMOVE: {
        (*data).p_move = Point(x, y);
        if ((*data).b_down == true) {

            int32_t shift_x = (*data).p_move.x - (*data).p_down.x;
            int32_t shift_y = (*data).p_move.y - (*data).p_down.y;

            (*data).top_left.x += shift_x * SPEED;    // get the X shift
            (*data).top_left.y += shift_y * SPEED;    // get the Y shift

            Point bottom_right = Point((*data).top_left.x + (*data).w, (*data).top_left.y + (*data).h); //  calculate

            //  boundary conditions:
            if ((*data).top_left.x < 0) {
                (*data).top_left.x = 0;
            }
            if ((*data).top_left.y < 0) {
                (*data).top_left.y = 0;
            }
            if (bottom_right.x > (*data).img.cols) {
                (*data).top_left.x = (*data).img.cols - (*data).w;
            }
            if (bottom_right.y > (*data).img.rows) {
                (*data).top_left.y = (*data).img.rows - (*data).h;
            }

            bottom_right = Point((*data).top_left.x + (*data).w, (*data).top_left.y + (*data).h);   //  refresh
            Mat new_roi((*data).img, Rect((*data).top_left, bottom_right));
            imshow("ROI", new_roi);

            (*data).p_down.x += shift_x;
            (*data).p_down.y += shift_y;
        }
        break;
    }
    }
}

void callBackFuncSource(int event, int x, int y, int flags, void* u_data) {
    User_data* data = (User_data*)u_data;
    switch (event) {
    case EVENT_LBUTTONDOWN: {
        (*data).p_down = Point(x, y);
        (*data).b_down = true;
        break;
    }
    case EVENT_LBUTTONUP: {
        (*data).p_up = Point(x, y);
        if ((*data).b_down == true) {
            (*data).b_down = false;

            imshow("Source image", (*data).img);

            Point top_left = get_top_left((*data).p_down, (*data).p_up);
            Point bottom_right = get_bottom_right((*data).p_down, (*data).p_up);

            //  boundary conditions:
            if (top_left.x < 0) {
                top_left.x = 0;
            }
            if (top_left.y < 0) {
                top_left.y = 0;
            }
            if (bottom_right.x > (*data).img.cols) {
                bottom_right.x = (*data).img.cols;
            }
            if (bottom_right.y > (*data).img.rows) {
                bottom_right.y = (*data).img.rows;
            }

            Mat roi((*data).img, Rect(top_left, bottom_right));
            namedWindow("ROI", WINDOW_AUTOSIZE);
            imshow("ROI", roi);
            moveWindow("ROI", top_left.x + WINDOW_X_SHIFT, top_left.y + WINDOW_Y_SHIFT);

            User_data_roi data_roi = User_data_roi();
            data_roi.img = (*data).img;
            data_roi.w = bottom_right.x - top_left.x;
            data_roi.h = bottom_right.y - top_left.y;
            data_roi.top_left = top_left;

            setMouseCallback("ROI", callBackFuncROI, &data_roi);

            waitKey(0);
            destroyWindow("ROI");
        }
        break;
    }
    case EVENT_MOUSEMOVE: {
        (*data).p_move = Point(x, y);
        if ((*data).b_down == true) {
            Mat img_mark = (*data).img.clone();
            rectangle(img_mark, (*data).p_down, (*data).p_move, RECT_COLOR, RECT_THICKNESS);
            imshow("Source image", img_mark);   //  show roi border
        }
        break;
    }
    }
}