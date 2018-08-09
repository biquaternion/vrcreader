#include "include/mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QListView>
#include <QSettings>
#include "include/vrcplayer.h"

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
    QSettings settings(QString("NIIPP"),
                       QString("vrcreader"));
    auto workDir = settings.value(QString("work directory"), QVariant(QString(""))).toString();
    _fileName = QFileDialog::getOpenFileName(this, "Choose File", workDir);
    if (!_fileName.isEmpty()){
        emit takeFileName(_fileName, ui->cbOutput->isChecked());
        workDir = QFileInfo(_fileName).dir().path();
        settings.setValue(QString("work directory"), workDir);
    }
}
void MainWindow::changeOutputFlag()
{

}
void MainWindow::metaData(const VRCHeader &header)
{
    QStringList sl;
    sl << "Version   \t" + QString::number(header.fVersion);
    sl << "Frames    \t" + QString::number(header.fFrames);
    sl << "Width     \t" + QString::number(header.fWidth);
    sl << "Height    \t" + QString::number(header.fHeight);
    sl << "Image Type\t" + QString::number(header.fImageType);
    sl << "Bits      \t" + QString::number(header.fBits);
    sl << "Ofs R     \t" + QString::number(header.fOfsR);
    sl << "Bits R    \t" + QString::number(header.fBitsR);
    sl << "Ofs G     \t" + QString::number(header.fOfsG);
    sl << "Bits G    \t" + QString::number(header.fBitsG);
    sl << "Ofs B     \t" + QString::number(header.fOfsB);
    sl << "Bits B    \t" + QString::number(header.fBitsB);
    sl << "Interval  \t" + QString::number(header.fInterval);
    sl << "Str Order \t" + QString::number(header.fStrOrder);

    slModel.setStringList(sl);
    ui->listView->setModel(&slModel);

    ui->navigationSlider->setMaximum(header.fFrames);
    ui->videoProgressBar->setMaximum(header.fFrames);
    ui->videoProgressBar->setValue(0);
    ui->navigationSlider->setValue(0);
}
void MainWindow::progressChanged(int val)
{
    ui->videoProgressBar->setValue(val);
    ui->navigationSlider->setValue(val);
}