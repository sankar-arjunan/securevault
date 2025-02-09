/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 5.15.15
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef LOGIN_H
#define LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QLineEdit *emailText;
    QLineEdit *OTPText;
    QLabel *emailLabel;
    QLabel *OTPlabel;
    QPushButton *loginButton;
    QPushButton *clearButton;
    QLabel *label;
    QLineEdit *keyText;
    QLabel *keyLabel;
    QPushButton *otpButton;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(320, 309);
        emailText = new QLineEdit(Dialog);
        emailText->setObjectName(QString::fromUtf8("emailText"));
        emailText->setGeometry(QRect(90, 70, 211, 31));
        OTPText = new QLineEdit(Dialog);
        OTPText->setObjectName(QString::fromUtf8("OTPText"));
        OTPText->setGeometry(QRect(120, 170, 181, 31));
        emailLabel = new QLabel(Dialog);
        emailLabel->setObjectName(QString::fromUtf8("emailLabel"));
        emailLabel->setGeometry(QRect(20, 70, 61, 31));
        OTPlabel = new QLabel(Dialog);
        OTPlabel->setObjectName(QString::fromUtf8("OTPlabel"));
        OTPlabel->setGeometry(QRect(20, 170, 61, 31));
        loginButton = new QPushButton(Dialog);
        loginButton->setObjectName(QString::fromUtf8("loginButton"));
        loginButton->setGeometry(QRect(200, 260, 111, 31));
        clearButton = new QPushButton(Dialog);
        clearButton->setObjectName(QString::fromUtf8("clearButton"));
        clearButton->setGeometry(QRect(10, 260, 121, 31));
        label = new QLabel(Dialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(40, 20, 231, 31));
        keyText = new QLineEdit(Dialog);
        keyText->setObjectName(QString::fromUtf8("keyText"));
        keyText->setGeometry(QRect(120, 210, 181, 31));
        keyLabel = new QLabel(Dialog);
        keyLabel->setObjectName(QString::fromUtf8("keyLabel"));
        keyLabel->setGeometry(QRect(20, 210, 91, 31));
        otpButton = new QPushButton(Dialog);
        otpButton->setObjectName(QString::fromUtf8("otpButton"));
        otpButton->setGeometry(QRect(200, 120, 111, 31));

        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QCoreApplication::translate("Dialog", "Login", nullptr));
        emailLabel->setText(QCoreApplication::translate("Dialog", "<html><head/><body><p>Email</p></body></html>", nullptr));
        OTPlabel->setText(QCoreApplication::translate("Dialog", "<html><head/><body><p>OTP</p></body></html>", nullptr));
        loginButton->setText(QCoreApplication::translate("Dialog", "Login", nullptr));
        clearButton->setText(QCoreApplication::translate("Dialog", "Clear", nullptr));
        label->setText(QCoreApplication::translate("Dialog", "<html><head/><body><p align=\"center\"><span style=\" font-size:18pt;\">Login</span></p></body></html>", nullptr));
        keyLabel->setText(QCoreApplication::translate("Dialog", "<html><head/><body><p>Decrypt Key</p></body></html>", nullptr));
        otpButton->setText(QCoreApplication::translate("Dialog", "Send OTP", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // LOGIN_H
