/********************************************************************************
** Form generated from reading UI file 'rojer_home.ui'
**
** Created by: Qt User Interface Compiler version 5.15.15
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef ROJER_HOME_H
#define ROJER_HOME_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>
#include <QtGui/QIcon>
#include <QStyledItemDelegate>
#include <QMessageBox>

QT_BEGIN_NAMESPACE

class Ui_homePage
{
public:
    QWidget *centralwidget;
    QLabel *titleText;
    QLineEdit *searchBar;
    QToolButton *uploadButton;
    QToolButton *refreshButton;
    QListWidget *filesListWidget;


    void setupUi(QMainWindow *homePage)
    {
        if (homePage->objectName().isEmpty())
            homePage->setObjectName(QString::fromUtf8("homePage"));
        homePage->setFixedSize(640, 480);
        centralwidget = new QWidget(homePage);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        titleText = new QLabel(centralwidget);
        titleText->setObjectName(QString::fromUtf8("titleText"));
        titleText->setGeometry(QRect(20, 20, 171, 31));
        searchBar = new QLineEdit(centralwidget);
        searchBar->setObjectName(QString::fromUtf8("searchBar"));
        searchBar->setGeometry(QRect(150, 20, 250, 31));
        searchBar->setStyleSheet(QString::fromUtf8("border: 1px solid rgb(69, 69, 69);\n"
"background:  rgb(60, 60, 60);\n"
"padding: 5px;"));
        searchBar->setPlaceholderText(QCoreApplication::translate("homePage", "Search files...", nullptr));

        uploadButton = new QToolButton(centralwidget);
        uploadButton->setObjectName(QString::fromUtf8("uploadButton"));
        uploadButton->setGeometry(QRect(520, 20, 101, 31));
        uploadButton->setStyleSheet(QString::fromUtf8("font-size:12px;"));

        refreshButton = new QToolButton(centralwidget);
        refreshButton->setObjectName(QString::fromUtf8("refreshButton"));
        refreshButton->setGeometry(QRect(430, 20, 80, 31));
        refreshButton->setStyleSheet(QString::fromUtf8("font-size:12px;"));
        refreshButton->setText(QCoreApplication::translate("homePage", "Refresh", nullptr));


        filesListWidget = new QListWidget(centralwidget);
        filesListWidget->setObjectName(QString::fromUtf8("filesListWidget"));
        filesListWidget->setGeometry(QRect(20, 70, 601, 401));
        filesListWidget->setViewMode(QListView::IconMode);
        filesListWidget->setSpacing(12);
        filesListWidget->setDragEnabled(false);
        filesListWidget->setAcceptDrops(false);
        filesListWidget->setContentsMargins(10, 10, 10, 10);
        filesListWidget->setFlow(QListWidget::LeftToRight);
        filesListWidget->setLayoutDirection(Qt::LeftToRight);
        filesListWidget->setItemDelegate(new QStyledItemDelegate);
        filesListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
        int numColumns = 5;
        int itemWidth = (filesListWidget->width() - (numColumns - 1) * filesListWidget->spacing()) / numColumns;
        int iconSize = itemWidth * 0.6;
        filesListWidget->setIconSize(QSize(iconSize, iconSize));

        homePage->setCentralWidget(centralwidget);


        retranslateUi(homePage);

        QMetaObject::connectSlotsByName(homePage);
    } // setupUi

    void retranslateUi(QMainWindow *homePage)
    {
        homePage->setWindowTitle(QCoreApplication::translate("homePage", "Secure Vault", nullptr));
        titleText->setText(QCoreApplication::translate("homePage", "<html><head/><body><p><span style=\" font-size:18pt;\">Secure Vault</span></p></body></html>", nullptr));
        uploadButton->setText(QCoreApplication::translate("homePage", "Upload", nullptr));
    } // retranslateUi


};

namespace Ui {
    class homePage: public Ui_homePage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // ROJER_HOME_H
