#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QHBoxLayout>
#include <QStringListModel>
#include <QListView>
#include <QTimer>
#include "vrc.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QString fileName() { return _fileName; }

private:
    Ui::MainWindow *ui;
    QString _fileName;
    QStringListModel slModel;

public slots:
    void onOpenClick();
    void changeOutputFlag();
    void metaData(VRCHeader*);
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
