#ifndef MOTIONTRACKER_H
#define MOTIONTRACKER_H

#include "motiontracker_type.h"

#include <QThread>
#include <QImage>
#include <QQueue>

#include <QDebug>

class MotionTracker : public QObject
{
    Q_OBJECT

    bool m_enabled;

    QThread thread;
    QQueue<QImage> frame_q;

    QImage frame;

    MotionTrackerMat mat;

    QMutex config_mutex;
    MotionTrackerConfig config;

    QImage Mat2QImage(cv::Mat const& src,int cvt);
    cv::Mat QImage2Mat(QImage const& src,int cvt);

public:
    explicit MotionTracker(QObject *parent = 0);
    ~MotionTracker();

    void set(MotionTrackerConfig m);

signals:
    void frameReady(QImage img);

public slots:
    void putFrame(QImage img);

    bool enabled();
    void setEnabled(bool en);

private slots:
    void opencv_run(QImage img);
    void draw_motion_comp(cv::Mat& img, const cv::Rect rec, double angle);

};

#endif // MOTIONTRACKER_H
