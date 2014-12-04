/********************************************************************************
** Form generated from reading UI file 'vscvipcadd.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VSCVIPCADD_H
#define UI_VSCVIPCADD_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VSCVIPCAdd
{
public:
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_2;
    QGridLayout *gridLayout;
    QRadioButton *radioButtonFile;
    QLabel *labelIP;
    QLabel *labelPort;
    QLineEdit *lineEditPassword;
    QLabel *labelName;
    QLineEdit *lineEditUser;
    QLineEdit *lineEditName;
    QLabel *labelUser;
    QLabel *labelPassword;
    QLineEdit *lineEditPort;
    QRadioButton *radioButtonCamera;
    QComboBox *listIP;
    QComboBox *listCamera;
    QLineEdit *lineEditFile;
    QWidget *widget;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout_2;
    QPushButton *pushButtonCancel;
    QPushButton *pushButtonApply;

    void setupUi(QWidget *VSCVIPCAdd)
    {
        if (VSCVIPCAdd->objectName().isEmpty())
            VSCVIPCAdd->setObjectName(QStringLiteral("VSCVIPCAdd"));
        VSCVIPCAdd->resize(852, 499);
        VSCVIPCAdd->setStyleSheet(QStringLiteral(""));
        horizontalLayoutWidget_2 = new QWidget(VSCVIPCAdd);
        horizontalLayoutWidget_2->setObjectName(QStringLiteral("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(10, 20, 965, 302));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_2->setSpacing(5);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setHorizontalSpacing(2);
        gridLayout->setVerticalSpacing(0);
        radioButtonFile = new QRadioButton(horizontalLayoutWidget_2);
        radioButtonFile->setObjectName(QStringLiteral("radioButtonFile"));

        gridLayout->addWidget(radioButtonFile, 0, 0, 1, 1);

        labelIP = new QLabel(horizontalLayoutWidget_2);
        labelIP->setObjectName(QStringLiteral("labelIP"));

        gridLayout->addWidget(labelIP, 7, 0, 1, 1);

        labelPort = new QLabel(horizontalLayoutWidget_2);
        labelPort->setObjectName(QStringLiteral("labelPort"));

        gridLayout->addWidget(labelPort, 8, 0, 1, 1);

        lineEditPassword = new QLineEdit(horizontalLayoutWidget_2);
        lineEditPassword->setObjectName(QStringLiteral("lineEditPassword"));
        lineEditPassword->setMaximumSize(QSize(300, 16777215));

        gridLayout->addWidget(lineEditPassword, 10, 1, 1, 1);

        labelName = new QLabel(horizontalLayoutWidget_2);
        labelName->setObjectName(QStringLiteral("labelName"));

        gridLayout->addWidget(labelName, 5, 0, 1, 1);

        lineEditUser = new QLineEdit(horizontalLayoutWidget_2);
        lineEditUser->setObjectName(QStringLiteral("lineEditUser"));
        lineEditUser->setMaximumSize(QSize(300, 16777215));

        gridLayout->addWidget(lineEditUser, 9, 1, 1, 1);

        lineEditName = new QLineEdit(horizontalLayoutWidget_2);
        lineEditName->setObjectName(QStringLiteral("lineEditName"));
        lineEditName->setMaximumSize(QSize(300, 16777215));

        gridLayout->addWidget(lineEditName, 5, 1, 1, 1);

        labelUser = new QLabel(horizontalLayoutWidget_2);
        labelUser->setObjectName(QStringLiteral("labelUser"));

        gridLayout->addWidget(labelUser, 9, 0, 1, 1);

        labelPassword = new QLabel(horizontalLayoutWidget_2);
        labelPassword->setObjectName(QStringLiteral("labelPassword"));

        gridLayout->addWidget(labelPassword, 10, 0, 1, 1);

        lineEditPort = new QLineEdit(horizontalLayoutWidget_2);
        lineEditPort->setObjectName(QStringLiteral("lineEditPort"));
        lineEditPort->setMaximumSize(QSize(300, 16777215));

        gridLayout->addWidget(lineEditPort, 8, 1, 1, 1);

        radioButtonCamera = new QRadioButton(horizontalLayoutWidget_2);
        radioButtonCamera->setObjectName(QStringLiteral("radioButtonCamera"));

        gridLayout->addWidget(radioButtonCamera, 1, 0, 1, 1);

        listIP = new QComboBox(horizontalLayoutWidget_2);
        listIP->setObjectName(QStringLiteral("listIP"));

        gridLayout->addWidget(listIP, 7, 1, 1, 1);

        listCamera = new QComboBox(horizontalLayoutWidget_2);
        listCamera->setObjectName(QStringLiteral("listCamera"));

        gridLayout->addWidget(listCamera, 1, 1, 1, 1);

        lineEditFile = new QLineEdit(horizontalLayoutWidget_2);
        lineEditFile->setObjectName(QStringLiteral("lineEditFile"));
        lineEditFile->setMaximumSize(QSize(300, 16777215));

        gridLayout->addWidget(lineEditFile, 0, 1, 1, 1);


        horizontalLayout_2->addLayout(gridLayout);

        widget = new QWidget(horizontalLayoutWidget_2);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setMinimumSize(QSize(550, 300));
        widget->setMaximumSize(QSize(600, 400));
        widget->setStyleSheet(QStringLiteral(""));

        horizontalLayout_2->addWidget(widget);

        gridLayoutWidget = new QWidget(VSCVIPCAdd);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(240, 340, 191, 61));
        gridLayout_2 = new QGridLayout(gridLayoutWidget);
        gridLayout_2->setSpacing(0);
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
        pushButtonApply->setIconSize(QSize(60, 40));

        gridLayout_2->addWidget(pushButtonApply, 0, 0, 1, 1);


        retranslateUi(VSCVIPCAdd);

        QMetaObject::connectSlotsByName(VSCVIPCAdd);
    } // setupUi

    void retranslateUi(QWidget *VSCVIPCAdd)
    {
        VSCVIPCAdd->setWindowTitle(QApplication::translate("VSCVIPCAdd", "Form", 0));
        radioButtonFile->setText(QApplication::translate("VSCVIPCAdd", "File", 0));
        labelIP->setText(QApplication::translate("VSCVIPCAdd", "IP Address", 0));
        labelPort->setText(QApplication::translate("VSCVIPCAdd", "Port", 0));
        labelName->setText(QApplication::translate("VSCVIPCAdd", "Device Name", 0));
        labelUser->setText(QApplication::translate("VSCVIPCAdd", "User", 0));
        labelPassword->setText(QApplication::translate("VSCVIPCAdd", "Password", 0));
        radioButtonCamera->setText(QApplication::translate("VSCVIPCAdd", "Camera", 0));
        pushButtonCancel->setText(QApplication::translate("VSCVIPCAdd", "Cancel", 0));
        pushButtonApply->setText(QApplication::translate("VSCVIPCAdd", "Apply", 0));
    } // retranslateUi

};

namespace Ui {
    class VSCVIPCAdd: public Ui_VSCVIPCAdd {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VSCVIPCADD_H
