#include "motiontrackerdialog.h"
#include "ui_motiontrackerdialog.h"

#include <QWhatsThis>

MotionTrackerDialog::MotionTrackerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MotionTrackerDialog)
{
    ui->setupUi(this);
    setWindowTitle("Motion Tracker Config");

    ui->wtfDuration->setWhatsThis("Maximal duration of the motion track in the same units as timestamp");
    ui->wtfBlur->setWhatsThis("Size of blur, blur, which will be applied to the frame difference");
    ui->wtfDiffThresh->setWhatsThis("Threshold for frame difference");
    ui->wtfSegThresh->setWhatsThis("Segmentation threshold that is recommended to be equal to the interval between motion history “steps” or greater");
    ui->wtfDeltaMin->setWhatsThis("Minimal (or maximal) allowed difference between mhi values within a pixel neighborhood");
    ui->wtfDeltaMax->setWhatsThis("Maximal (or minimal) allowed difference between mhi values within a pixel neighborhood");
    ui->wtfRectMin->setWhatsThis("Minimal area size for drawing region of interest");
    ui->wtfRectMax->setWhatsThis("Maximal area size for drawing region of interest");
    ui->wtfOrientation->setWhatsThis("Trying to show motion direction");

    connect(ui->wtfDuration,    SIGNAL(clicked()),SLOT(showWTF()));
    connect(ui->wtfBlur,        SIGNAL(clicked()),SLOT(showWTF()));
    connect(ui->wtfDiffThresh,  SIGNAL(clicked()),SLOT(showWTF()));
    connect(ui->wtfSegThresh,   SIGNAL(clicked()),SLOT(showWTF()));
    connect(ui->wtfDeltaMin,    SIGNAL(clicked()),SLOT(showWTF()));
    connect(ui->wtfDeltaMax,    SIGNAL(clicked()),SLOT(showWTF()));
    connect(ui->wtfRectMin,     SIGNAL(clicked()),SLOT(showWTF()));
    connect(ui->wtfRectMax,     SIGNAL(clicked()),SLOT(showWTF()));
    connect(ui->wtfOrientation, SIGNAL(clicked()),SLOT(showWTF()));

    ui->durationSpinBox->setRange(0,30000);
    ui->durationSlider->setRange(0,30000);

    ui->blurSpinBox->setRange(0,50);
    ui->blurSlider->setRange(0,50);

    ui->diffThreshSpinBox->setRange(0,255);
    ui->diffThreshSlider->setRange(0,255);

    ui->segThreshSpinBox->setRange(0,30000);
    ui->segThreshSlider->setRange(0,30000);

    ui->deltaMinSpinBox->setRange(0,255);
    ui->deltaMinSlider->setRange(0,255);

    ui->deltaMaxSpinBox->setRange(0,255);
    ui->deltaMaxSlider->setRange(0,255);

    ui->rectMinSpinBox->setRange(0,100000);
    ui->rectMinSlider->setRange(0,100000);

    ui->rectMaxSpinBox->setRange(0,100000);
    ui->rectMaxSlider->setRange(0,100000);

    connect(ui->durationSlider,     SIGNAL(valueChanged(int)),      SLOT(sliderChanged()));
    connect(ui->durationSpinBox,    SIGNAL(valueChanged(double)),   SLOT(spinChanged()));
    connect(ui->blurSlider,         SIGNAL(valueChanged(int)),      SLOT(sliderChanged()));
    connect(ui->blurSpinBox,        SIGNAL(valueChanged(double)),   SLOT(spinChanged()));
    connect(ui->diffThreshSlider,   SIGNAL(valueChanged(int)),      SLOT(sliderChanged()));
    connect(ui->diffThreshSpinBox,  SIGNAL(valueChanged(double)),   SLOT(spinChanged()));
    connect(ui->segThreshSlider,    SIGNAL(valueChanged(int)),      SLOT(sliderChanged()));
    connect(ui->segThreshSpinBox,   SIGNAL(valueChanged(double)),   SLOT(spinChanged()));
    connect(ui->deltaMinSlider,     SIGNAL(valueChanged(int)),      SLOT(sliderChanged()));
    connect(ui->deltaMinSpinBox,    SIGNAL(valueChanged(double)),   SLOT(spinChanged()));
    connect(ui->deltaMaxSlider,     SIGNAL(valueChanged(int)),      SLOT(sliderChanged()));
    connect(ui->deltaMaxSpinBox,    SIGNAL(valueChanged(double)),   SLOT(spinChanged()));

    connect(ui->rectMinSlider,  SIGNAL(valueChanged(int)),ui->rectMinSpinBox,   SLOT(setValue(int)));
    connect(ui->rectMinSpinBox, SIGNAL(valueChanged(int)),ui->rectMinSlider,    SLOT(setValue(int)));
    connect(ui->rectMaxSlider,  SIGNAL(valueChanged(int)),ui->rectMaxSpinBox,   SLOT(setValue(int)));
    connect(ui->rectMaxSpinBox, SIGNAL(valueChanged(int)),ui->rectMaxSlider,    SLOT(setValue(int)));

    connect(ui->buttonBox,SIGNAL(clicked(QAbstractButton*)),SLOT(bbClicked(QAbstractButton*)));

    MotionTrackerConfig def;
    ui->durationSpinBox->setValue(def.duration);
    ui->blurSpinBox->setValue(def.blur.width);
    ui->diffThreshSpinBox->setValue(def.diffThresh);
    ui->segThreshSpinBox->setValue(def.segThresh);
    ui->deltaMinSpinBox->setValue(def.delta_min);
    ui->deltaMaxSpinBox->setValue(def.delta_max);
    ui->rectMinSpinBox->setValue(def.rect_min);
    ui->rectMaxSpinBox->setValue(def.rect_max);
    ui->orientation->setChecked(def.orientation);
}

MotionTrackerDialog::~MotionTrackerDialog()
{
    delete ui;
}

MotionTrackerConfig MotionTrackerDialog::config()
{
    MotionTrackerConfig mc;
    mc.duration = ui->durationSpinBox->value();
    mc.blur = cv::Size(ui->blurSpinBox->value(),ui->blurSpinBox->value());
    mc.diffThresh = ui->diffThreshSpinBox->value();
    mc.segThresh = ui->segThreshSpinBox->value();
    mc.delta_min = ui->deltaMinSpinBox->value();
    mc.delta_max = ui->deltaMaxSpinBox->value();
    mc.rect_min = ui->rectMinSpinBox->value();
    mc.rect_max = ui->rectMaxSpinBox->value();
    mc.orientation = ui->orientation->isChecked();

    return mc;
}

void MotionTrackerDialog::showWTF()
{
    QToolButton* b = qobject_cast<QToolButton*>(QObject::sender());
    if (b)
    {
        QWhatsThis::showText(mapToGlobal(b->pos()),b->whatsThis());
    }
}

void MotionTrackerDialog::spinChanged()
{
    QDoubleSpinBox* sp = qobject_cast<QDoubleSpinBox*>(QObject::sender());

    if (sp == ui->durationSpinBox)      ui->durationSlider->setValue(sp->value());
    if (sp == ui->blurSpinBox)          ui->blurSlider->setValue(sp->value());
    if (sp == ui->diffThreshSpinBox)    ui->diffThreshSlider->setValue(sp->value());
    if (sp == ui->segThreshSpinBox)     ui->segThreshSlider->setValue(sp->value());
    if (sp == ui->deltaMinSpinBox)      ui->deltaMinSlider->setValue(sp->value());
    if (sp == ui->deltaMaxSpinBox)      ui->deltaMaxSlider->setValue(sp->value());
}

void MotionTrackerDialog::sliderChanged()
{
    QSlider* sl = qobject_cast<QSlider*>(QObject::sender());

    if (sl == ui->durationSlider)   ui->durationSpinBox->setValue(sl->value());
    if (sl == ui->blurSlider)       ui->blurSpinBox->setValue(sl->value());
    if (sl == ui->diffThreshSlider) ui->diffThreshSpinBox->setValue(sl->value());
    if (sl == ui->segThreshSlider)  ui->segThreshSpinBox->setValue(sl->value());
    if (sl == ui->deltaMinSlider)   ui->deltaMinSpinBox->setValue(sl->value());
    if (sl == ui->deltaMaxSlider)   ui->deltaMaxSpinBox->setValue(sl->value());
}

void MotionTrackerDialog::bbClicked(QAbstractButton *b)
{
    if (ui->buttonBox->buttonRole(b) == QDialogButtonBox::ApplyRole)
    {
        emit apply();
    }
}
