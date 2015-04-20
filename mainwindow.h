#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "vlcreader.h"
#include "motiontracker.h"
#include "motiontrackerdialog.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Ui::MainWindow *ui;

    MotionTrackerDialog mtd;

    VLCReader vlc;
    MotionTracker mt;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void openFile();
    void openURL();

    void motionTrackerTurn();
    void motionTrackerConfig();
    void setMotionTrackerConfig();
};

#endif // MAINWINDOW_H
