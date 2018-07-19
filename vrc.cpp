#include "vrc.h"
#include "QDebug"

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
    _numberOfBytesRead = _file->read((char*)_header, sizeof(VRCHeader));
    _fWidth = _header->fWidth;
    _fHeight = _header->fHeight;
    _fSize = _fWidth * _fHeight;
    if (_file->bytesAvailable() % _fSize)
    {
        qDebug() << "metadata";
    }
    _mat = new cv::Mat(_fHeight, _fWidth, CV_8UC1);
    _colMat = new cv::Mat(_fHeight, _fWidth, CV_8UC3);
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

    quint64 pos = _frameNumber * _fSize + 128;//_file->pos();
    _file->seek(pos);
    _numberOfBytesRead = _file->read((char*)_mat->data, _fSize);
    _frameHeader.b = (char*)_mat->data;

    output();

    emit newFrame(_frameNumber);
}
/////////////////////////////////////////////////////////
void VRCReader::rewind(int num)
{
    quint64 pos = num * _fSize + 128;
    _frameNumber = num;
    _file->seek(pos);
    _numberOfBytesRead = _file->read((char*)_mat->data, _fSize);
    _frameHeader.b = (char*)_mat->data;
    emit newFrame(num);
    output();

}
/////////////////////////////////////////////////////////
void VRCReader::output() {
    std::vector<cv::Mat> mats;
    mats.push_back(*_mat);
    mats.push_back(*_mat);
    mats.push_back(*_mat);
    mats.push_back(*_mat);
    cv::merge(mats, *_colMat);
    if (_putTextToImage) {
        int font = cv::FONT_HERSHEY_PLAIN;
        cv::Scalar color = cv::Scalar(255, 0, 255);
        //cv::putText(*_colMat, "fInterval   = " + QString::number(_header->fInterval).toStdString(), cv::Point(2, 15), font, 0.5, cv::Scalar(255, 255, 0));
        cv::putText(*_colMat,
                    "Azimuth  = " + QString::number(_frameHeader.h->az).toStdString(),
                    cv::Point(2, 15), font, 1, color);
        cv::putText(*_colMat,
                    "Elevation = " + QString::number(_frameHeader.h->el).toStdString(),
                    cv::Point(2, 30), font, 1, color);
        cv::putText(*_colMat,
                    "R  = " + QString::number(_frameHeader.h->r).toStdString(),
                    cv::Point(2, 45), font, 1, color);
        //cv::putText(*_colMat, "Elevation = " + QString::number(_frameHeader.h->el).toStdString(), cv::Point(2, 30), font, 1, color);
    }
    //cv::imshow("vrc", *_colMat);
    cv::imshow("vrc", *_colMat);
    //cv::imshow("vrc", *_colMat);

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

    cv::imwrite(fn.toStdString(), *_mat);
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
