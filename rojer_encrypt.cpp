#include "rojer_encrypt.h"
#include <QByteArray>
#include <openssl/evp.h>

bool aesOperation(const QByteArray &input, QByteArray &output, const QByteArray &key, bool isEncrypt) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return false;

    const EVP_CIPHER *cipher = EVP_aes_256_cbc();
    unsigned char iv[EVP_MAX_IV_LENGTH] = {};

    if (!EVP_CipherInit_ex(ctx, cipher, nullptr, reinterpret_cast<const unsigned char *>(key.constData()), iv, isEncrypt)) {
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    output.resize(input.size() + EVP_CIPHER_block_size(cipher));
    int len = 0;

    if (!EVP_CipherUpdate(ctx, reinterpret_cast<unsigned char *>(output.data()), &len, reinterpret_cast<const unsigned char *>(input.constData()), input.size())) {
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    int final_len = 0;
    if (!EVP_CipherFinal_ex(ctx, reinterpret_cast<unsigned char *>(output.data()) + len, &final_len)) {
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    output.resize(len + final_len);
    EVP_CIPHER_CTX_free(ctx);
    return true;
}

bool encrypt(const QByteArray &inputData, QByteArray &outputData, const QByteArray &key) {
    return aesOperation(inputData, outputData, key, true);
}

bool decrypt(const QByteArray &encryptedData, QByteArray &decryptedData, const QByteArray &key) {
    return aesOperation(encryptedData, decryptedData, key, false);
}
