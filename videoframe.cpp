#include "videoframe.h"

VideoFrame::VideoFrame(QWidget *parent) :
    QWidget(parent)
{
}

void VideoFrame::paintEvent(QPaintEvent* ev)
{
    QPainter p(this);
    p.fillRect(ev->rect(),Qt::black);

    double w = width();
    double h = height();

    double pw = frame.width();
    double ph = frame.height();

    double wf = pw / w;
    double hf = ph / h;

    double f = 0;

    if      (wf > hf)   f = wf;
    else if (hf > wf)   f = hf;

    pw = pw / f;
    ph = ph / f;

    frameRect = QRectF((w-pw)/2.0, (h-ph)/2.0, pw, ph).toRect();
    p.drawImage(frameRect,frame);
}

void VideoFrame::draw(QImage img)
{
    frame = img;
    update();
}
