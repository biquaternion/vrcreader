#include "include/vrcplayer.h"
#include "QDebug"
#include <QPixmap>
#include <QPainter>
#include <QTextItem>
#include <include/vrcplayer.h>
#include <string>
#include <iostream>
#include <iomanip>


///////////////////////////////////////////////////////////
struct FrameHeader
{
    double      jd {0.0};
    double      t  {0.0};
    double      s  {0.0};
    double      az {0.0};
    double      el {0.0};
    double      r  {0.0};
};
/////////////////////////////////////////////////////////
inline QDebug operator << (QDebug debug, const FrameHeader & header) {
    return debug << QString("az: %1;    el: %2").arg(header.az).arg(header.el);
}
/////////////////////////////////////////////////////////
VRCPlayer::VRCPlayer() :
    _label(std::make_shared<QLabel>(nullptr, Qt::Window)) {}
/////////////////////////////////////////////////////////
VRCPlayer::VRCPlayer(std::shared_ptr<QLabel> label) :
    _label(label) {}
/////////////////////////////////////////////////////////
void VRCPlayer::openFile(const QString &fileName, bool putTextToImage)
{
    if (_reader.openFile(fileName)) {
        _putTextToImage = putTextToImage;
        _fileInfo.setFile(fileName);
        _image = QImage(static_cast<int>(_reader.width()),
                        static_cast<int>(_reader.height()),
                        QImage::Format_Indexed8);
        _frameNumber = 0;
        emit metaData(_reader._header);
    }
}
/////////////////////////////////////////////////////////
void VRCPlayer::play()
{
    if (_timerId == 0) {
        _timerId = startTimer(40);
    }
    qDebug() << this->thread();
}
/////////////////////////////////////////////////////////
void VRCPlayer::pause()
{
    if (_timerId != 0){
        killTimer(_timerId);
        _timerId = 0;
    }
}
/////////////////////////////////////////////////////////
void VRCPlayer::fwd()
{
    _dir = forward;
    play();
}
/////////////////////////////////////////////////////////
void VRCPlayer::back()
{
    _dir = backward;
    play();
}
/////////////////////////////////////////////////////////
void VRCPlayer::nextFrame()
{
    assert(_frameNumber >= 0);
    if(_frameNumber < _reader.frames()) {
        _frameNumber++;
    } else {
        _frameNumber = _reader.frames();
    }
    rewind(static_cast<int>(_frameNumber));
}
/////////////////////////////////////////////////////////
void VRCPlayer::prevFrame()
{
    if (_frameNumber > 0) {
        _frameNumber--;
    } else {
        _frameNumber = 0;
    }
    rewind(static_cast<int>(_frameNumber));
}
/////////////////////////////////////////////////////////
void VRCPlayer::rewind(int frameNum)
{
    if (_reader.readFrame(frameNum, reinterpret_cast<char*>(_image.bits()))) {
        output();
        emit newFrame(static_cast<int>(frameNum));
        _frameNumber = frameNum;
    }
//    output();
//    emit newFrame(static_cast<int>(frameNum));
}
/////////////////////////////////////////////////////////
void VRCPlayer::timerEvent(QTimerEvent *event)
{
    auto timerId = event->timerId();
    if (timerId == _timerId){
        timeout();
    } else {
        killTimer(timerId);
//        _timerId = 0;
    }
}
/////////////////////////////////////////////////////////
void VRCPlayer::timeout()
{
    if (_dir == forward) {
        _frameNumber++;
    }
    else {
        _frameNumber--;
    }
    if (_frameNumber >= _reader.frames()) {
        if (_dir == forward) {
            _frameNumber = 0;
        }
        emit lastFrame();
    }
    if (_frameNumber <= 0) {
        if (_dir == backward) {
            _frameNumber = _reader.frames();
        }
        emit lastFrame();
    }
    rewind(static_cast<int>(_frameNumber));
}
/////////////////////////////////////////////////////////
void VRCPlayer::output() {
    QImage image = _image.convertToFormat(QImage::Format_RGB32);
    if (_putTextToImage) {
        QPainter painter(&image);
        FrameHeader frameHeader;
        _reader.extractCustomMetaData(frameHeader);
        painter.setPen(Qt::yellow);
        painter.drawText(QRect(5, 5, 200, 150), 0, QString("fInterval\t=%1\n"
                                               "Azimuth\t=%2\n"
                                               "Elevation\t=%3\n"
                                               "R\t\t=%4")
                         .arg(_reader._header.fInterval)
                         .arg(frameHeader.az)
                         .arg(frameHeader.el)
                         .arg(frameHeader.r));
        painter.end();
        //qDebug() << frameHeader;
    }
    _label->setPixmap(QPixmap::fromImage(image));
    _label->show();
}
/////////////////////////////////////////////////////////
void VRCPlayer::saveFrame()
{
    QString fp = _fileInfo.absolutePath();
    QString fn = fp + "/" +
            _fileInfo.baseName() +
            "_frame#" + QString::number(_frameNumber) +
            ".bmp";
    QImage imageToSave = _image.convertToFormat(QImage::Format_ARGB32);
    imageToSave.save(fn);
    qDebug() << fn;
}
/////////////////////////////////////////////////////////
void VRCPlayer::setTextToImFlag(bool fl)
{
    _putTextToImage = fl;
}
