#include <QApplication>
#include "mainwindow.h"
#include "vrc.h"
#include "QTimer"
#include <QDebug>
#include <QString>

void readerStart(QString);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    VRCReader reader;

    QObject::connect(&w, SIGNAL(takeFileName(const QString &, bool)),
                     &reader, SLOT(openFile(const QString &, bool)));
    QObject::connect(&w, SIGNAL(onPlayClick()),
                     &reader, SLOT(run()), Qt::DirectConnection);
    QObject::connect(&w, SIGNAL(onPauseClick()),
                     &reader, SLOT(pause()), Qt::DirectConnection);
    QObject::connect(&w, SIGNAL(destroyed()),
                     &reader, SLOT(pause()),
                     Qt::DirectConnection);
    QObject::connect(&w, SIGNAL(onSaveFrameClick()),
                     &reader, SLOT(saveFrame()), Qt::DirectConnection);
    QObject::connect(&w, SIGNAL(onBackClick()),
                     &reader, SLOT(back()), Qt::DirectConnection);
    QObject::connect(&w, SIGNAL(rewind(int)),
                     &reader, SLOT(rewind(int)), Qt::DirectConnection);
    QObject::connect(&w, SIGNAL(onCBOutputCLick(bool)),
                     &reader, SLOT(setTextToImFlag(bool)), Qt::DirectConnection);
    QObject::connect(&reader, SIGNAL(lastFrame()),
                     &w, SIGNAL(onPauseClick()), Qt::DirectConnection);
    QObject::connect(&reader, SIGNAL(metaData(VRCHeader*)),
                     &w, SLOT(metaData(VRCHeader*)), Qt::DirectConnection);
    QObject::connect(&reader, SIGNAL(newFrame(int)),
                     &w, SLOT(progressChanged(int)), Qt::DirectConnection);

    w.show();
    return a.exec();
}
