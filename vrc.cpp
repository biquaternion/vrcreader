#include "vrc.h"
#include "QDebug"
#include <QPixmap>
#include <QPainter>
#include <QTextItem>

VRCReader::VRCReader() :
    _header(new VRCHeader),
    _frameNumber(0),
    _dir(forward)
{
}
/////////////////////////////////////////////////////////
VRCReader::~VRCReader()
{
    if (_file.isOpen())
        _file.close();
    delete _header;
}
/////////////////////////////////////////////////////////
void VRCReader::openFile(const QString &fn, bool putTxtToIm)
{
    _putTextToImage = putTxtToIm;
    if (_file.isOpen()) {
        _file.close();
    }
    qDebug() << fn;
    _file.setFileName(fn);
    _fileInfo.setFile(_file);
    if (!_file.open(QIODevice::ReadOnly))
    {
        qDebug() << "can't open file";
        return;
    }
    _numberOfBytesRead = _file.read(reinterpret_cast<char*>(_header), sizeof(VRCHeader));
    _fWidth = _header->fWidth;
    _fHeight = _header->fHeight;
    _fSize = _fWidth * _fHeight;
    if (_file.bytesAvailable() % _fSize)
    {
        qDebug() << "metadata";
    }
    _image = QImage(static_cast<int>(_fWidth),
                    static_cast<int>(_fHeight),
                    QImage::Format_Indexed8);
    emit metaData(_header);
    qDebug() << _header->fWidth << " x " << _header->fHeight;
    if (_file.isOpen()) {
        _timerId = this->startTimer(40);
    }
}
/////////////////////////////////////////////////////////
void VRCReader::timeout()
{
    if (_dir) {
        _frameNumber++;
    }
    else {
        _frameNumber--;
    }
    if (_frameNumber >= _header->fFrames) {
        _frameNumber = 0;
        emit lastFrame();
    }
    if (_frameNumber <= 0) {
        _frameNumber = _header->fFrames;
        emit lastFrame();
    }

    qint64 pos = _frameNumber * _fSize + 128;//_file->pos();
    _file.seek(pos);
    _numberOfBytesRead = _file.read(reinterpret_cast<char*>(_image.bits()), _fSize);
    _frameHeader.b = reinterpret_cast<char*>(_image.bits());

    output();

    emit newFrame(static_cast<int>(_frameNumber));
}
/////////////////////////////////////////////////////////
void VRCReader::rewind(int num)
{
    qint64 pos = num * _fSize + 128;
    _frameNumber = num;
    _file.seek(pos);
    _numberOfBytesRead = _file.read(reinterpret_cast<char*>(_image.bits()), _fSize);
    _frameHeader.b = reinterpret_cast<char*>(_image.bits());
    emit newFrame(num);
    output();

}
/////////////////////////////////////////////////////////
void VRCReader::output() {
    QImage image = _image.convertToFormat(QImage::Format_RGB32);
    _label.setPixmap(QPixmap::fromImage(image));
    if (_putTextToImage) {
        // todo: not drawn. fix it
        QPainter painter(&image);
        painter.drawText(QRect(5, 5, 200, 50), 0, QString("fInterval\t=%1\n"
                                               "Azimuth\t=%2\n"
                                               "Elevation\t=%3\n"
                                               "R\t\t=%4")
                         .arg(_header->fInterval)
                         .arg(_frameHeader.h->az)
                         .arg(_frameHeader.h->el)
                         .arg(_frameHeader.h->r));
        painter.end();
    }
    _label.show();
}
/////////////////////////////////////////////////////////
void VRCReader::saveFrame()
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
void VRCReader::timerEvent(QTimerEvent *event)
{
    auto timerId = event->timerId();
    if (timerId == _timerId){
        timeout();
    } else {
        killTimer(timerId);
    }
}
/////////////////////////////////////////////////////////
void VRCReader::play()
{
    if (_timerId == 0) {
        _timerId = startTimer(40);
    }
    qDebug() << this->thread();
}
/////////////////////////////////////////////////////////
void VRCReader::pause()
{
    if (_timerId != 0){
        killTimer(_timerId);
        _timerId = 0;
    }
}
/////////////////////////////////////////////////////////
void VRCReader::setTextToImFlag(bool fl)
{
    _putTextToImage = fl;
}
