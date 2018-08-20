#ifndef VRCREADER_H
#define VRCREADER_H

#include <QFile>

class VRCPlayer;

struct VRCHeader
{
    int8_t       fSign[4];
    int32_t      fVersion;   // integer
    int32_t      fFrames;
    int32_t      fWidth;     // longint
    int32_t      fHeight;
    int32_t      fImageType; // integer
    int32_t      fBits;
    int32_t      fOfsR;
    int32_t      fBitsR;
    int32_t      fOfsG;
    int32_t      fBitsG;
    int32_t      fOfsB;
    int32_t      fBitsB;
    int32_t      fInterval;
    int32_t      fStrOrder;
    int32_t      fReserved[17];
};

class VRCReader
{
    friend class VRCPlayer;
public:
    VRCReader() = default;
    ~VRCReader();
    bool openFile(const QString &fn);
    bool readFrame(int64_t frameNum = 0, char *data = nullptr);
    int32_t frames() const;
    int32_t width() const;
    int32_t height() const;
    const uint8_t *data() const;

    template <typename T>
    void extractCustomMetaData(T &metaData) {
        if (_data == nullptr) {
            // log "no data in frame header"
            return;
        }
        if (sizeof(T) > _fSize) {
            // log "requested too big piece of data"
            return;
        }
        memcpy(&metaData, _data, sizeof(metaData));
    }

private:
    int64_t      _numberOfBytesRead;
    VRCHeader    _header;
    uint8_t     *_data;
    QFile        _file;

    int32_t      _fSize;
    int32_t      _fWidth {0};
    int32_t      _fHeight {0};
    int32_t      _fFrames {0};

};

#endif // VRCREADER_H
