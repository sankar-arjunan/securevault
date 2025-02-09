#ifndef MAINAPP_H
#define MAINAPP_H

#include <QMainWindow>
#include <QDialog>
#include <QNetworkAccessManager>
#include <QTextEdit>
#include "rojer_home.h"
#include "login.h"

struct FileItem {
    QString fileid;
    QString name;
    QString size;
    QString type;
};


class MainApp : public QMainWindow {
    Q_OBJECT

public:
    explicit MainApp(QWidget *parent = nullptr);

private slots:
    void onLoginClicked();
    void onClear();
    void onSendOTPClicked();
    void onSearch();
    void onRefresh();
    void onUpload();
    void showContextMenu(const QPoint &pos);
    void previewFile(const QString &fileId, const FileItem &file);
    void downloadFile(const QString &fileId, const QString &fileName);
    void renameFile(QListWidgetItem *item, const QString &fileId, const QString &currentName);
    void deleteFile(QListWidgetItem *item, const QString &fileId);


protected:
    void closeEvent(QCloseEvent *event) override; 

private:
    Ui::homePage ui;
    QDialog Dialog;
    Ui::Dialog loginUi;

    QString globalPassword;
    QString authToken;
    QNetworkAccessManager networkManager;
    void fetchFileList(const QString &query);
    void handleFileUpload(const QString &filePath);
    FileItem getFileData(const QString &fileId); 
};

#endif // MAINAPP_H
