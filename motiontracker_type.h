#ifndef MOTIONTRACKER_TYPE_H
#define MOTIONTRACKER_TYPE_H

#include <QMutex>

#include <cv.h>

struct MotionTrackerConfig {
    double      duration;
    cv::Size    blur;
    double      diffThresh;
    double      segThresh;
    double      delta_min;
    double      delta_max;
    int         rect_min;
    int         rect_max;
    bool        orientation;

    MotionTrackerConfig() :
        duration(200),
        blur(cv::Size(10,10)),
        diffThresh(30),
        segThresh(300),
        delta_min(1),
        delta_max(255),
        rect_min(1000),
        rect_max(100000),
        orientation(false)
    {

    }
};

struct MotionTrackerMat {
    bool need_init;
    cv::Mat mhi;
    cv::Mat mask;
    cv::Mat orientation;
    cv::Mat segmask;

    MotionTrackerMat() :
        need_init(true)
    {

    }
};

#endif // MOTIONTRACKER_TYPE_H
