#include "callbacks.h"

//#include <iostream>
#include <windows.h>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

#define IMAGE_PATH          ("Resources/sample1.jpg")   //  путь к файлу: ../MouseWorks/MouseWorks/Resources/image.jpg

int main() {

    ShowWindow(GetConsoleWindow(), SW_HIDE);            //     консоль (переключение SH_SHOW / SH_HIDE)

    char filename[] = IMAGE_PATH;
    char fullFilename[MAX_PATH];
    GetFullPathNameA(filename, MAX_PATH, fullFilename, nullptr);

    //std::cout << "Image file path:\t" << fullFilename << std::endl << std::endl;
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
