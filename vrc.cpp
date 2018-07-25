#include "vrc.h"
#include "QDebug"
#include <QPixmap>
#include <QPainter>
#include <QTextItem>

VRCReader::VRCReader() :
    _header(new VRCHeader),
    _isFrameHeader(false),
    _file(new QFile(this)),
    _frameNumber(0),
    _dir(forward)
{
    this->moveToThread(this);
    _timer = new QTimer();
    QObject::connect(_timer, SIGNAL(timeout()),
                     this, SLOT(timeout()), Qt::DirectConnection);
}
/////////////////////////////////////////////////////////
VRCReader::VRCReader(QString s, bool putTxtToIm) :
    _header(new VRCHeader),
    _isFrameHeader(false),
    _frameNumber(0),
    _putTextToImage(putTxtToIm),
    _dir(forward)
{
    this->moveToThread(this);
    _timer = new QTimer();
    openFile(s);
    QObject::connect(_timer, SIGNAL(timeout()),
                     this, SLOT(timeout()), Qt::DirectConnection);
}
/////////////////////////////////////////////////////////
VRCReader::~VRCReader()
{
    if (_file->isOpen())
        _file->close();
    delete _file;
    delete _header;
}
/////////////////////////////////////////////////////////
void VRCReader::openFile(QString fn)
{
    if (_file->isOpen()) {
        _file->close();
    }
    qDebug() << fn;
    _file->setFileName(fn);
    _fileInfo = new QFileInfo(*_file);
    if (!_file->open(QIODevice::ReadOnly))
    {
        qDebug() << "can't open file";
        return;
    }
    _timer->setInterval(10);
    _numberOfBytesRead = _file->read(reinterpret_cast<char*>(_header), sizeof(VRCHeader));
    _fWidth = _header->fWidth;
    _fHeight = _header->fHeight;
    _fSize = _fWidth * _fHeight;
    if (_file->bytesAvailable() % _fSize)
    {
        qDebug() << "metadata";
    }
    _image = QImage(static_cast<int>(_fWidth),
                    static_cast<int>(_fHeight),
                    QImage::Format_Indexed8);
    emit metaData(_header);
    qDebug() << _header->fWidth << " x " << _header->fHeight;
}
/////////////////////////////////////////////////////////
void VRCReader::openFile(QString *fn, bool putTxtToIm)
{
    _putTextToImage = putTxtToIm;
    openFile(*fn);
    if (_file->isOpen())
        _timer->start();
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
    _file->seek(pos);
    _numberOfBytesRead = _file->read(reinterpret_cast<char*>(_image.bits()), _fSize);
    _frameHeader.b = reinterpret_cast<char*>(_image.bits());

    output();

    emit newFrame(static_cast<int>(_frameNumber));
}
/////////////////////////////////////////////////////////
void VRCReader::rewind(int num)
{
    qint64 pos = num * _fSize + 128;
    _frameNumber = num;
    _file->seek(pos);
    _numberOfBytesRead = _file->read(reinterpret_cast<char*>(_image.bits()), _fSize);
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
    //QString fn = _file->fileName() + "_frame_" + _frameNumber;
    QString fp = _fileInfo->absolutePath();
    QString fn = fp + "/" +
            _fileInfo->baseName() +
            "_frame#" + QString::number(_frameNumber) +
            ".bmp";
    // todo: doesn't work. Learn why and fix.
    _image.save(fn);
    qDebug() << fn;
}
/////////////////////////////////////////////////////////
void VRCReader::play()
{
    _timer->start();
    qDebug() << this->thread();
    qDebug() << _timer->thread();
}
/////////////////////////////////////////////////////////
void VRCReader::pause()
{
    _timer->stop();
}
/////////////////////////////////////////////////////////
void VRCReader::setTextToImFlag(bool fl)
{
    _putTextToImage = fl;
}
