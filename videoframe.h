#ifndef VIDEOFRAME_H
#define VIDEOFRAME_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>

#include <QDebug>

class VideoFrame : public QWidget
{
    Q_OBJECT

    QImage surface;

    QImage frame;
    QRect frameRect;

public:
    explicit VideoFrame(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent* ev);

public slots:
    void draw(QImage img);

};

#endif // VIDEOFRAME_H
