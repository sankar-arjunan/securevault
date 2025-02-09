#include <QFile>
#include <QByteArray>
#include <QCryptographicHash>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QFileDialog>
#include <QFileInfo>
#include <QNetworkReply>
#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkAccessManager>
#include <QAction>
#include <QInputDialog>
#include <QMenu>
#include <QCloseEvent>

#include "sv_main.h"
#include "rojer_encrypt.h"

void MainApp::closeEvent(QCloseEvent *event) {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Exit Confirmation",
                                  "Are you sure you want to exit?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // Send logout request
        QNetworkRequest request(QUrl("http://localhost:3000/logout"));
        request.setRawHeader("Authorization", "Bearer " + authToken.toUtf8());

        QNetworkReply *reply = networkManager.post(request, QByteArray());
        connect(reply, &QNetworkReply::finished, [this, reply, event]() {
            if (reply->error() == QNetworkReply::NoError) {
                qDebug() << "Logged out successfully";
            } else {
                qDebug() << "Logout failed:" << reply->errorString();
            }
            reply->deleteLater();
            event->accept();
        });
    } else {
        event->ignore();
    }
}

QString truncateFileName(const QString &fileName) {
    int dotIndex = fileName.lastIndexOf('.');
    if (dotIndex == -1 || dotIndex == 0) return fileName.left(10); // No extension or hidden file

    QString baseName = fileName.left(dotIndex);
    QString extension = fileName.mid(dotIndex);

    if (baseName.length() > 5) {
        return baseName.left(5) + "..." + extension;
    }
    return fileName;
}


FileItem MainApp::getFileData(const QString &fileId) {
    FileItem file;
    file.fileid = fileId;

    QNetworkRequest request(QUrl("http://localhost:3000/info?fileId=" + fileId));
    request.setRawHeader("Authorization", ("Bearer " + authToken).toUtf8());
    QNetworkReply *reply = networkManager.get(request);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QNetworkReply::NoError) {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        file.name = jsonObject["name"].toString();
        file.size = QString::number(jsonObject["size"].toInt()) + " bytes";
        file.type = jsonObject["type"].toString();
    } else {
        qDebug() << "Failed to fetch file data: " << reply->errorString();
        file.name = "Unknown";
        file.size = "0 bytes";
        file.type = "OTHER";
    }

    reply->deleteLater();
    return file;
}


MainApp::MainApp(QWidget *parent) : QMainWindow(parent) {
    loginUi.setupUi(&Dialog);
    ui.setupUi(this);

    connect(loginUi.loginButton, &QPushButton::clicked, this, &MainApp::onLoginClicked);
    connect(loginUi.clearButton, &QPushButton::clicked, this, &MainApp::onClear);
    connect(loginUi.otpButton, &QPushButton::clicked, this, &MainApp::onSendOTPClicked);
    connect(ui.searchBar, &QLineEdit::returnPressed, this, &MainApp::onSearch);
    connect(ui.refreshButton, &QToolButton::clicked, this, &MainApp::onRefresh);
    connect(ui.uploadButton, &QToolButton::clicked, this, &MainApp::onUpload);

    ui.filesListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui.filesListWidget, &QListWidget::customContextMenuRequested, this, &MainApp::showContextMenu);

    Dialog.exec();
}

void MainApp::fetchFileList(const QString &query) {
    ui.filesListWidget->clear();

    if (authToken.isEmpty()) {
        QMessageBox::warning(this, "Authentication Error", "You must be logged in.");
        return;
    }

    QUrl apiUrl("http://localhost:3000/list");
    if (!query.isEmpty()) {
        apiUrl.setQuery("q=" + QUrl::toPercentEncoding(query));
    }

    QNetworkRequest request(apiUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", ("Bearer " + authToken).toUtf8());

    QNetworkReply *reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(response);
            if (!jsonResponse.isArray()) {
                QMessageBox::warning(this, "Error", "Unexpected response format.");
                reply->deleteLater();
                return;
            }

            QJsonArray fileArray = jsonResponse.array();
            for (const QJsonValue &value : fileArray) {
                QJsonObject fileObject = value.toObject();
                QString fileId = fileObject["_id"].toString();
                QString name = truncateFileName(fileObject["name"].toString());
                QString size = QString::number(fileObject["size"].toInt()) + " bytes";
                QString type = fileObject["type"].toString();

                QString iconPath = (type == "IMAGE") ? ":/image.png" :
                                   (type == "TEXT") ? ":/text.png" :":/other.png";

                QIcon fileIcon(iconPath);
                QListWidgetItem *item = new QListWidgetItem(fileIcon, name, ui.filesListWidget);
                item->setData(Qt::UserRole, fileId);
                item->setToolTip(QString("Size: %1\nType: %2").arg(size, type));
            }
        } else {
            QMessageBox::warning(this, "Error", "Failed to load files.\n" + reply->errorString());
        }
        reply->deleteLater();
    });
}

void MainApp::handleFileUpload(const QString &filePath) {
    QFile inputFile(filePath);
    if (!inputFile.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(nullptr, "File Error", "Could not open file for reading.");
        return;
    }
    QByteArray fileData = inputFile.readAll();
    QByteArray key = QCryptographicHash::hash(globalPassword.toUtf8(), QCryptographicHash::Sha256);

    QByteArray encryptedData;
    if (!encrypt(fileData, encryptedData, key)) {
        QMessageBox::warning(nullptr, "Encryption Error", "File encryption failed.");
        return;
    }

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart filePart;
    filePart.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader, 
                       "form-data; name=\"file\"; filename=\"" + QFileInfo(filePath).fileName() + "\"");
    filePart.setBody(encryptedData);
    multiPart->append(filePart);

    QNetworkRequest request(QUrl("http://localhost:3000/upload"));
    request.setRawHeader("Authorization", ("Bearer " + authToken).toUtf8());

    QNetworkReply *reply = networkManager.post(request, multiPart);
    multiPart->setParent(reply);

    QObject::connect(reply, &QNetworkReply::finished, [=]() {
        reply->error() == QNetworkReply::NoError
            ? QMessageBox::information(nullptr, "Upload Successful", "File uploaded successfully.")
            : QMessageBox::warning(nullptr, "Upload Failed", "Error uploading file.");
        reply->deleteLater();
    });
}


void MainApp::onRefresh() {
    ui.searchBar->clear();
    fetchFileList("");
}

void MainApp::onSearch() {
    QString query = ui.searchBar->text().trimmed();
    fetchFileList(query);
}

void MainApp::onSendOTPClicked() {
    QString email = loginUi.emailText->text();
    if (email.isEmpty()) {
        QMessageBox::warning(&Dialog, "Error", "Enter an email first.");
        return;
    }

    loginUi.otpButton->setEnabled(false);
    loginUi.otpButton->setText("Sending...");

    QUrl url("http://localhost:3000/sendotp");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["email"] = email;

    QNetworkReply *reply = networkManager.post(request, QJsonDocument(json).toJson());
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() != QNetworkReply::NoError) {
            QMessageBox::critical(&Dialog, "Error", "OTP send failed: " + reply->errorString());
        }

        loginUi.otpButton->setText("Sent");

        reply->deleteLater();
    });

    loginUi.emailText->setReadOnly(true);
}

void MainApp::onLoginClicked() {
    QString email = loginUi.emailText->text();
    QString otp = loginUi.OTPText->text();
    globalPassword = loginUi.keyText->text();

    if (email.isEmpty() || otp.isEmpty() || globalPassword.isEmpty()) {
        QMessageBox::warning(&Dialog, "Error", "All fields are required.");
        return;
    }

    QUrl url("http://localhost:3000/login");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["email"] = email;
    json["otp"] = otp;

    QNetworkReply *reply = networkManager.post(request, QJsonDocument(json).toJson());
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonObject response = QJsonDocument::fromJson(reply->readAll()).object();
            authToken = response["token"].toString();
            Dialog.close();
            show();
            onRefresh();
        } else {
            QMessageBox::warning(&Dialog, "Login Failed", "Invalid credentials.");
        }
        reply->deleteLater();
    });
}

void MainApp::onClear() {
    loginUi.emailText->clear();
    loginUi.OTPText->clear();
    loginUi.keyText->clear();
    loginUi.otpButton->setText("Send OTP");
    loginUi.emailText->setReadOnly(false);
    loginUi.otpButton->setEnabled(true);
}

void MainApp::onUpload() {
    QString filePath = QFileDialog::getOpenFileName(this, "Select a file to upload", "", "All Files (*)");

    if (!filePath.isEmpty()) {
        QString encryptedPath = filePath;
        QByteArray key = QCryptographicHash::hash(globalPassword.toUtf8(), QCryptographicHash::Sha256);
        handleFileUpload(encryptedPath);
        onRefresh();
    } else {
        QMessageBox::warning(this, "No File Selected", "No file was selected.");
    }
}

void MainApp::previewFile(const QString &fileId, const FileItem &file) {
    QNetworkRequest request(QUrl("http://localhost:3000/download/" + fileId));
    request.setRawHeader("Authorization", ("Bearer " + authToken).toUtf8());

    QNetworkReply *reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray encryptedData = reply->readAll();
            QByteArray decryptedData;
            QByteArray key = QCryptographicHash::hash(globalPassword.toUtf8(), QCryptographicHash::Sha256);

            if (decrypt(encryptedData, decryptedData, key)) {
                if (file.type == "TEXT") {
                    QMainWindow *textWindow = new QMainWindow(this);
                    textWindow->setWindowTitle(file.name);
                    textWindow->resize(600, 400);
                    textWindow->setAttribute(Qt::WA_DeleteOnClose);

                    QTextEdit *textViewer = new QTextEdit(textWindow);
                    
                    if (decryptedData.startsWith("\xFF\xFE")) {  
                        QString text = QString::fromUtf16(reinterpret_cast<const char16_t*>(decryptedData.constData()), decryptedData.size() / 2);
                        textViewer->setPlainText(text);
                    } else if (decryptedData.startsWith("\xFE\xFF")) {  
                        QString text = QString::fromUtf16(reinterpret_cast<const char16_t*>(decryptedData.constData()), decryptedData.size() / 2).toUtf8();
                        textViewer->setPlainText(text);
                    } else {
                        textViewer->setPlainText(QString::fromUtf8(decryptedData));
                    }

                    textViewer->setReadOnly(true);
                    textWindow->setCentralWidget(textViewer);
                    textWindow->show();
                } else if (file.type == "IMAGE") {
                    QPixmap pixmap;
                    if (pixmap.loadFromData(decryptedData)) {
                        QLabel *imageLabel = new QLabel;
                        imageLabel->setPixmap(pixmap);
                        imageLabel->setWindowTitle(file.name);
                        imageLabel->setScaledContents(true);
                        imageLabel->resize(pixmap.size().boundedTo(QSize(800, 600)));
                        imageLabel->setAttribute(Qt::WA_DeleteOnClose);
                        imageLabel->show();
                    } else {
                        QMessageBox::warning(this, "Error", "Invalid image data.");
                    }
                }
            } else {
                QMessageBox::warning(this, "Error", "Decryption failed.");
            }
        } else {
            QMessageBox::warning(this, "Error", "Download failed.");
        }
        reply->deleteLater();
    });
}


void MainApp::downloadFile(const QString &fileId, const QString &fileName) {
    QString folder = QFileDialog::getExistingDirectory(this, "Select Output Folder");
    if (folder.isEmpty()) return;

    QNetworkRequest request(QUrl("http://localhost:3000/download/" + fileId));
    request.setRawHeader("Authorization", ("Bearer " + authToken).toUtf8());

    QNetworkReply *reply = networkManager.get(request);
    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray encryptedData = reply->readAll();
            QByteArray decryptedData;
            QByteArray key = QCryptographicHash::hash(globalPassword.toUtf8(), QCryptographicHash::Sha256);

            if (!decrypt(encryptedData, decryptedData, key)) {
                QMessageBox::warning(this, "Error", "Decryption failed.");
                return;
            }

            QFile outFile(folder + "/" + fileName);
            if (outFile.open(QIODevice::WriteOnly)) {
                outFile.write(decryptedData);
                outFile.close();
                QMessageBox::information(this, "Download Complete", "File saved successfully.");
            } else {
                QMessageBox::warning(this, "Error", "Failed to save file.");
            }
        } else {
            QMessageBox::warning(this, "Error", "Download failed.");
        }
        reply->deleteLater();
    });
}


void MainApp::renameFile(QListWidgetItem *item, const QString &fileId, const QString &currentName) {
    bool ok;
    QString newName = QInputDialog::getText(this, "Rename File", "New file name:", QLineEdit::Normal, currentName, &ok);
    if (!ok || newName.isEmpty()) return;

    QNetworkRequest request(QUrl("http://localhost:3000/rename"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", ("Bearer " + authToken).toUtf8());

    QJsonObject json;
    json["fileId"] = fileId;
    json["newName"] = newName;

    QNetworkReply *reply = networkManager.post(request, QJsonDocument(json).toJson());
    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            item->setText(newName);
            QMessageBox::information(this, "Rename Successful", "File renamed to " + newName);
            onRefresh();
        } else {
            QMessageBox::warning(this, "Rename Failed", "Error renaming file.");
        }
        reply->deleteLater();
    });
}


void MainApp::deleteFile(QListWidgetItem *item, const QString &fileId) {
    if (QMessageBox::question(this, "Confirm Delete", "Are you sure you want to delete this file?") != QMessageBox::Yes)
        return;

    QNetworkRequest request(QUrl("http://localhost:3000/delete/" + fileId));
    request.setRawHeader("Authorization", ("Bearer " + authToken).toUtf8());

    QNetworkReply *reply = networkManager.deleteResource(request);
    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            delete item;
            QMessageBox::information(this, "Delete Successful", "File deleted.");
        } else {
            QMessageBox::warning(this, "Delete Failed", "Error deleting file.");
        }
        reply->deleteLater();
    });
    onRefresh();
}



void MainApp::showContextMenu(const QPoint &pos) {
    QListWidgetItem *item = ui.filesListWidget->itemAt(pos);
    if (!item) return;

    QString fileId = item->data(Qt::UserRole).toString();

    FileItem file = getFileData(fileId);

    QMenu contextMenu;
    contextMenu.setStyleSheet("QMenu { background-color: #333; color: white; }"
                              "QMenu::item { padding: 8px 20px; }"
                              "QMenu::item:selected { background-color: #4A90E2; }");

    contextMenu.addAction("Filename: " + file.name)->setEnabled(false);
    contextMenu.addAction("Size: " + file.size)->setEnabled(false);
    contextMenu.addSeparator();

    QAction *previewAction = new QAction("Preview", &contextMenu);
    QAction *downloadAction = new QAction("Download", &contextMenu);
    QAction *renameAction = new QAction("Rename", &contextMenu);
    QAction *deleteAction = new QAction("Delete", &contextMenu);

    contextMenu.addAction(previewAction);
    contextMenu.addAction(downloadAction);
    contextMenu.addAction(renameAction);
    contextMenu.addAction(deleteAction);

    previewAction->setEnabled(file.type == "IMAGE" || file.type == "TEXT");
    previewAction->setVisible(file.type == "IMAGE" || file.type == "TEXT"); 
    previewAction->parentWidget()->update();
    connect(previewAction, &QAction::triggered, [this, fileId, file]() { previewFile(fileId, file); });
    connect(downloadAction, &QAction::triggered, [this, fileId, file]() { downloadFile(fileId, file.name); });
    connect(renameAction, &QAction::triggered, [this, item, fileId, file]() { renameFile(item, fileId, file.name); });
    connect(deleteAction, &QAction::triggered, [this, item, fileId]() { deleteFile(item, fileId); });

    contextMenu.exec(ui.filesListWidget->mapToGlobal(pos));
}
