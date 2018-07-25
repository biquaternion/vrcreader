#include <QApplication>
#include "mainwindow.h"
#include "vrc.h"
#include "QTimer"
#include <QDebug>
#include <QString>

//#include "QObject"
void readerStart(QString);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    auto reader = std::make_unique<VRCReader>();

    QObject::connect(&w, SIGNAL(takeFileName(QString*, bool)),
                     reader.get(), SLOT(openFile(QString*, bool)), Qt::DirectConnection);
    QObject::connect(&w, SIGNAL(onPlayClick()),
                     reader.get(), SLOT(run()), Qt::DirectConnection);
    QObject::connect(&w, SIGNAL(onPauseClick()),
                     reader.get(), SLOT(pause()), Qt::DirectConnection);
    QObject::connect(&w, SIGNAL(destroyed()),
                     reader.get(), SLOT(pause()),
                     Qt::DirectConnection);
    QObject::connect(&w, SIGNAL(onSaveFrameClick()),
                     reader.get(), SLOT(saveFrame()), Qt::DirectConnection);
    QObject::connect(&w, SIGNAL(onBackClick()),
                     reader.get(), SLOT(back()), Qt::DirectConnection);
    QObject::connect(&w, SIGNAL(rewind(int)),
                     reader.get(), SLOT(rewind(int)), Qt::DirectConnection);
    QObject::connect(&w, SIGNAL(onCBOutputCLick(bool)),
                     reader.get(), SLOT(setTextToImFlag(bool)), Qt::DirectConnection);
    QObject::connect(reader.get(), SIGNAL(lastFrame()),
                     &w, SIGNAL(onPauseClick()), Qt::DirectConnection);
    QObject::connect(reader.get(), SIGNAL(metaData(VRCHeader*)),
                     &w, SLOT(metaData(VRCHeader*)), Qt::DirectConnection);
    QObject::connect(reader.get(), SIGNAL(newFrame(int)),
                     &w, SLOT(progressChanged(int)), Qt::DirectConnection);

    return a.exec();
}
