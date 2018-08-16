// Clsss VRCReader is designed to open and read "*.vrc"-files
// It also display the current image, taken from file, using
// the OpenCV HighGUI interface, in the special window.

#ifndef VRC_H
#define VRC_H
#include <QFile>
#include <QTimer>
#include <QImage>
#include <QLabel>
#include <QObject>
#include <QString>
#include <QFileInfo>
#include <QTimerEvent>
#include <memory>
#include "vrcreader.h"


class VRCPlayer : public QObject
{
    Q_OBJECT
public:
    VRCPlayer();
    explicit VRCPlayer(std::shared_ptr<QLabel> label);
    ~VRCPlayer() = default;

    template <typename T>
    void getMetaData(T &metaData) {
        _reader.extractCustomMetaData(metaData);
    }
public slots:
    void play();
    void pause();
    void fwd();
    void back();
    void nextFrame();
    void prevFrame();
    void rewind(int frameNum);
    void timeout();
    void openFile(const QString &fileName, bool putTextToImage);
    void output();
    void setTextToImFlag(bool);
    void saveFrame();
private:
    void timerEvent(QTimerEvent *event) override;
    enum Direction {
        backward  = 0,
        forward = 1
    };
    VRCReader   _reader;

    int         _timerId {0};
    int64_t     _frameNumber {0};
    QImage      _image;
    QFileInfo   _fileInfo;
    bool        _putTextToImage;
    std::shared_ptr<QLabel> _label;

    Direction   _dir {forward};
signals:
    void lastFrame();
    void metaData(const VRCHeader&);
    void newFrame(int);
};

#endif // VRC_H
