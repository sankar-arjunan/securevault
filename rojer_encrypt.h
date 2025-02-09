#ifndef ROJER_ENCRYPT_H
#define ROJER_ENCRYPT_H

#include <QByteArray>

bool encrypt(const QByteArray &inputData, QByteArray &outputData, const QByteArray &key);

bool decrypt(const QByteArray &encryptedData, QByteArray &decryptedData, const QByteArray &key);

#endif // ROJER_ENCRYPT_H
