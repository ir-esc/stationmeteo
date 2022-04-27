#include "dummyio.h"
#include <iostream>
#include <QIODevice>
#include <assert.h>

qint64 DummyIO::readData(char *data, qint64 maxSize) {
    qint64 size = this->rxData.length();
    if (size>maxSize) size=maxSize;
    memcpy(data,this->rxData.constData(),size);
    rxData.remove(0,size);
    return size;
}

qint64 DummyIO::writeData(const char *data, qint64 maxSize) {
    this->txData.append(data,maxSize);
    return maxSize;
}

bool DummyIO::open(OpenMode mode) {
    rxData.remove(0,rxData.length());
    this->mode = mode;
    return QIODevice::open(mode);
}

bool DummyIO::isSequential() {
    return true;
}

void DummyIO::dataToRead(const char *data, qint64 size) {
    this->rxData.append(data,size);
    emit readyRead();
}

void DummyIO::dataToRead(const QByteArray &data)
{
    dataToRead(data.constData(),data.length());
}

qint64 DummyIO::bytesAvailable() const {
    return this->rxData.length();
}


quint64 DummyIO::dataWriten(char *data, qint64 maxSize) {
    qint64 size = this->txData.length();
    if (size>maxSize) size=maxSize;
    memcpy(data,this->txData.constData(),size);
    txData.remove(0,size);
    return size;
}

QByteArray DummyIO::dataWriten()
{
    QByteArray data = txData;
    txData.clear();
    return data;
}

QIODevice::OpenMode DummyIO::getMode() const
{
    return mode;
}




