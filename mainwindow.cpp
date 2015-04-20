#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Video Motion Tracking");

    mtd.hide();

    connect(ui->openFile,SIGNAL(triggered()),SLOT(openFile()));
    connect(ui->openURL,SIGNAL(triggered()),SLOT(openURL()));

    connect(ui->motionTrackerTurn,SIGNAL(triggered()),SLOT(motionTrackerTurn()));
    connect(ui->motionTrackerConfig,SIGNAL(triggered()),SLOT(motionTrackerConfig()));

    connect(&vlc,SIGNAL(frameReaded(QImage)),&mt,SLOT(putFrame(QImage)));
    connect(&mt,SIGNAL(frameReady(QImage)),ui->video,SLOT(draw(QImage)));

    connect(&mtd,SIGNAL(apply()),SLOT(setMotionTrackerConfig()));
    connect(&mtd,SIGNAL(accepted()),SLOT(setMotionTrackerConfig()));

    vlc.setSource("v4l2:///dev/video0");
    vlc.play();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openFile()
{
    QString path = QFileDialog::getOpenFileName(this,
                                                "Выберите файл:",
                                                QDir::homePath());
    if (!path.isEmpty())
    {
        vlc.stop();
        vlc.setFile(path);
        vlc.play();
    }
}

void MainWindow::openURL()
{
    bool success = false;
    QString path = QInputDialog::getText(this,
                                         "Открыть URL",
                                         "Введите URL:",
                                         QLineEdit::Normal,
                                         QString(),
                                         &success);
    if (success)
    {
        vlc.stop();
        vlc.setSource(path);
        vlc.play();
    }
}

void MainWindow::motionTrackerTurn()
{
    mt.setEnabled(!mt.enabled());
    ui->motionTrackerTurn->setText(mt.enabled() ? "Отключить" :
                                                  "Включить");
}

void MainWindow::motionTrackerConfig()
{
    mtd.setGeometry(geometry());
    mtd.show();
}

void MainWindow::setMotionTrackerConfig()
{
    mt.set(mtd.config());
}
