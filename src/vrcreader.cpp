#include "include/vrcreader.h"
#include <QDebug>

/////////////////////////////////////////////////////////
VRCReader::VRCReader()
{}
/////////////////////////////////////////////////////////
VRCReader::~VRCReader()
{
    if (_file.isOpen())
        _file.close();
}
/////////////////////////////////////////////////////////
bool VRCReader::openFile(const QString &fn)
{
    if (_file.isOpen()) {
        _file.close();
    }
    qDebug() << fn;
    _file.setFileName(fn);
    if (!_file.open(QIODevice::ReadOnly))
    {
        qDebug() << "can't open file";
        return false;
    }
    _numberOfBytesRead = _file.read(reinterpret_cast<char*>(&_header), sizeof(VRCHeader));
    _fWidth = _header.fWidth;
    _fHeight = _header.fHeight;
    _fSize = _fWidth * _fHeight;
    _fFrames = _header.fFrames;
    if (_file.bytesAvailable() % _fSize)
    {
        qDebug() << "metadata";
    }
//    _image = QImage(static_cast<int>(_fWidth),
//                    static_cast<int>(_fHeight),
//                    QImage::Format_Indexed8);
    qDebug() << _header.fWidth << " x " << _header.fHeight;
    if (_file.isOpen()) {
        return true;
    } else {
        return false;
    }
}
/////////////////////////////////////////////////////////
bool VRCReader::readFrame(int64_t frameNum, char *data) {
    if (!_file.isOpen()) {
        return false;
    }
    qint64 pos = frameNum * _fSize + 128;//_file->pos();
    _file.seek(pos);
    _numberOfBytesRead = _file.read(data, _fSize);
    if (_numberOfBytesRead < _fSize) {
        return false;
    }
    _frameHeader.b = reinterpret_cast<char*>(data);
    return true;
}
/////////////////////////////////////////////////////////
int32_t VRCReader::frames() const {
    return _fFrames;
}
/////////////////////////////////////////////////////////
int32_t VRCReader::width() const {
    return _fWidth;
}
/////////////////////////////////////////////////////////
int32_t VRCReader::height() const {
    return _fHeight;
}
/////////////////////////////////////////////////////////
const FrameHeader &VRCReader::frameHeader() const
{
    return *_frameHeader.h;
}
/////////////////////////////////////////////////////////
