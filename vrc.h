// Clsss VRCReader is designed to open and read "*.vrc"-files
// It also display the current image, taken from file, using
// the OpenCV HighGUI interface, in the special window.

#ifndef VRC_H
#define VRC_H
#include "QFile"
#include <QTimer>
#include <QImage>
#include <QLabel>
#include <QObject>
#include <QString>
#include "QFileInfo"
#include <QTimerEvent>

class VRCReader;

struct VRCHeader
{
    qint8       fSign[4];
    qint32      fVersion;   // integer
    qint32      fFrames;
    qint32      fWidth;     // longint
    qint32      fHeight;
    qint32      fImageType; // integer
    qint32      fBits;
    qint32      fOfsR;
    qint32      fBitsR;
    qint32      fOfsG;
    qint32      fBitsG;
    qint32      fOfsB;
    qint32      fBitsB;
    qint32      fInterval;
    qint32      fStrOrder;
    qint32      fReserved[17];
};
#pragma pack(push,1)
struct FrameHeader
{
    double      jd;
    double      t;
    double      s;
    double      az;
    double      el;
    double      r;
};
#pragma pack(pop)
union uFrameHeader
{
    FrameHeader *h;
    char        *b;
};

class VRCReader : public QObject
{
    Q_OBJECT
public:
    VRCReader();
    ~VRCReader();
public slots:
    void timeout();
    void openFile(const QString &fn, bool putTxtToIm = false);
    void play();
    void run() {_dir = forward; play();}
    void back() {_dir = backward; play();}
    void rewind(int);
    void pause();
    void setTextToImFlag(bool);
    void saveFrame();
private:
    void timerEvent(QTimerEvent *event);
    enum Direction {
        backward  = 0,
        forward = 1
    };
    qint64      _numberOfBytesRead;
    VRCHeader   *_header;
    uFrameHeader _frameHeader;
    QFile       _file;
    QFileInfo   _fileInfo;

    int         _timerId {0};

    qint64      _fSize;
    qint64      _fWidth;
    qint64      _fHeight;
    qint64      _frameNumber;
    bool        _putTextToImage;

    QImage      _image;
    QLabel      _label;

    Direction   _dir;

    void        output();
signals:
    void lastFrame();
    void metaData(VRCHeader*);
    void newFrame(int);
};

#endif // VRC_H
