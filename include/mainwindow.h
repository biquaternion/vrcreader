#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QStringListModel>
#include <QMainWindow>
#include <QPushButton>
#include <QHBoxLayout>
#include <QListView>
#include <QAction>
#include <QTimer>
#include "vrcplayer.h"
#include <memory>

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
    std::shared_ptr<QLabel> imageWidget() const;

private:
    Ui::MainWindow *ui;
    QString _fileName;
    QStringListModel slModel;
    std::unique_ptr<QAction> _open;
    std::shared_ptr<QAction> _play;
    std::shared_ptr<QAction> _next;
    std::shared_ptr<QAction> _prev;
    bool _playbackState {false};
    std::shared_ptr<QLabel> _label;

    void init();
    void initHotkeys();
    void initConnections();

public slots:
    void onOpenClick();
    void changeOutputFlag();
    void metaData(const VRCHeader &header);
    void progressChanged(int val);
signals:
    void onPlayClick();
    void onPauseClick();
    void onNextFrameClick();
    void onPrevFrameClick();
    void onSaveFrameClick();
    void onCBOutputCLick(bool);
    void takeFileName(const QString &, bool);
    void lastFrame();
    void rewind(int);
};

#endif // MAINWINDOW_H
