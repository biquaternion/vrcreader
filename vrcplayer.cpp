#include "vrcplayer.h"
#include "QDebug"
#include <QPixmap>
#include <QPainter>
#include <QTextItem>


/////////////////////////////////////////////////////////
VRCPlayer::VRCPlayer()
{}
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
        play();
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
        if (_dir == forward){
            _frameNumber = 0;
        }
        emit lastFrame();
    }
    if (_frameNumber <= 0) {
        if (_dir == backward){
            _frameNumber = _reader.frames();
        }
        emit lastFrame();
    }
    rewind(static_cast<int>(_frameNumber));
}
/////////////////////////////////////////////////////////
void VRCPlayer::output() {
    QImage image = _image.convertToFormat(QImage::Format_RGB32);
    _label.setPixmap(QPixmap::fromImage(image));
    if (_putTextToImage) {
        // todo: not drawn. fix it
        QPainter painter(&image);
        painter.drawText(QRect(5, 5, 200, 50), 0, QString("fInterval\t=%1\n"
                                               "Azimuth\t=%2\n"
                                               "Elevation\t=%3\n"
                                               "R\t\t=%4")
                         .arg(_reader._header.fInterval)
                         .arg(_reader.frameHeader().az)
                         .arg(_reader.frameHeader().el)
                         .arg(_reader.frameHeader().r));
        painter.end();
    }
    _label.show();
}
/////////////////////////////////////////////////////////
void VRCPlayer::saveFrame()
{
    QString fp = _fileInfo.absolutePath();
    QString fn = fp + "/" +
            _fileInfo.baseName() +
            "_frame#" + QString::number(_frameNumber) +
            ".bmp";
    // todo: doesn't work. Learn why and fix.
    _image.save(fn);
    qDebug() << fn;
}
/////////////////////////////////////////////////////////
void VRCPlayer::setTextToImFlag(bool fl)
{
    _putTextToImage = fl;
}
