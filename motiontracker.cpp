#include "motiontracker.h"

MotionTracker::MotionTracker(QObject *parent) :
    QObject(parent),
    m_enabled(false)
{
    moveToThread(&thread);
    thread.start();
}

MotionTracker::~MotionTracker()
{
    thread.quit();
    thread.wait();
}

void MotionTracker::set(MotionTrackerConfig m)
{
    config_mutex.lock();
    config = m;
    config_mutex.unlock();
}

void MotionTracker::putFrame(QImage img)
{
    mat.need_init =
            (img.size().width() != mat.mhi.cols) ||
            (img.size().height() != mat.mhi.rows);

    QMetaObject::invokeMethod(this,"opencv_run",Q_ARG(QImage,img));
}

bool MotionTracker::enabled()
{
    return m_enabled;
}

void MotionTracker::setEnabled(bool en)
{
    if (en && !m_enabled)
    {
        mat.need_init = true;
    }
    m_enabled = en;
}

void MotionTracker::opencv_run(QImage img)
{
    if (!m_enabled)
    {
        emit frameReady(img);
    }
    else
    {
        if (!img.isNull())
        {
            cv::Mat now_mat = QImage2Mat(img,CV_RGBA2RGB);
            if (mat.need_init)
            {
                mat.need_init   = false;
                cv::Size size   = now_mat.size();
                mat.mhi         = cv::Mat(size, CV_32FC1,   cv::Scalar(0,0,0));
                mat.mask        = cv::Mat(size, CV_8UC1,    cv::Scalar(0,0,0));
                mat.orientation = cv::Mat(size, CV_32FC1,   cv::Scalar(0,0,0));
                mat.segmask     = cv::Mat(size, CV_32FC1,   cv::Scalar(0,0,0));

                frame_q.clear();
            }

            frame_q.enqueue(img);
            if (frame_q.count() == 2)
            {
                config_mutex.lock();
                MotionTrackerConfig mc = config;
                config_mutex.unlock();

                cv::Mat pre_mat = QImage2Mat(frame_q.dequeue(),CV_RGBA2RGB);

                cv::Mat diff;
                cv::absdiff(now_mat,pre_mat,diff);
                cv::cvtColor(diff,diff,CV_RGB2GRAY);

                if (mc.blur != cv::Size(0,0))
                {
                    cv::blur(diff,diff,mc.blur);
                }

                cv::threshold(diff,diff,mc.diffThresh,255,CV_THRESH_BINARY);

                double timestamp = 1000.0 * clock() / CLOCKS_PER_SEC;

                cv::updateMotionHistory(diff,mat.mhi,timestamp,mc.duration);

                if (mc.orientation)
                {
                    cv::calcMotionGradient(mat.mhi,mat.mask,mat.orientation,mc.delta_min,mc.delta_max,3);
                }

                std::vector<cv::Rect> boundingRects;
                cv::segmentMotion(mat.mhi,mat.segmask,boundingRects,timestamp,mc.segThresh);


                foreach (const cv::Rect& rec, boundingRects)
                {
                    if(rec.area() > mc.rect_min && rec.area() < mc.rect_max)
                    {
                        cv::rectangle(now_mat,rec,cv::Scalar(0,0,255),3);

                        if (mc.orientation)
                        {
                            // cv::Mat silhouette = diff(rec);
                            cv::Mat orientation_rect = mat.orientation(rec);
                            cv::Mat mask_rect = mat.mask(rec);
                            cv::Mat mhi_rect = mat.mhi(rec);

                            // if(cv::norm(silhouette,cv::NORM_L2,cv::noArray()) > rec.area() * 0.5)
                            {
                                double angle = cv::calcGlobalOrientation(orientation_rect,mask_rect,mhi_rect,timestamp,mc.duration);
                                draw_motion_comp(now_mat,rec,angle);
                            }
                        }
                    }
                }

                emit frameReady(Mat2QImage(now_mat,CV_RGB2RGBA));
            }
        }
    }
}

void MotionTracker::draw_motion_comp(cv::Mat& img, const cv::Rect rec, double angle)
{
    int r = 0;
    if(rec.height / 2 <= rec.width / 2)
    {
        r = rec.height / 2;
    }
    else
    {
        r = rec.width / 2;
    }

    int cx = rec.x + rec.width / 2;
    int cy = rec.y + rec.height / 2;

    angle = angle * M_PI / 180;
    cv::circle(img,
               cv::Point(cx,cy),
               r,
               cv::Scalar(255,0,0));
    cv::line(img,
             cv::Point(cx,cy),
             cv::Point(int(cx + cos(angle) * r),int(cy + sin(angle) * r)),
             cv::Scalar(255,0,0));
}

QImage MotionTracker::Mat2QImage(cv::Mat const& src,int cvt)
{
    cv::Mat temp;
    cv::cvtColor(src,temp,cvt);
    return QImage((const uchar*)temp.data,temp.cols,temp.rows,temp.step,QImage::Format_RGB32).copy();
}

cv::Mat MotionTracker::QImage2Mat(QImage const& src,int cvt)
{
    cv::Mat result(src.height(),src.width(),CV_8UC4,(uchar*)src.bits(),src.bytesPerLine());
    cv::cvtColor(result,result,cvt);
    return result;
}
