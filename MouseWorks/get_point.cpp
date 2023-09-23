#include "get_point.h"

using namespace cv;

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