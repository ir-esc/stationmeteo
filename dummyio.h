#ifndef DUMMYIO_H
#define DUMMYIO_H

#include <QIODevice>

class DummyIO : public QIODevice
{
public:
   // DummyIO();
protected:
    qint64 readData(char *data, qint64 maxSize);
    qint64 writeData(const char *data, qint64 maxSize);
public:
    bool open(OpenMode mode);
    bool isSequential();
    qint64 bytesAvailable() const;

    void dataToRead(const char *data, qint64 size);
    void dataToRead(const QByteArray &data);

    quint64 dataWriten(char *data, qint64 maxSize);
    QByteArray dataWriten();

    OpenMode getMode() const;

private:
    QByteArray rxData;
    QByteArray txData;
    OpenMode mode;

};
#endif // DUMMYIO_H
