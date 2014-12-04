/********************************************************************************
** Form generated from reading UI file 'vsccameraadd.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VSCCAMERAADD_H
#define UI_VSCCAMERAADD_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VSCCameraAdd
{
public:
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_2;
    QGridLayout *gridLayout;
    QLineEdit *lineEditUser;
    QLineEdit *lineEditPassword;
    QLabel *labelRtspLoc;
    QLabel *labelUser;
    QRadioButton *radioButtonOnvif;
    QLineEdit *lineEditIP;
    QLabel *labelOnvifAddr;
    QLabel *labelPassword;
    QLineEdit *lineEditName;
    QCheckBox *checkBoxProfileToken;
    QLineEdit *lineEditProfileToken;
    QLabel *labelOnvifProfileToken;
    QLabel *labelIP;
    QRadioButton *radioButtonRtsp;
    QLineEdit *lineEditRtspLoc;
    QLineEdit *lineEditPort;
    QLabel *labelPort;
    QLineEdit *lineOnvifAddr;
    QLabel *labelName;
    QRadioButton *radioButtonFile;
    QLabel *labelRtspLoc_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QPushButton *pushButton;
    QWidget *widget;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout_2;
    QPushButton *pushButtonCancel;
    QPushButton *pushButtonApply;

    void setupUi(QWidget *VSCCameraAdd)
    {
        if (VSCCameraAdd->objectName().isEmpty())
            VSCCameraAdd->setObjectName(QStringLiteral("VSCCameraAdd"));
        VSCCameraAdd->resize(897, 510);
        VSCCameraAdd->setStyleSheet(QStringLiteral(""));
        horizontalLayoutWidget_2 = new QWidget(VSCCameraAdd);
        horizontalLayoutWidget_2->setObjectName(QStringLiteral("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(10, 20, 1070, 402));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_2->setSpacing(5);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setHorizontalSpacing(2);
        gridLayout->setVerticalSpacing(0);
        lineEditUser = new QLineEdit(horizontalLayoutWidget_2);
        lineEditUser->setObjectName(QStringLiteral("lineEditUser"));
        lineEditUser->setMaximumSize(QSize(300, 16777215));

        gridLayout->addWidget(lineEditUser, 9, 1, 1, 1);

        lineEditPassword = new QLineEdit(horizontalLayoutWidget_2);
        lineEditPassword->setObjectName(QStringLiteral("lineEditPassword"));
        lineEditPassword->setMaximumSize(QSize(300, 16777215));

        gridLayout->addWidget(lineEditPassword, 10, 1, 1, 1);

        labelRtspLoc = new QLabel(horizontalLayoutWidget_2);
        labelRtspLoc->setObjectName(QStringLiteral("labelRtspLoc"));

        gridLayout->addWidget(labelRtspLoc, 11, 0, 1, 1);

        labelUser = new QLabel(horizontalLayoutWidget_2);
        labelUser->setObjectName(QStringLiteral("labelUser"));

        gridLayout->addWidget(labelUser, 9, 0, 1, 1);

        radioButtonOnvif = new QRadioButton(horizontalLayoutWidget_2);
        radioButtonOnvif->setObjectName(QStringLiteral("radioButtonOnvif"));

        gridLayout->addWidget(radioButtonOnvif, 1, 0, 1, 1);

        lineEditIP = new QLineEdit(horizontalLayoutWidget_2);
        lineEditIP->setObjectName(QStringLiteral("lineEditIP"));
        lineEditIP->setMaximumSize(QSize(300, 16777215));

        gridLayout->addWidget(lineEditIP, 7, 1, 1, 1);

        labelOnvifAddr = new QLabel(horizontalLayoutWidget_2);
        labelOnvifAddr->setObjectName(QStringLiteral("labelOnvifAddr"));

        gridLayout->addWidget(labelOnvifAddr, 13, 0, 1, 1);

        labelPassword = new QLabel(horizontalLayoutWidget_2);
        labelPassword->setObjectName(QStringLiteral("labelPassword"));

        gridLayout->addWidget(labelPassword, 10, 0, 1, 1);

        lineEditName = new QLineEdit(horizontalLayoutWidget_2);
        lineEditName->setObjectName(QStringLiteral("lineEditName"));
        lineEditName->setMaximumSize(QSize(300, 16777215));

        gridLayout->addWidget(lineEditName, 5, 1, 1, 1);

        checkBoxProfileToken = new QCheckBox(horizontalLayoutWidget_2);
        checkBoxProfileToken->setObjectName(QStringLiteral("checkBoxProfileToken"));

        gridLayout->addWidget(checkBoxProfileToken, 15, 0, 1, 1);

        lineEditProfileToken = new QLineEdit(horizontalLayoutWidget_2);
        lineEditProfileToken->setObjectName(QStringLiteral("lineEditProfileToken"));
        lineEditProfileToken->setMaximumSize(QSize(300, 16777215));

        gridLayout->addWidget(lineEditProfileToken, 16, 1, 1, 1);

        labelOnvifProfileToken = new QLabel(horizontalLayoutWidget_2);
        labelOnvifProfileToken->setObjectName(QStringLiteral("labelOnvifProfileToken"));

        gridLayout->addWidget(labelOnvifProfileToken, 16, 0, 1, 1);

        labelIP = new QLabel(horizontalLayoutWidget_2);
        labelIP->setObjectName(QStringLiteral("labelIP"));

        gridLayout->addWidget(labelIP, 7, 0, 1, 1);

        radioButtonRtsp = new QRadioButton(horizontalLayoutWidget_2);
        radioButtonRtsp->setObjectName(QStringLiteral("radioButtonRtsp"));

        gridLayout->addWidget(radioButtonRtsp, 0, 0, 1, 1);

        lineEditRtspLoc = new QLineEdit(horizontalLayoutWidget_2);
        lineEditRtspLoc->setObjectName(QStringLiteral("lineEditRtspLoc"));
        lineEditRtspLoc->setMaximumSize(QSize(300, 16777215));

        gridLayout->addWidget(lineEditRtspLoc, 11, 1, 1, 1);

        lineEditPort = new QLineEdit(horizontalLayoutWidget_2);
        lineEditPort->setObjectName(QStringLiteral("lineEditPort"));
        lineEditPort->setMaximumSize(QSize(300, 16777215));

        gridLayout->addWidget(lineEditPort, 8, 1, 1, 1);

        labelPort = new QLabel(horizontalLayoutWidget_2);
        labelPort->setObjectName(QStringLiteral("labelPort"));

        gridLayout->addWidget(labelPort, 8, 0, 1, 1);

        lineOnvifAddr = new QLineEdit(horizontalLayoutWidget_2);
        lineOnvifAddr->setObjectName(QStringLiteral("lineOnvifAddr"));
        lineOnvifAddr->setMaximumSize(QSize(300, 16777215));

        gridLayout->addWidget(lineOnvifAddr, 13, 1, 1, 1);

        labelName = new QLabel(horizontalLayoutWidget_2);
        labelName->setObjectName(QStringLiteral("labelName"));

        gridLayout->addWidget(labelName, 5, 0, 1, 1);

        radioButtonFile = new QRadioButton(horizontalLayoutWidget_2);
        radioButtonFile->setObjectName(QStringLiteral("radioButtonFile"));

        gridLayout->addWidget(radioButtonFile, 0, 1, 1, 1);

        labelRtspLoc_2 = new QLabel(horizontalLayoutWidget_2);
        labelRtspLoc_2->setObjectName(QStringLiteral("labelRtspLoc_2"));

        gridLayout->addWidget(labelRtspLoc_2, 12, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(horizontalLayoutWidget_2);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        pushButton = new QPushButton(horizontalLayoutWidget_2);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setMinimumSize(QSize(30, 25));
        pushButton->setMaximumSize(QSize(30, 25));

        horizontalLayout->addWidget(pushButton);


        gridLayout->addLayout(horizontalLayout, 12, 1, 1, 1);


        horizontalLayout_2->addLayout(gridLayout);

        widget = new QWidget(horizontalLayoutWidget_2);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setMinimumSize(QSize(600, 400));
        widget->setMaximumSize(QSize(600, 400));
        widget->setStyleSheet(QStringLiteral(""));

        horizontalLayout_2->addWidget(widget);

        gridLayoutWidget = new QWidget(VSCCameraAdd);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(200, 430, 191, 61));
        gridLayout_2 = new QGridLayout(gridLayoutWidget);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        pushButtonCancel = new QPushButton(gridLayoutWidget);
        pushButtonCancel->setObjectName(QStringLiteral("pushButtonCancel"));
        pushButtonCancel->setMinimumSize(QSize(60, 30));
        pushButtonCancel->setMaximumSize(QSize(60, 30));
        pushButtonCancel->setStyleSheet(QLatin1String("QPushButton { \n"
"border:none;\n"
"color:white;\n"
"background-image: url(:/action/resources/button.png);}\n"
"QPushButton:hover {\n"
"	background-image: url(:/action/resources/buttonpress.png);}\n"
"QPushButton:pressed {\n"
"	background-image: url(:/action/resources/buttonpress.png);}\n"
""));
        pushButtonCancel->setIconSize(QSize(60, 40));

        gridLayout_2->addWidget(pushButtonCancel, 0, 1, 1, 1);

        pushButtonApply = new QPushButton(gridLayoutWidget);
        pushButtonApply->setObjectName(QStringLiteral("pushButtonApply"));
        pushButtonApply->setMinimumSize(QSize(60, 30));
        pushButtonApply->setMaximumSize(QSize(60, 30));
        pushButtonApply->setStyleSheet(QLatin1String("QPushButton { \n"
"border:none;\n"
"color:white;\n"
"background-image: url(:/action/resources/button.png);}\n"
"QPushButton:hover {\n"
"	background-image: url(:/action/resources/buttonpress.png);}\n"
"QPushButton:pressed {\n"
"	background-image: url(:/action/resources/buttonpress.png);}\n"
""));
        pushButtonApply->setIconSize(QSize(60, 30));

        gridLayout_2->addWidget(pushButtonApply, 0, 0, 1, 1);


        retranslateUi(VSCCameraAdd);

        QMetaObject::connectSlotsByName(VSCCameraAdd);
    } // setupUi

    void retranslateUi(QWidget *VSCCameraAdd)
    {
        VSCCameraAdd->setWindowTitle(QApplication::translate("VSCCameraAdd", "Form", 0));
        labelRtspLoc->setText(QApplication::translate("VSCCameraAdd", "RTSP Location", 0));
        labelUser->setText(QApplication::translate("VSCCameraAdd", "User", 0));
        radioButtonOnvif->setText(QApplication::translate("VSCCameraAdd", "ONVIF ProfileS", 0));
        labelOnvifAddr->setText(QApplication::translate("VSCCameraAdd", "ONVIF Address", 0));
        labelPassword->setText(QApplication::translate("VSCCameraAdd", "Password", 0));
        checkBoxProfileToken->setText(QApplication::translate("VSCCameraAdd", "Token ", 0));
        labelOnvifProfileToken->setText(QApplication::translate("VSCCameraAdd", "Profile Token", 0));
        labelIP->setText(QApplication::translate("VSCCameraAdd", "IP Address", 0));
        radioButtonRtsp->setText(QApplication::translate("VSCCameraAdd", "RTSP", 0));
        labelPort->setText(QApplication::translate("VSCCameraAdd", "Port", 0));
        labelName->setText(QApplication::translate("VSCCameraAdd", "Device Name", 0));
        radioButtonFile->setText(QApplication::translate("VSCCameraAdd", "File", 0));
        labelRtspLoc_2->setText(QApplication::translate("VSCCameraAdd", "File Location", 0));
        label->setText(QApplication::translate("VSCCameraAdd", "TextLabel", 0));
        pushButton->setText(QString());
        pushButtonCancel->setText(QApplication::translate("VSCCameraAdd", "Cancel", 0));
        pushButtonApply->setText(QApplication::translate("VSCCameraAdd", "Apply", 0));
    } // retranslateUi

};

namespace Ui {
    class VSCCameraAdd: public Ui_VSCCameraAdd {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VSCCAMERAADD_H
