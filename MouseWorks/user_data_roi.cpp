#include "user_data_roi.h"

using namespace cv;

User_data_roi::User_data_roi(bool _b_down,
                             Point _p_move, Point _p_down, Point _p_up,
                             Mat _img,
                             uint32_t _w, uint32_t _h,
                             Point _top_left)
    : b_down(_b_down)
    , p_move(_p_move)
    , p_down(_p_down)
    , p_up(_p_up)
    , img(_img)
    , w(_w)
    , h(_h)
    , top_left(_top_left)
{}