#include "callbacks.h"

#include <windows.h>
#include <iostream>

#include <opencv2/imgproc.hpp>
#include "INIReader.h"

using namespace cv;
using namespace std;

//#define IMAGE_PATH          ("Resources/sample1.jpg")   //  путь к файлу: ../MouseWorks/MouseWorks/Resources/image.jpg

//char img_path[MAX_PATH] = "";
string img_path;
Size img_sz;


bool get_ini_params(const string& config) {

    setlocale(LC_NUMERIC, "en_US.UTF-8");
    INIReader reader(config);
    if (reader.ParseError() < 0)
    {
        cout << "Can't load '" << config << "'\n";
        return 0;
    }
    // ------------------------------ source_img: 
    img_sz.width = reader.GetInteger("source_img", "img_w", -1);
    if (img_sz.width == -1) {
        cout << "img_w not declared\n";
    }
    else {
        cout << "img_w = " << img_sz.width << ";\n";
    }

    img_sz.height = reader.GetInteger("source_img", "img_h", -1);
    if (img_sz.height == -1) {
        cout << "img_h not declared\n";
    }
    else {
        cout << "img_h = " << img_sz.height << ";\n";
    }

    img_path = reader.Get("source_img", "img_path", "");
    if (img_path == "") {
        cout << "img_path not declared!\n";
        return 0;
    }
    else {
        cout << "img_path = " << img_path << ";\n";
    }
    // ----------------------------------------
    return 1;
}


int main() {

    ShowWindow(GetConsoleWindow(), SW_HIDE);            //     консоль (переключение SH_SHOW / SH_HIDE)

    /*char filename[] = IMAGE_PATH;
    char fullFilename[MAX_PATH];
    GetFullPathNameA(filename, MAX_PATH, fullFilename, nullptr);
    Mat img = imread(fullFilename, IMREAD_COLOR);       //  считываем изображение из файла*/

    bool is_ok = get_ini_params("img_data.ini");
    if (!is_ok) {
        cout << "ini file error\n";
        return 0;
    }

    Mat img = imread(img_path, IMREAD_COLOR);       //  считываем изображение из файла
    if (img_sz.width > 0 && img_sz.height > 0) {
        img = img(Rect(Point(), img_sz));
    }

    namedWindow("Source image", WINDOW_AUTOSIZE);
    imshow("Source image", img);                        //  выводим изображение
    moveWindow("Source image", WINDOW_X_SHIFT, WINDOW_Y_SHIFT);

    User_data u_data = User_data();
    u_data.img = img;
    setMouseCallback("Source image", callBackFuncSource, &u_data);
    waitKey(0);

    return 0;
}
