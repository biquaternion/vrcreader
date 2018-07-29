#include <QApplication>
#include "mainwindow.h"
#include "vrcplayer.h"
#include "QTimer"
#include <QDebug>
#include <QString>

void readerStart(QString);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    VRCPlayer player;

    QObject::connect(&w, SIGNAL(takeFileName(const QString &, bool)),
                     &player, SLOT(openFile(const QString &, bool)));
    QObject::connect(&w, SIGNAL(onPlayClick()),
                     &player, SLOT(fwd()));
    QObject::connect(&w, SIGNAL(onPauseClick()),
                     &player, SLOT(pause()));
    QObject::connect(&w, SIGNAL(destroyed()),
                     &player, SLOT(pause()));
    QObject::connect(&w, SIGNAL(onSaveFrameClick()),
                     &player, SLOT(saveFrame()));
    QObject::connect(&w, SIGNAL(onBackClick()),
                     &player, SLOT(back()));
    QObject::connect(&w, SIGNAL(rewind(int)),
                     &player, SLOT(rewind(int)));
    QObject::connect(&w, SIGNAL(onCBOutputCLick(bool)),
                     &player, SLOT(setTextToImFlag(bool)));
    QObject::connect(&player, SIGNAL(lastFrame()),
                     &w, SIGNAL(onPauseClick()));
    QObject::connect(&player, SIGNAL(metaData(const VRCHeader&)),
                     &w, SLOT(metaData(const VRCHeader&)));
    QObject::connect(&player, SIGNAL(newFrame(int)),
                     &w, SLOT(progressChanged(int)));

    w.show();
    return a.exec();
}
