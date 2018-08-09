#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QStringListModel>
#include <QMainWindow>
#include <QPushButton>
#include <QHBoxLayout>
#include <QListView>
#include <QTimer>
#include "vrcplayer.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString fileName() { return _fileName; }

private:
    Ui::MainWindow *ui;
    QString _fileName;
    QStringListModel slModel;

public slots:
    void onOpenClick();
    void changeOutputFlag();
    void metaData(const VRCHeader &header);
    void progressChanged(int val);
signals:
    void onPlayClick();
    void onPauseClick();
    void onBackClick();
    void onSaveFrameClick();
    void onCBOutputCLick(bool);
    void takeFileName(const QString &, bool);
    void lastFrame();
    void rewind(int);
};

#endif // MAINWINDOW_H
