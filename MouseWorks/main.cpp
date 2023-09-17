#include <iostream>
#include <windows.h>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

#define IMAGE_PATH          ("Resources/sample1.jpg")   //  путь к файлу: ../MouseWorks/MouseWorks/Resources/image.jpg
#define WINDOW_Y_SHIFT      (65)
#define WINDOW_X_SHIFT      (0)
#define SPEED               (1)                         //  Must be >= 1
#define RECT_COLOR          (Scalar(0, 0, 255))         //  B G R
#define RECT_THICKNESS      (1)

class User_data {
public:
    bool b_down;
    Point p_move, p_down, p_up;
    Mat img;

    User_data  (bool _b_down = false,
                Point _p_move = Point(), Point _p_down = Point(), Point _p_up = Point(),
                Mat _img = Mat())
        : b_down(_b_down)
        , p_move(_p_move)
        , p_down(_p_down)
        , p_up(_p_up)
        , img(_img)
    {}
};

class User_data_roi {
public:
    bool b_down;
    Point p_move, p_down, p_up;
    Mat img;
    uint32_t w, h;
    Point top_left;

    User_data_roi  (bool _b_down = false,
                    Point _p_move = Point(), Point _p_down = Point(), Point _p_up = Point(),
                    Mat _img = Mat(),
                    uint32_t _w = 0, uint32_t _h = 0,
                    Point _top_left = Point())
        : b_down(_b_down)
        , p_move(_p_move)
        , p_down(_p_down)
        , p_up(_p_up)
        , img(_img)
        , w(_w)
        , h(_h)
        , top_left(_top_left)
    {}
};

Point get_top_left(Point& p1, Point& p2) {
    int32_t x, y;
    (p1.x < p2.x) ? x = p1.x : x = p2.x;
    (p1.y < p2.y) ? y = p1.y : y = p2.y;
    return Point(x, y);
}

Point get_bottom_right(Point& p1, Point& p2) {
    int32_t x, y;
    (p1.x > p2.x) ? x = p1.x : x = p2.x;
    (p1.y > p2.y) ? y = p1.y : y = p2.y;
    return Point(x, y);
}


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
    User_data *data = (User_data*)u_data;
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


int main() {

    ShowWindow(GetConsoleWindow(), SW_HIDE);            //     консоль (переключение SH_SHOW / SH_HIDE)

    char filename[] = IMAGE_PATH;
    char fullFilename[MAX_PATH];
    GetFullPathNameA(filename, MAX_PATH, fullFilename, nullptr);

    cout << "Image file path:\t" << fullFilename << endl << endl;
    Mat img = imread(fullFilename, IMREAD_COLOR);       //  считываем изображение из файла

    namedWindow("Source image", WINDOW_AUTOSIZE);
    imshow("Source image", img);                        //  выводим изображение
    moveWindow("Source image", WINDOW_X_SHIFT, WINDOW_Y_SHIFT);
    User_data u_data = User_data();
    u_data.img = img;
    setMouseCallback("Source image", callBackFuncSource, &u_data);
    waitKey(0);

    return 0;
}
