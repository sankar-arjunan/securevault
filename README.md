# Secure Vault

A secure, end-to-end encrypted file manager with client-side encryption and OTP-based authentication. Built with **Node.js**, **MongoDB**, and **Qt C++ (macOS native UI)**.

## Features

- 🛡️ **Client-side encryption**: Files are encrypted before upload and decrypted only on the client.
- 🔑 **OTP-based login**: Email-based authentication without passwords.
- 🔒 **Zero knowledge**: Server never sees or stores encryption keys.
- 🧠 **In-memory previews**: Securely view decrypted text/images without writing to disk.
- ☁️ **Encrypted file storage**: Files are stored encrypted in MongoDB GridFS.

## Stack

- **Frontend**: Native Qt C++ macOS app
- **Backend**: Node.js + Express + MongoDB
- **Crypto**: AES encryption, SHA-256 hash-based key derivation

## How it works

1. User logs in with email and OTP.
2. Files are encrypted on the client using a password-derived key.
3. Encrypted files are uploaded to the server.
4. For viewing, files are decrypted in-memory (not saved to disk).
5. Server stores only the encrypted file blob and metadata.

## Security Notes

- **Encryption happens only on the client.**
- **Decryption key is never sent to the server.**
- **Previewed files are never saved to disk.**

## Run the Server

```bash
npm install
node server.js
