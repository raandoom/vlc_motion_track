#ifndef MOTIONTRACKERDIALOG_H
#define MOTIONTRACKERDIALOG_H

#include <QDialog>
#include <QAbstractButton>

#include "motiontracker_type.h"

namespace Ui {
class MotionTrackerDialog;
}

class MotionTrackerDialog : public QDialog
{
    Q_OBJECT

    Ui::MotionTrackerDialog *ui;

public:
    explicit MotionTrackerDialog(QWidget *parent = 0);
    ~MotionTrackerDialog();

    MotionTrackerConfig config();

signals:
    void apply();

private slots:
    void showWTF();
    void sliderChanged();
    void spinChanged();

    void bbClicked(QAbstractButton* b);
};

#endif // MOTIONTRACKERDIALOG_H
