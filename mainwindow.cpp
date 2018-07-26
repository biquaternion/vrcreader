#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QListView>
#include "vrc.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //QListView lv; = new QListView(this);
    //ui->gridLayout->addWidget(&lv);

    connect(ui->openButton, SIGNAL(clicked()),
            this, SLOT(onOpenClick()));
    connect(ui->playButton, SIGNAL(clicked()),
            this, SIGNAL(onPlayClick()));
    connect(ui->pauseButton, SIGNAL(clicked()),
            this, SIGNAL(onPauseClick()));
    connect(ui->saveButton, SIGNAL(clicked()),
            this, SIGNAL(onSaveFrameClick()));
    connect(ui->cbOutput, SIGNAL(toggled(bool)),
            this, SIGNAL(onCBOutputCLick(bool)));
    connect(ui->backButton, SIGNAL(clicked()),
            this, SIGNAL(onBackClick()));
    connect(ui->navigationSlider, SIGNAL(valueChanged(int)),
            this, SIGNAL(rewind(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onOpenClick()
{
    _fileName = QFileDialog::getOpenFileName(0, "Choose File", "C:\\Dima\\WORK\\Sajene\\ARYA\\records");
    emit takeFileName(_fileName, ui->cbOutput->isChecked());
}
void MainWindow::changeOutputFlag()
{

}
void MainWindow::metaData(VRCHeader *header)
{
    QStringList sl;
    sl << "Version       " + QString::number(header->fVersion);
    sl << "Frames        " + QString::number(header->fFrames);
    sl << "Width         " + QString::number(header->fWidth);
    sl << "Height        " + QString::number(header->fHeight);
    sl << "Image Type    " + QString::number(header->fImageType);
    sl << "Bits          " + QString::number(header->fBits);
    sl << "Ofs R         " + QString::number(header->fOfsR);
    sl << "Bits R        " + QString::number(header->fBitsR);
    sl << "Ofs G         " + QString::number(header->fOfsG);
    sl << "Bits G        " + QString::number(header->fBitsG);
    sl << "Ofs B         " + QString::number(header->fOfsB);
    sl << "Bits B        " + QString::number(header->fBitsB);
    sl << "Interval      " + QString::number(header->fInterval);
    sl << "Str Order     " + QString::number(header->fStrOrder);

    slModel.setStringList(sl);
    ui->listView->setModel(&slModel);

    ui->navigationSlider->setMaximum(header->fFrames);
    ui->videoProgressBar->setMaximum(header->fFrames);
    ui->videoProgressBar->setValue(0);
    ui->navigationSlider->setValue(0);
}
void MainWindow::progressChanged(int val)
{
    ui->videoProgressBar->setValue(val);
    ui->navigationSlider->setValue(val);
}
